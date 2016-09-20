#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <sstream>
#include "CTVEngine.h"
#include "CTVScene.h"
#include "CTVInputEngine.h"
#include "CTVTextureFactory.h"
#include "CTVMaterialFactory.h"
#include "CTVAtmosphere.h"
#include "CTVGlobals.h"
#include "CTVMesh.h"
#include "CTVLightEngine.h"
#include "tv_types.h"
#include "HelperFunctions.h"
#include "CTVGraphicEffect.h"
#include "CTVRenderSurface.h"
#include "CTVInternalObjects.h"
#include "CTVScreen2DText.h"
#include "CTVMathLibrary.h"

#include <CEGUI.h>
#include <CEGUIImageset.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIFactoryModule.h>
#include <CEGUIWindow.h>
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIConfig.h>
#include <directx9GUIRenderer/d3d9renderer.h>

#include "camera.h"
#include "actor.h"
#ifdef CEGUI_WITH_XERCES
#   include "CEGUIXercesParser.h"
#endif

#if defined( __WIN32__ ) || defined( _WIN32 )
#   undef min
#   undef max
#endif

// ----------- CEGUI -----------
CEGUI::Renderer* mGUIRenderer;
CEGUI::System* mGUISystem;
CEGUI::Window* mEditorGuiSheet;
CEGUI::FrameWindow* wnd;
CEGUI::Window* stamina_bar;
CEGUI::Window* text;
LPDIRECT3DDEVICE9 d3Device;
// ----------- END CEGUI -----------

// ----------- TV3D -----------
CTVEngine* pEngine;
CTVInputEngine* pInput;
CTVScene* pScene;
CTVGlobals* pGlobals;
CTVInternalObjects* TVInternal;

CTVScreen2DText* pScreen2DText;
CTVTextureFactory* pTexFactory;
CTVMaterialFactory* pMatFactory;
CTVLandscape* pLand;
CTVAtmosphere* pAtmos;

CTVMathLibrary* pMathLibrary;
CTVGraphicEffect* pEffect_background;
CTVRenderSurface* pRenderSurface_background;
// ----------- END TV3D -----------

// ----------- OBJECTS -----------
#define MAX_ACTORS 10
#define TREE_QUANTITY 40
MyActor* pActor[MAX_ACTORS];
//CTVMesh** TheTree;
CTVMesh** TheRealTree;
MyCamera* pCamera;
// ----------- END OBJECTS -----------

// ----------- SOME GLOBAL VARIABLES (TODO) -----------
int my_font;
int black_material, glow_material, tree_material, normal_material;
bool oldRetButton1 = 0;
bool oldRetButton2 = 0;
bool oldRetButton3 = 0;
bool oldRetButton4 = 0;
bool menuShown = false;

void render();
void input();
void unload();
// ----------- END SOME GLOBAL VARIABLES -----------

LRESULT CALLBACK WndProc(HWND wpHWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//WinMain: The entry point to everything Windows
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

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


// ---------- BASIC TV3D ENGINE INITIALIZATION ----------
	//Create the TVEngine
	pEngine = new CTVEngine();
	pEngine->SetDebugFile("..\\debug.txt");

	//pEngine->Init3DWindowed(WindowHandle, true);
	pEngine->Init3DFullscreen (1024,768);
	pEngine->SetAntialiasing(true, cTV_MULTISAMPLE_2_SAMPLES);
	pEngine->SetAngleSystem(cTV_ANGLE_DEGREE);
	pEngine->DisplayFPS(true);

	pGlobals = new CTVGlobals();
	pMathLibrary = new CTVMathLibrary();
	pInput = new CTVInputEngine();
	pInput->Initialize(true, true);
// ---------- END BASIC TV3D ENGINE INITIALIZATION ----------

// ---------- SCENE INITIALIZATION ---------- 
	pScene = new CTVScene();
	pScene->SetBackgroundColor( 0.0f, 0.01f, 0.02f);
	pScene->SetTextureFilter(cTV_FILTER_BILINEAR);
	pScene->SetViewFrustum(90.0f, 10000.0f);
// ---------- 

// ---------- MATERIALS ---------- 
	pMatFactory = new CTVMaterialFactory();
	tree_material = pMatFactory->CreateMaterialQuick(1, 1, 1, 1, "tree_material");
	pMatFactory->CreateMaterial("tree_material");
	pMatFactory->SetAmbient(tree_material, 0.4, 0.4, 0.4, 1.0);
	pMatFactory->SetDiffuse(tree_material, 0.95, 0.95, 0.95, 1.0);
	pMatFactory->SetEmissive(tree_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetSpecular(tree_material, 0.3, 0.3, 0.3, 1.0);
	pMatFactory->SetPower(tree_material, 64.0f);

// we need black material to block glowing effects
	black_material = pMatFactory->CreateMaterialQuick(1, 1, 1, 1, "black_material");
	pMatFactory->CreateMaterial("black_material");
	pMatFactory->SetAmbient(black_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetDiffuse(black_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetEmissive(black_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetSpecular(black_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetPower(black_material, 64.0f);

	glow_material = pMatFactory->CreateMaterialQuick(1, 1, 1, 1, "glow_material");
	pMatFactory->CreateMaterial("glow_material");
	pMatFactory->SetAmbient(glow_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetDiffuse(glow_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetEmissive(glow_material, 0.5, 0.5, 0.5, 1.0);
	pMatFactory->SetSpecular(glow_material, 0.0, 0.0, 0.0, 1.0);
	pMatFactory->SetPower(glow_material, 8.0f);

	normal_material = pMatFactory->CreateMaterialQuick(1, 1, 1, 1, "normal_material");
	pMatFactory->CreateMaterial("normal_material");
	pMatFactory->SetAmbient(normal_material, 1.0, 1.0, 1.0, 1.0);
	pMatFactory->SetDiffuse(normal_material, 1.0, 1.0, 1.0, 1.0);
	pMatFactory->SetEmissive(normal_material, 1.0, 1.0, 1.0, 1.0);
	pMatFactory->SetSpecular(normal_material, 1.0, 1.0, 1.0, 1.0);
	pMatFactory->SetPower(normal_material, 64.0f);
// ---------- END MATERIALS ---------- 

// ---------- FILE LOADING (Textures) ---------- 
	pTexFactory = new CTVTextureFactory();
	pTexFactory->LoadTexture( "..\\Media\\terrain4_TX.png", "LandTexture", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\blue.jpg", "PlayerTexture", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\sun.jpg", "Sun", -1, -1, cTV_COLORKEY_NO, true);

	pTexFactory->LoadTexture( "..\\Media\\Sky\\Sunset\\left.jpg", "skybox_left", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\Sky\\Sunset\\right.jpg", "skybox_right", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\Sky\\Sunset\\front.jpg", "skybox_front", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\Sky\\Sunset\\back.jpg", "skybox_back", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\Sky\\Sunset\\up.jpg", "skybox_top", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\Sky\\Sunset\\down.jpg", "skybox_bottom", -1, -1, cTV_COLORKEY_NO, true);

	pTexFactory->LoadTexture( "..\\Media\\flare0.jpg", "flare0", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\flare1.jpg", "flare1", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\flare2.jpg", "flare2", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\flare3.jpg", "flare3", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\clouds.dds", "Clouds", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\water.bmp", "Water", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\marble.bmp", "Marble", -1, -1, cTV_COLORKEY_NO, true);

	pTexFactory->LoadTexture( "..\\Media\\Detail3.bmp", "DetailTexture", -1, -1, cTV_COLORKEY_NO, true);

	pTexFactory->LoadTexture( "..\\Media\\billboard_tree_07.png", "TreeTexture", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true);
	pTexFactory->LoadBumpTexture("..\\Media\\tmp_trunk_skin_20_BUMP.png", "TreeTextureBump", -1, -1, true, 1.0f);

// ---------- LAND INITIALIZATION ---------- 
	pLand = pScene->CreateLandscape("Landscape");
	pLand->GenerateTerrain("..\\Media\\terrain4_hifield.png", cTV_PRECISION_HIGH, 16, 16, 0.0f, 0.0f, 0.0f, true);
	pLand->SetTexture(pGlobals->GetTex("LandTexture"), -1);
	pLand->SetTextureScale(1,1);
	pLand->ExpandTexture(pGlobals->GetTex("LandTexture"), 0, 0, 16, 16);

	pLand->EnableLOD(true, 512.0f, cTV_PRECISION_HIGH, 256.0f, false);
	pLand->SetProgressiveLOD(true);
//	pLand->SetDetailTexture(pGlobals->GetTex("DetailTexture"));
//	pLand->SetDetailTextureScale(15.0f, 15.0f);

// ---------- END LAND INITIALIZATION ---------- 



// ---------- ATMOSPHERE ---------- 
	pAtmos = new CTVAtmosphere();
	pAtmos->SkyBox_Enable(true);
	pAtmos->SkyBox_SetTexture(pGlobals->GetTex("skybox_front"), pGlobals->GetTex("skybox_back"), pGlobals->GetTex("skybox_left"), pGlobals->GetTex("skybox_right"), pGlobals->GetTex("skybox_top"), pGlobals->GetTex("skybox_bottom"));
	pAtmos->SkyBox_SetScale(2.0f, 2.0f, 2.0f);

/*  pAtmos->Fog_Enable(true);
	pAtmos->Fog_SetColor(0.1f, 0.5f, 0.1f);
	pAtmos->Fog_SetType(cTV_FOG_EXP, cTV_FOGTYPE_RANGE);*/

	pAtmos->Sun_Enable(true);
	pAtmos->Sun_SetTexture(pGlobals->GetTex("Sun"));
	pAtmos->Sun_SetColor(1.0f, 0.8f, 0.8f, 1.0f);
	pAtmos->Sun_SetPosition(1000.0f, 2000.0f, 1000.0f);

	pAtmos->LensFlare_Enable(true);
	pAtmos->LensFlare_SetLensNumber(3);

//  pAtmos->LensFlare_SetLensParams(1, pGlobals->GetTex("flare0"), 2 * 5, 40, RGBA(1, 1, 1, 0.5f), RGBA(1, 1, 1, 0.5f));
	pAtmos->LensFlare_SetLensParams(1, pGlobals->GetTex("flare1"), 1, 18, RGBA(1, 1, 1, 0.2f), RGBA(1, 1, 1, 0.5f));
	pAtmos->LensFlare_SetLensParams(2, pGlobals->GetTex("flare2"), 1.8, 15, RGBA(1, 1, 1, 0.2f), RGBA(0.7f, 1, 1, 0.5f));
	pAtmos->LensFlare_SetLensParams(3, pGlobals->GetTex("flare3"), 1, 6, RGBA(1, 0.1f, 0, 0.2f), RGBA(0.5f, 1, 1, 0.5f));

    pAtmos->Rain_Enable(false);
   // pAtmos->Rain_Init(10.0f, pGlobals->GetTex("flare0"));

	pAtmos->Clouds_Create(1, 8000.0f, 4000.0f);
	pAtmos->Clouds_SetLayerParameters(0, 1000.0f, pGlobals->GetTex("Clouds"));
	pAtmos->Clouds_SetLayerAnimation(0, true, 0.02f, 0.02f);
	pAtmos->Clouds_SetLayerBlendMode(0);
	pAtmos->Clouds_Enable(true);
// ---------- END ATMOSPHERE ---------- 


// ---------- ACTOR INITIALIZATION ----------
	CTVActor* my_actor = pScene->CreateActor("Actor");
	my_actor->Load("..\\Media\\Character.X");
	my_actor->SetTexture( pGlobals->GetTex("PlayerTexture"), -1);
	my_actor->SetAnimationID(0);
	my_actor->PlayAnimation(0.002f);
	my_actor->SetAnimationLoop(true);
	my_actor->SetScale(0.3f,0.3f,0.3f);
	
	pActor[0] = new MyActor(my_actor, pLand, 10.0f, 10.0f);
	for(int i = 1; i < MAX_ACTORS; i++)
	{
		std::ostringstream os;
		os << "Companion " << i;
		pActor[i] = new MyActor(my_actor->Duplicate(os.str().c_str()), pLand, (float)(rand()%4096), (float)(rand()%4096));
	}
// ---------- END ACTOR INITIALIZATION ----------

// ---------- OBJECT INITIALIZATION ----------
//	TheTree = new CTVMesh*[TREE_QUANTITY];
	TheRealTree = new CTVMesh*[TREE_QUANTITY];
	TheRealTree[0] = pScene->CreateMeshBuilder();
	TheRealTree[0]->SetTextureEx(cTV_LAYER_BUMPMAP, pGlobals->GetTex("TreeTextureBump"));
	TheRealTree[0]->SetBlendingMode(cTV_BLEND_ALPHA);
	TheRealTree[0]->SetCollisionEnable(true);
	TheRealTree[0]->SetAlphaTest(true, 128, true);
	TheRealTree[0]->LoadXFile("..\\Media\\tree_07.x", true, true);
	TheRealTree[0]->SetLightingMode(cTV_LIGHTING_MANAGED);
	TheRealTree[0]->SetScale(2.5f, 2.5f, 2.5f);

	for(int i = 0; i<TREE_QUANTITY; i++)
	{
		float x = (float)(rand()%4096);
		float z = (float)(rand()%4096);
		float y = pLand->GetHeight(x,z);

/*		TheTree[i] = pScene->CreateBillboard(pGlobals->GetTex("TreeTexture"), (float)x, (float)y, (float)z, (float)320.0,((float)320.0));
		TheTree[i]->SetBlendingMode(cTV_BLEND_ALPHA);
		TheTree[i]->SetAlphaTest(true, 128, true);
		TheTree[i]->SetBillboardType(cTV_BILLBOARD_YAXIS);*/

		if(i > 0)
			TheRealTree[i] = TheRealTree[0]->Duplicate();
		TheRealTree[i]->SetPosition((float)x, (float)y, (float)z);
	}
// ---------- END OBJECT INITIALIZATION ----------

// ----------- MISC STUFF -----------
	pEffect_background = new CTVGraphicEffect();
	pRenderSurface_background = pScene->CreateRenderSurface(256, 256, true);
	pEffect_background->InitGlowEffect(pRenderSurface_background);

	pScreen2DText = new CTVScreen2DText();
	my_font = pScreen2DText->TextureFont_Create("Arial10pts", "Arial", 10, false, false, false, false);
	pCamera = new MyCamera();
// ----------- END MISC STUFF -----------


// ---------- CEGUI INITIALIZATION ----------
	TVInternal = new CTVInternalObjects;
	d3Device = TVInternal->GetDevice3D();
	mGUIRenderer = new CEGUI::DirectX9Renderer(d3Device,0);
	mGUISystem = new CEGUI::System(mGUIRenderer);
	CEGUI::Logger::getSingleton().setLogFilename("..\\cegui.log", true);
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Insane);

	// initialise the required dirs for the DefaultResourceProvider
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
#if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
	rp->setResourceGroupDirectory("schemas", "../../XMLRefSchema/");
#endif

	// set the default resource groups to be used
	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
#ifdef CEGUI_WITH_XERCES
	CEGUI::XercesParser::setSchemaDefaultResourceGroup("schemas");
#endif

	CEGUI::Imageset* taharezImages = CEGUI::ImagesetManager::getSingleton().createImageset("TaharezLook.imageset");
	CEGUI::System::getSingleton().setDefaultMouseCursor(&taharezImages->getImage("MouseArrow"));
	CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font");

	CEGUI::WidgetLookManager::getSingleton().parseLookNFeelSpecification("TaharezLook.looknfeel");
	CEGUI::SchemeManager::getSingleton().loadScheme("/TaharezLookWidgets.scheme");

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::DefaultWindow* root = (CEGUI::DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root");
	CEGUI::System::getSingleton().setGUISheet(root);
// ---------- END CEGUI INITIALIZATION ----------

// ---------- CEGUI OBJECT DEFINITIONS ----------
	wnd = (CEGUI::FrameWindow*)winMgr.createWindow("TaharezLook/FrameWindow", "Demo Window");

	root->addChildWindow(wnd);
	wnd->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	wnd->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.3f)));
	wnd->setMaxSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	wnd->setMinSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));
	wnd->setText("Character statistics");
	wnd->setVisible(false);

    text = winMgr.createWindow("TaharezLook/StaticText", "Speed");
    wnd->addChildWindow(text);
    text->setProperty("FrameEnabled", "false");
    text->setProperty("BackgroundEnabled", "false");
	text->setPosition(CEGUI::UVector2(cegui_reldim(0.02f), cegui_reldim( 0.2f)));
	text->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 0.5f)));
    text->setText("Current Speed:");

//	stamina_bar = winMgr.createWindow("", "Stamina");
// ---------- END CEGUI OBJECT DEFINITION ----------
	//Main loop
	MSG msg;
	while( msg.message!=WM_QUIT) {
		if(PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else {
			render();
			input();
		}
	}

	unload();

	//Exit sucessfully
	return 0;
}


//Render sub
void render() {
// ---------- SET SPECIAL MATERIALS FOR GLOW EFFECT ----------
	pLand->SetMaterial(glow_material);
	for(int i = 0; i < TREE_QUANTITY; i++)
		TheRealTree[i]->SetMaterial(glow_material);
	for(int i = 0; i < MAX_ACTORS; i++)
		pActor[i]->pActor->SetMaterial(black_material);
// ---------- END SET SPECIAL MATERIALS FOR GLOW EFFECT ----------
	
// ---------- GLOW EFFECT RENDERING ----------
	pRenderSurface_background->StartRender(true);
	pAtmos->Atmosphere_Render();
	pLand->Render();
	pScene->RenderAllMeshes(false);
	for(int i = 0; i < MAX_ACTORS; i++)
		pActor[i]->Render();
	pAtmos->Clouds_Render();
	pRenderSurface_background->EndRender();
	pEffect_background->UpdateGlow();
// ---------- END GLOW EFFECT RENDERING ----------

// ---------- RESET MATERIALS ---------- 
	pLand->SetMaterial(normal_material);
	for(int i = 0; i < TREE_QUANTITY; i++)
		TheRealTree[i]->SetMaterial(tree_material);
	for(int i = 0; i < MAX_ACTORS; i++)
		pActor[i]->pActor->SetMaterial(normal_material);
// ---------- END RESET MATERIALS ---------- 


// ---------- MAIN RENDERING ----------
// clear the screen
	pEngine->Clear(false);
// draw the background
	pAtmos->Atmosphere_Render();
// draw the land
	pLand->Render();
// draw the trees
	pScene->RenderAllMeshes(false);
// draw the actors
	for(int i = 0; i < MAX_ACTORS; i++)
		pActor[i]->Render();
// draw the clouds
	pAtmos->Clouds_Render();
//	pAtmos->Rain_Render();
// put the glow effect on the rendered scene
	pEffect_background->DrawGlow();
// ---------- END MAIN RENDERING ----------

// ---------- PRINT ACTOR NAMES ----------
	pScreen2DText->Action_BeginText();
	for(int i = 0; i < MAX_ACTORS; i++)
	{
		std::ostringstream os;
		if(i != 0)
			os << "actor " << i;
		else os << "player";
		float x;
		float y;
		cTV_3DVECTOR point = pActor[i]->getHeadPosition();
		pMathLibrary->Project3DPointTo2D(&point, &x, &y, false);
		pScreen2DText->TextureFont_DrawText(os.str().c_str(), x, y, cTV_COLORKEY_WHITE, my_font);
	}
	pScreen2DText->Action_EndText();
// ---------- END PRINT ACTOR NAMES ----------

// ---------- MAIN RENDER FUNCTIONS ----------
	CEGUI::System::getSingleton().renderGUI();
	d3Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pEngine->RenderToScreen();
// ---------- END MAIN RENDER FUNCTIONS ----------
}

//Input sub
void input() {
	float fTimeElapsed = pEngine->TimeElapsed();
	CEGUI::System::getSingleton().injectTimePulse(fTimeElapsed);

// ---------- CHECK FOR KEY EVENTS ----------
	cTV_KEYDATA* buffer = new cTV_KEYDATA[100];
	int buffer_size;
	pInput->GetKeyBuffer(buffer, &buffer_size);
	{
		for(int i = 0; i < buffer_size; i++)
		{
// show / hide character menu
			if(buffer[i].Key == cTV_KEY_C && buffer[i].Pressed) {
				if(wnd->isVisible()) {
					wnd->hide();
					menuShown = false;
				}
				else 
				{
					wnd->show();
					menuShown = true;
				}
			} else
// zoom
			if(buffer[i].Key == cTV_KEY_9 && buffer[i].Pressed) {
				pCamera->doZoom(1);
			} else
			if(buffer[i].Key == cTV_KEY_0 && buffer[i].Pressed) {
				pCamera->doZoom(-1);
			} else 
			
		//	if(buffer[i].Key == cTV_KEY_RETURN && buffer[i].Pressed && pInput->IsKeyPressed(cTV_KEY_ALT_LEFT))
		//	{
			// not working
		//		pEngine->SwitchFullscreen(800, 600, 32, 
		//	}

// escape -> close menu or quit if it's already closed
			if(buffer[i].Key == cTV_KEY_ESCAPE && buffer[i].Pressed) {
				if(menuShown)
				{
					wnd->hide();
					menuShown = false;
				}
				else
					PostQuitMessage(0);
			}
		}
	}
	delete buffer;
// ---------- END CHECK FOR KEY EVENTS ----------

// ---------- PROCESS MOUSE EVENTS AND KEY MOVEMENT ----------
	{
		int retX, retY, retRoll;
		bool retButton1,retButton2,retButton3,retButton4;
		pInput->GetMouseState(&retX, &retY, &retButton1, &retButton2, &retButton3, &retButton4, &retRoll);
		if(!menuShown)
		{
			pCamera->doZoom(retRoll);
			pCamera->updateMouse(retX, retY);
	//Check if we pressed the UP arrow key, if so, then we are
	//walking forward.
		} else
		{
			CEGUI::System::getSingleton().injectMouseMove(retX, retY);
			if(retButton1!=0 && oldRetButton1==0) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
			if(retButton2!=0 && oldRetButton2==0) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
			if(retButton3!=0 && oldRetButton3==0) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
			if(retButton1==0 && oldRetButton1!=0) CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
			if(retButton2==0 && oldRetButton2!=0) CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
			if(retButton3==0 && oldRetButton3!=0) CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::RightButton);
			oldRetButton1 = retButton1;
			oldRetButton2 = retButton2;
			oldRetButton3 = retButton3;
		}
		if(pInput->IsKeyPressed(cTV_KEY_UP) || pInput->IsKeyPressed(cTV_KEY_W)) {
			if(pInput->IsKeyPressed(cTV_KEY_LEFTSHIFT))
			{
				pActor[0]->doRun(fTimeElapsed);
			} else
			{
				pActor[0]->doWalk(fTimeElapsed);
			}	
		}
		if(pInput->IsKeyPressed(cTV_KEY_DOWN) || pInput->IsKeyPressed(cTV_KEY_S))
			pActor[0]->doBrake(fTimeElapsed);
		if(pInput->IsKeyPressed(cTV_KEY_LEFT) || pInput->IsKeyPressed(cTV_KEY_A))
			pActor[0]->doStrafeRight(fTimeElapsed);
		if(pInput->IsKeyPressed(cTV_KEY_RIGHT) || pInput->IsKeyPressed(cTV_KEY_D))
			pActor[0]->doStrafeLeft(fTimeElapsed);
	}
// ---------- PROCESS MOUSE EVENTS AND KEY MOVEMENT ----------

	for(int i = 0; i < MAX_ACTORS; i++)
		pActor[i]->determineStatus();

// ---------- UPDATE CHARACTER SCREEN ----------
	text->setText(pActor[0]->getStatusString());
// ---------- END UPDATE CHARACTER SCREEN ----------

	pActor[0]->rotate(pCamera->getDirection()->y);
	for(int i = 1; i < MAX_ACTORS; i++)
		pActor[i]->follow(pActor[0]->getPosition());
	for(int i = 0; i < MAX_ACTORS; i++)
	{
		pActor[i]->move(pLand, fTimeElapsed);
		pActor[i]->checkAnimation();
	}

/*	for(int i = 1; i < MAX_ACTORS; i++)
		pActor[i]->doAIMovement();*/
// ---------- UPDATE CAMERA ----------
	pCamera->updateViewVector(pActor[0]->getHeadPosition(), pLand);
	pScene->SetCamera(pCamera->getViewVector()->x, pCamera->getViewVector()->y, pCamera->getViewVector()->z, 
		pActor[0]->getHeadPosition().x, pActor[0]->getHeadPosition().y, pActor[0]->getHeadPosition().z);
// ---------- END UPDATE CAMERA ----------
}


//Unload sub
void unload() {
	//Uninitialize the variables
	pScene->DestroyAllMeshes();
	for(int i = 0; i < MAX_ACTORS; i++)
		delete pActor[i];

	delete pAtmos;
	delete pLand;
	delete pEffect_background;
	delete pRenderSurface_background;
	delete pScene;
//	for(int i = 0; i < TREE_QUANTITY; i++)
//		delete TheTree[i];
//	delete TheTree;
	for(int i = 0; i < TREE_QUANTITY; i++)
		delete TheRealTree[i];
	delete TheRealTree;
	pScreen2DText->Font_DeleteAll();
	delete pScreen2DText;
	delete pInput;
	delete pGlobals;
	delete TVInternal;
	delete pTexFactory;
	delete pMatFactory;
	delete pMathLibrary;
	delete pEngine;
	delete pCamera;
	//delete CEGUI::System::getSingletonPtr();
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
