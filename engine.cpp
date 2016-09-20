#include "engine.h"
#include "configuration.h"
#include <sstream>

Engine::Engine(HWND handle, int screen_x, int screen_y):
	oldRetButton1(false),
	oldRetButton2(false), 
	oldRetButton3(false), 
	oldRetButton4(false), 
	menuShown(false),
	arrowDirection(0.0f),
	windowHandle(handle)
{
	pCamera = new MyCamera();




// ---------- BASIC TV3D ENGINE INITIALIZATION ----------
//Create the TVEngine
	pEngine = new CTVEngine();
	pEngine->SetDebugFile("..\\debug.txt");

//	pEngine->Init3DWindowed(windowHandle, true);
	pEngine->Init3DFullscreen(configuration.getResolutionX(), configuration.getResolutionY(), configuration.getBitDepth(), true, false, configuration.getBufferFormat(), configuration.getGamma(), windowHandle);
	pEngine->SetAntialiasing(true, configuration.getAntiAliasing());
	pEngine->SetAngleSystem(cTV_ANGLE_DEGREE);
	pEngine->DisplayFPS(configuration.getIsShowFPS());

	pGlobals = new CTVGlobals();
	pMathLibrary = new CTVMathLibrary();
	pInput = new CTVInputEngine();
	pInput->Initialize(true, true);
// ---------- END BASIC TV3D ENGINE INITIALIZATION ----------

// ---------- SCENE INITIALIZATION ---------- 
	pScene = new CTVScene();
	pScene->SetBackgroundColor( 0.0f, 0.01f, 0.02f);
	pScene->SetTextureFilter(configuration.getTextureFilter());
	pScene->SetViewFrustum(configuration.getFieldOfView(), configuration.getFarplane());
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
	pTexFactory->LoadTexture( "..\\Media\\water.png", "Water", -1, -1, cTV_COLORKEY_NO, true);
	pTexFactory->LoadTexture( "..\\Media\\marble.bmp", "Marble", -1, -1, cTV_COLORKEY_NO, true);

	pTexFactory->LoadTexture( "..\\Media\\wall.bmp", "WallTexture", -1, -1, cTV_COLORKEY_NO, true);

	pTexFactory->LoadTexture( "..\\Media\\Detail3.bmp", "DetailTexture", -1, -1, cTV_COLORKEY_NO, true);

	pTexFactory->LoadTexture( "..\\Media\\billboard_tree_07.png", "TreeTexture", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true);
	pTexFactory->LoadBumpTexture("..\\Media\\tmp_trunk_skin_20_BUMP.png", "TreeTextureBump", -1, -1, true, 1.0f);

// ---------- LAND INITIALIZATION ---------- 
	pLand = pScene->CreateLandscape("Landscape");
	pLand->GenerateTerrain("..\\Media\\terrain4_hifield.png", configuration.getLandscapePrecision(), 16, 16, 0.0f, 0.0f, 0.0f, true);
	pLand->SetTexture(pGlobals->GetTex("LandTexture"), -1);
	pLand->SetTextureScale(1,1);
	pLand->ExpandTexture(pGlobals->GetTex("LandTexture"), 0, 0, 16, 16);

	pLand->EnableLOD(true, configuration.getLODSwitchDistance(), configuration.getLandscapePrecision(), configuration.getStartDistance(), false);
	pLand->SetProgressiveLOD(configuration.getIsProgressiveLOD());
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

	pAtmos->Sun_Enable(configuration.getIsUseSun());
	pAtmos->Sun_SetTexture(pGlobals->GetTex("Sun"));
	pAtmos->Sun_SetColor(1.0f, 0.8f, 0.8f, 1.0f);
	pAtmos->Sun_SetPosition(1000.0f, 2000.0f, 1000.0f);

	pAtmos->LensFlare_Enable(configuration.getIsUseLens());
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
	pAtmos->Clouds_Enable(configuration.getIsUseClouds());
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

// ---------- SAMPLE ROOM ----------
	Room = new CTVMesh();
	Room = pScene->CreateMeshBuilder();
	Room->AddWall(pGlobals->GetTex("WallTexture"), 500.0f, -500.0f, -500.0f, -500.0f, 1000.0f, 10.0f);
	Room->AddWall(pGlobals->GetTex("WallTexture"), -500.0f, -500.0f, -500.0f, 500.0f, 1000.0f, 10.0f);
	Room->AddWall(pGlobals->GetTex("WallTexture"), -500.0f, 500.0f, 500.0f, 500.0f, 1000.0f, 50.0f);
	Room->AddWall(pGlobals->GetTex("WallTexture"), 500.0f, 500.0f, 500.0f, -500.0f, 1000.0f, 50.0f);
	Room->AddFloor(pGlobals->GetTex("WallTexture"), -500.0f, -500.0f, 500.0f, 500.0f, 0.0f);
	Room->SetTexture(pGlobals->GetTex("WallTexture"));



// ---------- END SAMPLE ROOM ----------


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
// ----------- END MISC STUFF -----------

}

void Engine::initGUI(CharacterCreation* character_creation) {
	gui = new GUI(TVInternal->GetDevice3D(), character_creation);
}



//Render sub
void Engine::render() {
// ---------- SET SPECIAL MATERIALS FOR GLOW EFFECT ----------
	pLand->SetMaterial(glow_material);
	for(int i = 0; i < TREE_QUANTITY; i++)
		TheRealTree[i]->SetMaterial(glow_material);
	for(int i = 0; i < MAX_ACTORS; i++)
		pActor[i]->pActor->SetMaterial(black_material);
//	Room->SetMaterial(black_material);
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
//	Room->SetMaterial(normal_material);
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
		cTV_3DVECTOR point = pActor[i]->getHeadPosition();
		cTV_3DVECTOR direct_line = point - *pCamera->getPosition();
		if(abs(direct_line.Length()) < 500.0f)
		{
			std::ostringstream os;
			if(i != 0)
				os << "actor " << i;
			else os << "player";
			float x;
			float y;
			pMathLibrary->Project3DPointTo2D(&point, &x, &y, false);
			pScreen2DText->TextureFont_DrawText(os.str().c_str(), x, y, cTV_COLORKEY_WHITE, my_font);
		}
	}
	pScreen2DText->Action_EndText();
// ---------- END PRINT ACTOR NAMES ----------

// ---------- MAIN RENDER FUNCTIONS ----------
	gui->renderGUI();
	
	pEngine->RenderToScreen();
// ---------- END MAIN RENDER FUNCTIONS ----------
}

//Input sub
void Engine::input() {
	float fTimeElapsed = pEngine->TimeElapsed();
	gui->injectTimePulse(fTimeElapsed);

// ---------- CHECK FOR KEY EVENTS ----------
	cTV_KEYDATA* buffer = new cTV_KEYDATA[100];
	int buffer_size;
	pInput->GetKeyBuffer(buffer, &buffer_size);
	{
		for(int i = 0; i < buffer_size; i++)
		{
// show / hide character menu
			if(buffer[i].Key == cTV_KEY_C && buffer[i].Pressed) {
				gui->show();
			} else
// zoom
			if(buffer[i].Key == cTV_KEY_9 && buffer[i].Pressed) {
				pCamera->doZoom(1);
			} else
			if(buffer[i].Key == cTV_KEY_0 && buffer[i].Pressed) {
				pCamera->doZoom(-1);
			}
			
		//	if(buffer[i].Key == cTV_KEY_RETURN && buffer[i].Pressed && pInput->IsKeyPressed(cTV_KEY_ALT_LEFT))
		//	{
			// not working
		//		pEngine->SwitchFullscreen(800, 600, 32, 
		//	}

// escape -> close menu or quit if it's already closed
			if(buffer[i].Key == cTV_KEY_ESCAPE && buffer[i].Pressed && !menuShown) {
				PostQuitMessage(0);
			} else if(buffer[i].Pressed) // get it to cegui
			{
				gui->injectChar(buffer[i].Key);
			} else if(buffer[i].Released)
			{
			}
		}
	}
	delete buffer;

	menuShown = gui->isCharacterMenuShown();
// ---------- END CHECK FOR KEY EVENTS ----------

// ---------- PROCESS MOUSE EVENTS AND KEY MOVEMENT ----------
	{
		arrowDirection = 0.0f;
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
			gui->injectMouseMove(retX, retY);
			gui->injectMouseButton(retButton1, retButton2, retButton3, oldRetButton1, oldRetButton2, oldRetButton3);

			oldRetButton1 = retButton1;
			oldRetButton2 = retButton2;
			oldRetButton3 = retButton3;
		}

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		bool run = false;
		bool back = false;

		if(pInput->IsKeyPressed(cTV_KEY_UP) || pInput->IsKeyPressed(cTV_KEY_W))
			up = true;
		if(pInput->IsKeyPressed(cTV_KEY_DOWN) || pInput->IsKeyPressed(cTV_KEY_S))
			down = true;
		if(pInput->IsKeyPressed(cTV_KEY_LEFT) || pInput->IsKeyPressed(cTV_KEY_A))
			left = true;
		if(pInput->IsKeyPressed(cTV_KEY_RIGHT) || pInput->IsKeyPressed(cTV_KEY_D))
			right = true;
		if(pInput->IsKeyPressed(cTV_KEY_LEFTSHIFT))
			run = true;
		if(pInput->IsKeyPressed(cTV_KEY_ALT_LEFT))
			back = true;

		pActor[0]->translateMovementKeys(up, down, left, right, run, back, pCamera->getDirection()->y, fTimeElapsed);
		

//		pActor[0]->doBrake(fTimeElapsed);
		
/*
Mausbewegung aendert nicht Koerperhaltung (hoechstens Kopf / Oberkoerper!)
Wenn eine der 8 Richtungen durch Pfeiltasten vorgegeben, setze es als normalen forward & rotate um
*/
//			pActor[0]->doStrafeRight(fTimeElapsed);
		

/*			
//	pActor[0]->rotate(pCamera->getDirection()->y);


			pActor[0]->doStrafeLeft(fTimeElapsed);*/
	}
// ---------- PROCESS MOUSE EVENTS AND KEY MOVEMENT ----------

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		pActor[i]->determineStatus();
		pActor[i]->slowDown(fTimeElapsed);
	}

// ---------- UPDATE CHARACTER SCREEN ----------
//	text->setText(pActor[0]->getStatusString());
// ---------- END UPDATE CHARACTER SCREEN ----------
//	pActor[0]->rotate(pCamera->getDirection()->y);

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
Engine::~Engine() {
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
	delete Room;
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
	delete gui;
}

