#include "dc.h"
#include <stdlib.h>

DC* DC::Instance(const Size current_resolution, const eBitDepth bit_depth, const Uint32 nflags, const Uint32 initflags)
{
	static DC inst(current_resolution, bit_depth, nflags, initflags);
	return &inst;
/*	if(instance == NULL)
	{
		instance = new DC(current_resolution, bit_depth, nflags, initflags);
	} else
	{
	// setResolution etc?
	}
	return instance;*/
}	

DC::DC(const Size current_resolution, const eBitDepth bit_depth, const Uint32 nflags, const Uint32 initflags) :
	surface(NULL),
	oldSurface(NULL),
	pressedRectangle(false),
	initOK(true),
	brush(),
	pen(),
	color(NULL),
	textColor(),
	font(NULL),
// evtl gar nicht initialisieren...
	bitDepth(bit_depth),
	bits(0),
	resolution(current_resolution),
	Draw_HLine(NULL),
	Draw_VLine(NULL),
	Draw_Line(NULL),
	DrawFilledRound(NULL),
	DrawFilledEdgedRound(NULL),
	DrawEmptyEdgedRound(NULL),
	DrawEmptyRound(NULL),
	DrawFilledEdgedBorderRound(NULL)
{
	if ( SDL_Init(initflags) < 0 )
	{
		initOK = false;
		return;
	}
	atexit(SDL_Quit);
	setScreen(resolution, bitDepth, nflags);
}

DC::~DC()
{
	SDL_FreeSurface(surface);
}

void DC::switchToSurface(SDL_Surface* temp_surface) 
{
#ifdef _SCC_DEBUG
	if(oldSurface != NULL) {
		toErrorLog("DEBUG (DC::switchToSurface()): Original Surface was not restored!");return;
	}
#endif
	oldSurface = surface;
	surface = temp_surface;
}

void DC::switchToOriginalSurface()
{
#ifdef _SCC_DEBUG
	if(oldSurface == NULL) {
		toErrorLog("DEBUG (DC::switchToOriginalSurface()): Already at original surface!");return;
	}
#endif
	surface = oldSurface;
	oldSurface = NULL;
}

const bool DC::isSurfacePuffered() const
{
	return(oldSurface != NULL);
}

SDL_Cursor* DC::createCursor(char* xpm_image[])
{
	int i, row, col;
	Uint8 data[4*32];
	Uint8 mask[4*32];
	int hot_x, hot_y;
  
	i = -1;
	for ( row=0; row<32; ++row ) {
		for ( col=0; col<32; ++col ) {
			if ( col % 8 ) {
				data[i] <<= 1;
				mask[i] <<= 1;
			} else {
				++i;
				data[i] = mask[i] = 0;
			}
			switch (xpm_image[4+row][col]) {
				case '+':
					data[i] |= 0x01;
					mask[i] |= 0x01;
					break;
				case '.':
					mask[i] |= 0x01;
					break;
				case ' ':
					break;
			}
		}
	}
	sscanf(xpm_image[4+row], "%d,%d", &hot_x, &hot_y);
	return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}




std::ostringstream DC::getAvailibleDrivers()
{
	std::ostringstream availible_drivers;
#ifdef __WIN32__
	availible_drivers << " directx ";
	availible_drivers << " windib";
#elif __linux__
	availible_drivers << " x11 ";
	availible_drivers << " dga ";
	availible_drivers << " nano ";
	availible_drivers << " fbcon ";
	availible_drivers << " directfb ";
	availible_drivers << " svgalib ";	
	availible_drivers << " aalib";
#endif
	return(availible_drivers);
}




void DC::setFullscreen(const bool full_screen)
{
	if(((surface->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN) != full_screen)
	{
#ifdef __linux
		SDL_WM_ToggleFullScreen(surface);
#elif __WIN32__
		Uint32 nflags = surface->flags;
		nflags ^= SDL_FULLSCREEN;
		surface = SDL_SetVideoMode(surface->w, surface->h, surface->format->BitsPerPixel, nflags);
#endif
	}										
}

void DC::addRectangle(const Rect& rect)
{
	if((changedRectangles>199) || (rect.getLeft() >= max_x) || (rect.getTop() >= max_y) || (rect.getRight() < 0) || (rect.getBottom() < 0))
		return;
	SDL_Rect r;
	if(rect.getLeft() < 0)
		r.x = 0;
	else
		r.x = rect.getLeft();

	if(rect.getTop() < 0)
		r.y = 0;
	else 
		r.y = rect.getTop();
	
	if(rect.getRight() >= max_x)
		r.w = max_x - r.x;
	else 
		r.w = 1 + rect.getRight() - r.x;

	if(rect.getBottom() >= max_y)
		r.h = max_y - r.y;
	else 
		r.h = 1 + rect.getBottom() - r.y;

	for(unsigned int i = 0; i < changedRectangles; i ++)
	{
		if(
			(r.x >= changedRectangle[i].x)&&
			(r.x + r.w <= changedRectangle[i].x + changedRectangle[i].w)&&
			(r.y >= changedRectangle[i].y)&&
			(r.y + r.h <= changedRectangle[i].y + changedRectangle[i].h)
		)
			return;
		if(
			(r.x <= changedRectangle[i].x)&&
			(r.x + r.w >= changedRectangle[i].x + changedRectangle[i].w)&&
			(r.y <= changedRectangle[i].y)&&
			(r.y + r.h >= changedRectangle[i].y + changedRectangle[i].h)
		)
		{
			changedRectangle[i] = r;
			return;
		}
	}
	
	changedRectangle[changedRectangles] = r;
	changedRectangles++;
}

void DC::updateScreen()
{
//	for(unsigned int i = changedRectangles; i--;)
//	{
//		setPen(Pen(surface, rand()%256, rand()%256, rand()%256, 1, SOLID_PEN_STYLE)); 
//		DrawEmptyRectangle(changedRectangle[i].x+2, changedRectangle[i].y+2, changedRectangle[i].w-4, changedRectangle[i].h-4);
//	}
	SDL_UpdateRects(surface, changedRectangles, changedRectangle);
	changedRectangles=0;
}

void DC::DrawBrightenedBitmap(SDL_Surface* bitmap, const signed x, const signed y, const Rect& clip_rect, const unsigned int brightness) const
{
//	if(brightness == 100)
//	{
		SDL_SetAlpha(bitmap, SDL_SRCALPHA, brightness * 255 / 100);
		DrawBitmap(bitmap, x, y, clip_rect);
		SDL_SetAlpha(bitmap, SDL_SRCALPHA, 255);
		return;
//	}
	double brightness_percent = brightness / 100;
	SDL_Surface* neu = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap->w, bitmap->h, 8, 0, 0, 0, 0);
	Uint32* pc = (Uint32*)(bitmap->pixels);
	Uint32* dst = (Uint32*)(neu->pixels);

	for(unsigned int i = bitmap->w * bitmap->h; i--;)
	{
		Uint8 r,g,b;
		SDL_GetRGB(*pc, bitmap->format, &r, &g, &b);
//		r = r/2;// * brightness_percent; if(r > 255) r = 255;
//		g = g/2;// * brightness_percent; if(g > 255) g = 255;
//		b = b/2;// * brightness_percent; if(b > 255) b = 255;
		*dst = SDL_MapRGB(bitmap->format, r, g, b);
		++pc;
		++dst;
	}
	DrawBitmap(neu, x, y, clip_rect);
	SDL_FreeSurface(neu);
}

void DC::DrawBrightenedBitmap(SDL_Surface* bitmap, const signed x, const signed y, const unsigned int brightness) const
{
//	if(brightness == 100)
//	{
		SDL_SetAlpha(bitmap, SDL_SRCALPHA, brightness * 255 / 100);
		DrawBitmap(bitmap, x, y);
		SDL_SetAlpha(bitmap, SDL_SRCALPHA, 255);
		return;
//	}

	double brightness_percent = brightness / 100;
	SDL_Surface* neu = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap->w, bitmap->h, 8, 0, 0, 0, 0);
	Uint32* pc = (Uint32*)(bitmap->pixels);
	Uint32* dst = (Uint32*)(neu->pixels);

	for(unsigned int i = bitmap->w * bitmap->h; i--;)
	{
		Uint8 r,g,b;
		SDL_GetRGB(*pc, bitmap->format, &r, &g, &b);
//		r = r /2;//* brightness_percent; if(r > 255) r = 255;
//		g = g /2;//* brightness_percent; if(g > 255) g = 255;
//		b = b /2;//* brightness_percent; if(b > 255) b = 255;
//		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;
		*dst = SDL_MapRGB(bitmap->format, r, g, b);
		++pc;
		++dst;
	}
	DrawBitmap(neu, x, y);
	SDL_FreeSurface(neu);
}

void DC::DrawBitmap(SDL_Surface* bitmap, const signed int x, const signed int y) const
{
	SDL_Rect drect;
	drect.x = x;
	drect.y = y;
	if((drect.x + bitmap->w >= max_x)||(drect.y + bitmap->h >= max_y) || (drect.x < 0) || (drect.y < 0))
	{
		DrawBitmap(bitmap, x, y, Rect(0, 0, max_x, max_y));
		return;
	}
	SDL_BlitSurface(bitmap , NULL, surface, &drect);
}

// clipped bitmap
void DC::DrawBitmap(SDL_Surface* bitmap, const signed int x, const signed int y, const Rect& clip_rect) const
{
	SDL_Rect srect;
	SDL_Rect drect;
	drect.x = x;
	drect.y = y;
	srect.x = 0;
	srect.y = 0;
	srect.w = bitmap->w;
	srect.h = bitmap->h;

	if((clip_rect.getLeft() > x + srect.w) || (clip_rect.getTop() > y + srect.h) || (clip_rect.getBottom() < y) || (clip_rect.getRight() < x))
		return;
	if(clip_rect.getLeft() > x)
	{
		srect.x = clip_rect.getLeft() - x;
		srect.w -= srect.x;
		drect.x = clip_rect.getLeft();
	}
	if(clip_rect.getTop() > y)
	{
		srect.y = clip_rect.getTop() - y;
		srect.h -= srect.y;
		drect.y = clip_rect.getTop();
	}
	if(clip_rect.getRight() < x + srect.w)
		srect.w = clip_rect.getRight() - x;
	
	if(clip_rect.getBottom() < y + srect.h)
		srect.h = clip_rect.getBottom() - y;
	
	SDL_BlitSurface(bitmap , &srect, surface, &drect);
}

const Color DC::mixColor(const Color& id1, const Color& id2)  const
{
	return(Color(surface, id1.r()  +id2.r(),
			id1.g()+id2.g(),
			id1.b() +id2.b()));
}

const Color DC::mixColor(const Color& id1, const Color& id2, const unsigned int gradient) const
{
	return(Color(surface, (id1.r()*gradient  +id2.r()*(100-gradient))/100,
		(id1.g()*gradient+id2.g()*(100-gradient))/100,
		(id1.b()*gradient +id2.b()*(100-gradient))/100));
}

const Color DC::changeAbsoluteBrightness(const Color& id, const signed int brightness) const
{
	return(Color(surface, id.r() + brightness,
			id.g() + brightness,
			id.b() + brightness));
}

const Color DC::changeRelativeBrightness(const Color& id, const unsigned int brightness_percent) const
{
	int r = id.r() * brightness_percent / 100;if(r>255) r = 255;
	int g = id.g() * brightness_percent / 100;if(g>255) g = 255;
	int b = id.b() * brightness_percent / 100;if(b>255) b = 255;
	return(Color(surface, r, g , b));
}

void DC::DrawLine(const signed x1, const signed y1, const signed x2, const signed y2) const
{
	if(pen.getStyle()==TRANSPARENT_PEN_STYLE)
		return;
	signed int xx1, xx2, yy1, yy2;
	if(x1>x2)
		{xx1=x2;xx2=x1;yy1=y2;yy2=y1;}
	else {xx1=x1;xx2=x2;yy1=y1;yy2=y2;}

	if((xx1<pen.getWidth()/2)||(yy1+1<pen.getWidth()/2)||(yy2<pen.getWidth()/2)||(xx2>=max_x-pen.getWidth()/2)||(yy1>=max_y-pen.getWidth()/2)||(yy2>=max_y-pen.getWidth()/2))
		return;
		
	if(yy1==yy2)
	{
		if(xx1==xx2)
			return;
		if(pen.getWidth()>1)
		{
			SDL_Rect rc;
			rc.x=xx1/*-pen.getWidth()/2*/;rc.y=yy1/*-pen.getWidth()/2*/;rc.w=xx2-xx1/*+pen.getWidth()*/;rc.h=pen.getWidth();
			SDL_FillRect(surface, &rc, (Uint32)(*pen.getColor()) );
		} else
			(*this.*Draw_HLine)(xx1, yy1, xx2);
	} else
	if(xx1==xx2)
	{
		if(yy1>yy2)
			{signed int swp=yy1;yy1=yy2;yy2=swp;}
		if(pen.getWidth()>1)
		{
			SDL_Rect rc;
			rc.x=xx1/*-pen.getWidth()/2*/;rc.y=yy1/*-pen.getWidth()/2*/;rc.w=pen.getWidth();rc.h=yy2-yy1/*+pen.getWidth()*/;
			SDL_FillRect(surface, &rc, (Uint32)(*pen.getColor()) );
		} else
			(*this.*Draw_VLine)(xx1, yy1, yy2);
	} else
	{
		(*this.*Draw_Line)(xx1, yy1, xx2, yy2);
		if(pen.getWidth()==2)
		{
//			Color c = *pen.getColor();
//			const_cast<DC*>(this)->pen.setColor(Color(surface, (Uint8)(pen.getColor()->r()*0.5),  (Uint8)(pen.getColor()->g()*0.5), (Uint8)(pen.getColor()->b()*0.5)));
			(*this.*Draw_Line)(xx1, yy1+1, xx2, yy2+1);
			(*this.*Draw_Line)(xx1, yy1, xx2, yy2);
			(*this.*Draw_Line)(xx1, yy1-1, xx2, yy2-1);
//			const_cast<DC*>(this)->pen.setColor(c);
		}
	}
}


void DC::DrawSpline(const unsigned int c, const Point* p) const
{
	if((pen.getStyle() == TRANSPARENT_PEN_STYLE)||(c<2))
		return;
	for(unsigned int i=c-1;i--;)
//	{
//		aalineColor(surface, p[i].x, p[i].y, p[i+1].x, p[i+1].y, (Uint32)(*pen.getColor()));
//		aalineColor(surface, p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1, (Uint32)(*pen.getColor()));
//	}
	{
/*		if((i>0)&&(i<c-2))
		{
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y),
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y));
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y) + 1,
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y) + 1);
		} else*/
		{
			DrawLine(p[i].x, p[i].y, p[i+1].x, p[i+1].y);
//			DrawLine(p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1);
		}
	}
}
	
void DC::DrawSpline(const unsigned int c, const Point* p, const Point s) const
{
	if((pen.getStyle() == TRANSPARENT_PEN_STYLE)||(c<2))
		return;
	for(unsigned int i=c-1;i--;)
//	{
//		aalineColor(surface, p[i].x, p[i].y, p[i+1].x, p[i+1].y, (Uint32)(*pen.getColor()));
//		aalineColor(surface, p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1, (Uint32)(*pen.getColor()));
//	}
	{
/*		if((i>0)&&(i<c-2))
		{
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y),
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y));
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y) + 1,
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y) + 1);
		} else*/
		{
			DrawLine(s.x + p[i].x, s.y + p[i].y, s.x + p[i+1].x, s.y + p[i+1].y);
	//		DrawLine(s.x + p[i].x, s.y + p[i].y+1, s.x + p[i+1].x, s.y + p[i+1].y+1);
		}
	}
	
}
void DC::DrawText(const std::string& text, const signed int x, const signed int y) const 
{
	if(font->isShadow())
		font->DrawText(surface, toSDL_Color(0,0,0), text, x+font->getSize()/6, y+font->getSize()/6);

	font->DrawText(surface, textColor, text, x, y);
	if(font->isUnderlined())
	{
		Size s = font->getTextExtent(text);
		SDL_Rect r;
		r.x = x;r.y = y+s.getHeight()*2/3;r.w = s.getWidth();r.h = 1;
		SDL_FillRect(surface, &r, Color(surface, textColor.r, textColor.g, textColor.b));
	}

}

void DC::setScreen(const Size current_resolution, const eBitDepth bit_depth, const Uint32 nflags)
{
	if((current_resolution == resolution) && (bit_depth == bitDepth) && (surface!=NULL))
		return;
	resolution = current_resolution;
	max_x = resolution.getWidth();
	max_y = resolution.getHeight();
	switch(bit_depth)
	{
		case DEPTH_8BIT:bits=8;break;
		case DEPTH_16BIT:bits=16;break;
		case DEPTH_24BIT:bits=24;break;
		case DEPTH_32BIT:bits=32;break;
		default:bits=8;break;
	}
//	const SDL_VideoInfo* hardware = SDL_GetVideoInfo();
//	if(bits > (unsigned int)hardware->vfmt->BitsPerPixel)
//		bits = (unsigned int)hardware->vfmt->BitsPerPixel; :(

	surface = SDL_SetVideoMode(max_x, max_y, bits, nflags);
	if(surface==NULL)
		return;
	SDL_Rect r;
	r.x=0;r.y=0;r.w=max_x;r.h=max_y;
	changedRectangle[0] = r;
	changedRectangles = 1;

	switch(surface->format->BitsPerPixel)
	{
		case 8:	bitDepth = DEPTH_8BIT;
			Draw_HLine = &DC::Draw_HLine_8bit;
			Draw_VLine = &DC::Draw_VLine_8bit;
			Draw_Line = &DC::Draw_Line_8bit;
			DrawFilledRound = &DC::DrawFilledRound_8bit;
			DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_8bit;
			DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_8bit;
			DrawEmptyRound = &DC::DrawEmptyRound_8bit;
			DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_8bit;
			DrawTab = &DC::DrawTab_8bit;
			break;
		case 16:bitDepth = DEPTH_16BIT;
			Draw_HLine = &DC::Draw_HLine_16bit;
		        Draw_VLine = &DC::Draw_VLine_16bit;
		        Draw_Line = &DC::Draw_Line_16bit;
		        DrawFilledRound = &DC::DrawFilledRound_16bit;
		        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_16bit;
		        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_16bit;
		        DrawEmptyRound = &DC::DrawEmptyRound_16bit;
		        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_16bit;
			DrawTab = &DC::DrawTab_16bit;
			break;
		case 24:bitDepth = DEPTH_24BIT;
			Draw_HLine = &DC::Draw_HLine_24bit;
		        Draw_VLine = &DC::Draw_VLine_24bit;
		        Draw_Line = &DC::Draw_Line_24bit;
		        DrawFilledRound = &DC::DrawFilledRound_24bit;
		        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_24bit;
		        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_24bit;
		        DrawEmptyRound = &DC::DrawEmptyRound_24bit;
		        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_24bit;
			DrawTab = &DC::DrawTab_24bit;
			break;
		case 32:bitDepth = DEPTH_32BIT;
			Draw_HLine = &DC::Draw_HLine_32bit;
		        Draw_VLine = &DC::Draw_VLine_32bit;
		        Draw_Line = &DC::Draw_Line_32bit;
		        DrawFilledRound = &DC::DrawFilledRound_32bit;
		        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_32bit;
		        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_32bit;
		        DrawEmptyRound = &DC::DrawEmptyRound_32bit;
		        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_32bit;
			DrawTab = &DC::DrawTab_32bit;
			break;
		default:break;
	}
}



void DC::setBitDepth(const eBitDepth bit_depth)
{
	setScreen(resolution, bit_depth, surface->flags);
}

void DC::setResolution(const Size current_resolution)
{
	setScreen(current_resolution, bitDepth, surface->flags);
}

void DC::setColor(const Color* dc_color) 
{
#ifdef _SCC_DEBUG
	if(dc_color==NULL)
	{
		toErrorLog("ERROR (DC::setColor()): Color not initialized.");
		return;
	}
#endif
	color = dc_color;
}

void DC::setFont(const Font* dc_font) 
{
#ifdef _SCC_DEBUG
	if(dc_font==NULL)
	{
		toErrorLog("ERROR (DC::setFont()): Font not initialized.");
		return;
	}
#endif
	font = dc_font;
}


#if 0
void DC::DrawGridEdgedRoundedRectangle(const signed int x, const signed y, const unsigned width, const unsigned int height, const unsigned int radius, std::list<Rect> notDrawRectList) const 
{
	return;
	if((width<2)||(height<2)) return;
	if((x>=max_x)||(x<0)) return;
	if((y>=max_y)||(y<0)) return;
	unsigned int ww = width;
	unsigned int hh = height;
	if(x+ww>=max_x) ww = max_x - 1 - x;
	if(y+hh>=max_y) hh = max_y - 1 - y;
	bool rounded = false;
	if((radius <= 1)||(ww<2*radius)||(hh<2*radius))
		rounded=true;

	signed int xx = x;
	signed int yy = y;
	unsigned int lastHeight = 0;

	std::list<Rect>::const_iterator i = notDrawRectList.begin();
	while(i!=notDrawRectList.end())
	{
		lastHeight=0;
		while((i!=notDrawRectList.end())&&(yy == i->getTop()))
		{
			DrawRectangle(xx, yy, i->getLeft() - xx, i->getHeight());
			lastHeight = i->getHeight();
			xx = i->getLeft() + i->getWidth();
			i++;
		}
		// rechter Rand
		DrawRectangle(xx, yy, x+ww - xx, lastHeight);
		// neue Zeile
		xx = x;
		yy += lastHeight;
		if((i!=notDrawRectList.end())&&(yy < i->getTop()))
		{
			DrawRectangle(xx, yy, ww, i->getTop() - yy);
			yy = i->getTop();
			xx = x;
		}
	}
	DrawRectangle(xx, yy, x+ww - xx, y + hh - yy);		
				
/*	if(rounded) {
		DrawRectangle(x,y,ww,hh);
		return;
	}
	
	if (brush.getStyle() != TRANSPARENT_BRUSH_STYLE)
		DrawFilledEdgedRound(x, y, ww, hh, radius);
	if (pen.getStyle() != TRANSPARENT_PEN_STYLE)
		DrawEmptyEdgedRound(x, y, ww, hh, radius);*/
}
#endif

void DC::DrawEdgedRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const
{
	if((width<2)||(height<2)||(x>=max_x)||(y>=max_y)||(x<0)||(y<0))
		return;
	
	unsigned int mw = width;
	unsigned int mh = height;

	if(x + width >= max_x)
		mw = max_x - x;
	if(y + height >= max_y)
		mh = max_y - y;
	
	if((radius <= 1)||(mw<2*radius)||(mh<2*radius))	{
		DrawRectangle(x,y,mw,mh);
		return;
	}

	if(brush.getStyle() == TRANSPARENT_BRUSH_STYLE)
	{
		if(pen.getStyle() != TRANSPARENT_PEN_STYLE)
			(*this.*DrawEmptyEdgedRound)(x, y, mw, mh, radius);
	} else
	if(pen.getStyle() == TRANSPARENT_PEN_STYLE)
		(*this.*DrawFilledEdgedRound)(x, y, mw, mh, radius);
	else 
		(*this.*DrawFilledEdgedBorderRound)(x, y, mw, mh, radius);
}

void DC::DrawRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const
{
	if((width<2)||(height<2)||(x+width>=max_x)||(y+height>=max_y)||(x<0)||(y<0)) return;
	
	if((radius <= 1)||(width<2*radius)||(height<2*radius))	{
		DrawRectangle(x,y, width, height);
		return;
	}

	if (brush.getStyle() != TRANSPARENT_BRUSH_STYLE)
		(*this.*DrawFilledRound)(x, y, width, height, radius);
	if (pen.getStyle() != TRANSPARENT_PEN_STYLE)
		(*this.*DrawEmptyRound)(x, y, width, height, radius);
}


void DC::DrawTabRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const
{
	if((width<2)||(height<2)||(x+width>=max_x)||(y+height>=max_y)||(x<0)||(y<0)) return;
	
	(*this.*DrawTab)(x, y, width, height);
}


void DC::DrawRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const
{
	if((width<2)||(height<2)||(width>max_x)||(height>max_y)||(x<0)||(y<0)) return;
	SDL_Rect rc;
	rc.x=x+1;rc.y=y+1;rc.w=width-2;rc.h=height-2;
	if(brush.getStyle() != TRANSPARENT_BRUSH_STYLE)
		SDL_FillRect(surface, &rc, (Uint32)(*brush.getColor()) );
	DrawEmptyRectangle(x, y, width, height);	
}

void DC::DrawEmptyRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const
{
	if(pen.getStyle()==TRANSPARENT_PEN_STYLE) 
		return;
	if((width<2)||(height<2)||(width>max_x)||(height>max_y)||(x<0)||(y<0)) return;
	
	Uint32 dark_pen_col, bright_pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.getColor(), 110));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.getColor(), 70));
	
	} else
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.getColor(), 80));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.getColor(), 120));
	}

	SDL_Rect rc;
	rc.x=x-(pen.getWidth()>>1);rc.y=y-(pen.getWidth()>>1);rc.w=width;rc.h=pen.getWidth();
	SDL_FillRect(surface, &rc, bright_pen_col);
	
	rc.x=x-(pen.getWidth()>>1);rc.y=y+height-1-(pen.getWidth()>>1);rc.w=width;rc.h=pen.getWidth();
	SDL_FillRect(surface, &rc, dark_pen_col);
	
	rc.x=x-(pen.getWidth()>>1);rc.y=y-(pen.getWidth()>>1);rc.w=pen.getWidth();rc.h=height;
	SDL_FillRect(surface, &rc, bright_pen_col);
	
	rc.x=x+width-1-(pen.getWidth()>>1);rc.y=y-(pen.getWidth()>>1);rc.w=pen.getWidth();rc.h=height;
	SDL_FillRect(surface, &rc, dark_pen_col);
}

void DC::DrawVerticalLine(const signed int x0, const signed int y0, const signed int y1) const
{
	if((y1<0)||(y0>=max_y)||(x0<0)||(x0>=max_x)||(y1<y0))
	{
	//	std::ostringstream os;
	//	os.str("");
	//	os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
	//	toErrorLog(os.str());
		return;
	}
	signed int yy0;
	signed int yy1;
	if(y0 < 0)
		yy0=0;
	else yy0=y0;
	if(y1>=max_y)
		yy1=max_y-1;
	else 
		yy1=y1;
	(*this.*Draw_VLine)(x0, yy0, yy1);
}

void DC::DrawHorizontalLine(const signed int x0, const signed int y0, const signed int x1) const
{
	if((x1<0)||(x0>=max_x)||(y0<0)||(y0>=max_y)||(x1<x0))
	{
//	  std::ostringstream os;
//	  os.str("");
//	  os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
//		toLog("Line out of range");//os.str());
		return;
	}
	signed int xx0;
	signed int xx1;
	if(x0 < 0)
		xx0=0;
	else xx0=x0;
	if(x1>=max_x)
		xx1=max_x-1;
	else
		xx1=x1;
	(*this.*Draw_HLine)(xx0, y0, xx1);
}

const bool DC::Lock() const {
	if(SDL_MUSTLOCK(surface)) {
		return SDL_LockSurface(surface) == 0;
	}
	else return true;
}

void DC::Unlock() const {
	if(SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}

#include <sstream>
const eChooseDriverError DC::chooseDriver(std::list<std::string>& parameter_list, std::string& current_driver)
{
	if(getenv("SDL_VIDEODRIVER")!=NULL)
		current_driver = getenv("SDL_VIDEODRIVER");

	bool no_vo_argument = false; // TODO

	for(std::list<std::string>::iterator i = parameter_list.begin(); i != parameter_list.end(); ++i)
		if((*i) == "-vo")
		{
			i = parameter_list.erase(i);
			if(i == parameter_list.end())
				no_vo_argument = true;
			else current_driver = *i;
			parameter_list.erase(i);
			break;
		}
	
	std::list<std::string> availible_drivers = DC::getAvailibleDrivers();
	if(availible_drivers.empty())
		return(NO_VIDEO_DRIVERS_AVAILIBLE);
	if(current_driver == "")
		current_driver = *availible_drivers.begin();
	for(std::list<std::string>::const_iterator j = availible_drivers.begin(); j != availible_drivers.end(); ++j)
		if(current_driver == *j)
		{
			std::ostringstream video;
			video.str("");
			video << "SDL_VIDEODRIVER=" << current_driver;
			char* video_cstr = new char[strlen(video.str().c_str())];
			strcpy(video_cstr, video.str().c_str());
			putenv(video_cstr);
			return(NO_DRIVER_ERROR);
		}
	return(SDL_DRIVER_NOT_SUPPORTED);
}

SDL_Rect DC::changedRectangle[200];
unsigned int DC::changedRectangles=0;
Uint16 DC::max_x = 0;
Uint16 DC::max_y = 0;
//DC* DC::instance = NULL;
