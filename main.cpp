#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <sstream>

#include "engine.h"
#include "character_creation.h"


#if defined( __WIN32__ ) || defined( _WIN32 )
#   undef min
#   undef max
#endif

// ----------- Character ------------
CharacterCreation* character_creation;
Character* character;
// ----------- END Character ------------
Engine* engine;

// ----------- SOME GLOBAL VARIABLES (TODO) -----------
// ----------- END SOME GLOBAL VARIABLES -----------

LRESULT CALLBACK WndProc(HWND wpHWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//WinMain: The entry point to everything Windows
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

// ---------- LOAD CONFIGURATION -----------
	int screen_x = 1024;
	int screen_y = 768;
// ---------- END LOAD CONFIGURATION -----------

// ---------- WINDOWS STUFF -----------
	HWND WindowHandle;
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Sword", NULL };
	RegisterClassEx(&wc);
	WindowHandle = CreateWindow( "Sword", "SWORD",
		WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );
	// there is a problem creating the window
	if (WindowHandle ==  NULL) {
		MessageBox(NULL, "Unable to Create a Window", "Error", MB_OK);
		return false;
	}
	ShowWindow		(WindowHandle, 1);
	UpdateWindow(WindowHandle);
	SetFocus		(WindowHandle);
	ShowCursor		(TRUE);
// ---------- END WINDOWS STUFF -----------


	character_creation = new CharacterCreation();
	Character::initStatic();
	character = character_creation->createCharacter();

	engine = new Engine(WindowHandle, screen_x, screen_y);
	engine->initGUI(character_creation);

	//Main loop
	MSG msg;
	while( msg.message!=WM_QUIT) {
		if(PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else {
			engine->render();
			engine->input();
		}
	}

	delete engine;
	delete character_creation;
	delete character;

	//Exit sucessfully
	return 0;
}


LRESULT CALLBACK WndProc(HWND wpHWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch(msg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
						 } break;


		default:break;
	}
	return DefWindowProc(wpHWnd, msg, wParam, lParam);
}
