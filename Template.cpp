#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include "CTVEngine.h"
#include "CTVScene.h"
#include "CTVInputEngine.h"
#include "CTVTextureFactory.h"
#include "CTVAtmosphere.h"
#include "CTVGlobals.h"
#include "CTVMesh.h"
#include "CTVLightEngine.h"
#include "tv_types.h"
#include "HelperFunctions.h"
#include "CTVGraphicEffect.h"
#include "CTVRenderSurface.h"
//#include "CTVInternalObjects.h"

CTVTextureFactory* pTexFactory;
CTVLandscape* pLand;
CTVAtmosphere* pAtmos;
CTVActor* pPlayerActor;
//CTVMesh* pFloorMesh;
//CTVLightEngine* pLightEngine;

//CTVMesh* pWater;
CTVMesh** TheTree;


float fTimeElapsed = 0.0;

float sngPositionX = 0.0;
float sngPositionY = 0.0;
float sngPositionZ = 0.0;
float sngAngleX = 0.0;
float sngAngleY = 0.0;

//We could have done this in many ways, but we added some smoothing to
//the movement se we need to declare two additional variables.
float sngWalk = 0.0;
float sngStrafe = 0.0;
float sngBrake = 0.0;

int tree_quantity = 200;
    
//Declare a water height variable
float sngWaterHeight = 0.0;

cTV_3DVECTOR* PlayerPosition;
cTV_3DVECTOR* PlayerDirection;
cTV_3DVECTOR* LightPosition;
cTV_3DVECTOR* LightDirection;
cTV_3DVECTOR* MeshPosition;

CTVRenderSurface* RSReflection;
CTVRenderSurface* RSRefraction;

CTVGraphicEffect* pEffect;


//Setup TrueVision3D
CTVEngine* pEngine;
CTVInputEngine* pInput;
CTVScene* pScene;
    CTVGlobals* pGlobals;

void render();
void input();
void unload();

LRESULT CALLBACK WndProc(HWND wpHWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//WinMain: The entry point to everything Windows
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND WindowHandle;
    
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
    GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
    "TV3D Tutorial", NULL };
    RegisterClassEx(&wc);
    
    WindowHandle = CreateWindow( "TV3D Tutorial", "S-WORDS",
    WS_OVERLAPPEDWINDOW, 100, 100, 640, 480,
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
    
    //Create the TVEngine
    pEngine = new CTVEngine();
    pEngine->SetDebugFile("c:\\debug.txt");
    
    pEngine->Init3DWindowed(WindowHandle, true);
    //pEngine->Init3DFullscreen (1400,1050);
    
    pEngine->SetAngleSystem(cTV_ANGLE_DEGREE);
    pEngine->DisplayFPS(true);
    
    pGlobals = new CTVGlobals();
    
    // Scene
    pScene = new CTVScene();
    pScene->SetBackgroundColor( 0.0f, 0.01f, 0.02f);
    pScene->SetTextureFilter(cTV_FILTER_BILINEAR);
    
    pEffect = new CTVGraphicEffect();
    
    // loading textures
    pTexFactory = new CTVTextureFactory();
    pTexFactory->LoadTexture("C:\\Media\\dirtandgrass.jpg", "LandTexture", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\blue.jpg", "PlayerTexture", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\sun.jpg", "Sun", -1, -1, cTV_COLORKEY_NO, true);
    
    pTexFactory->LoadTexture( "C:\\Media\\Sky\\Winter\\left.jpg", "skybox_left", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\Sky\\Winter\\right.jpg", "skybox_right", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\Sky\\Winter\\front.jpg", "skybox_front", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\Sky\\Winter\\back.jpg", "skybox_back", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\Sky\\Winter\\up.jpg", "skybox_top", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\Sky\\Winter\\down.jpg", "skybox_bottom", -1, -1, cTV_COLORKEY_NO, true);
    
    pTexFactory->LoadTexture( "C:\\Media\\flare0.jpg", "flare0", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\flare1.jpg", "flare1", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\flare2.jpg", "flare2", -1, -1, cTV_COLORKEY_NO, true);
    pTexFactory->LoadTexture( "C:\\Media\\flare3.jpg", "flare3", -1, -1, cTV_COLORKEY_NO, true);
    
    pTexFactory->LoadTexture( "C:\\Media\\clouds.dds", "Clouds", -1, -1, cTV_COLORKEY_NO, true);
    
    pTexFactory->LoadTexture( "C:\\Media\\water.bmp", "Water", -1, -1, cTV_COLORKEY_NO, true);
    
    pTexFactory->LoadTexture( "C:\\Media\\Tree.dds", "TreeTexture", -1, -1, cTV_COLORKEY_NO, true);
    
    // Land mass
    pLand = pScene->CreateLandscape("Landscape");
    pLand->GenerateTerrain("C:\\Media\\heightmap.jpg", cTV_PRECISION_VERY_LOW, 32, 32, 0.0f, 0.0f, 0.0f);
    pScene->SetViewFrustum(60, 45000);
    pLand->SetTexture(pGlobals->GetTex("LandTexture"), -1);

  
    /*pWater = pScene->CreateMeshBuilder("Watermesh");
    pWater->AddFloor(pGlobals->GetTex("Water"), -256, -256, 256, 256, 25, 2, 2, false);
    pWater->SetScale(8,1,8);
    
    RSReflection = pScene->CreateRenderSurface(1024, 1024, true, cTV_TEXTUREFORMAT_A8R8G8B8);
    RSRefraction = pScene->CreateRenderSurface(1024, 1024, true, cTV_TEXTUREFORMAT_A8R8G8B8);
    
    cTV_PLANE pWaterPlane;
    pWaterPlane.Dist = 25.0f;
    pWaterPlane.Normal.x = 0.0f;
    pWaterPlane.Normal.y = -1.0f;
    pWaterPlane.Normal.z = 0.0f;
    
    pEffect->SetWaterReflection(pWater, RSReflection, RSRefraction, 1, &pWaterPlane);
*/
    // Atmosphere
    pAtmos = new CTVAtmosphere();
    pAtmos->SkyBox_Enable(true);
    pAtmos->SkyBox_SetTexture(pGlobals->GetTex("skybox_front"), 
    pGlobals->GetTex("skybox_back"), 
    pGlobals->GetTex("skybox_left"), 
    pGlobals->GetTex("skybox_right"), 
    pGlobals->GetTex("skybox_top"), 
    pGlobals->GetTex("skybox_bottom"));
    
/*    pAtmos->Fog_Enable(true);
    pAtmos->Fog_SetColor(0.1f, 0.5f, 0.1f);
    pAtmos->Fog_SetType(cTV_FOG_EXP, cTV_FOGTYPE_RANGE);*/
    
    pAtmos->Sun_Enable(true);
    pAtmos->Sun_SetTexture(pGlobals->GetTex("Sun"));
    pAtmos->Sun_SetColor(1.0f, 0.8f, 0.8f, 1.0f);
    pAtmos->Sun_SetPosition(1000.0f, 2000.0f, 1000.0f);
    
//    pAtmos->Clouds_Create
    // always above water
      pAtmos->LensFlare_Enable(true);
      pAtmos->LensFlare_SetLensNumber(3);
      
//        pAtmos->LensFlare_SetLensParams(1, pGlobals->GetTex("flare0"), 2 * 5, 40, RGBA(1, 1, 1, 0.5f), RGBA(1, 1, 1, 0.5f));
        pAtmos->LensFlare_SetLensParams(1, pGlobals->GetTex("flare1"), 2 * 1, 18, RGBA(1, 1, 1, 0.5f), RGBA(1, 1, 1, 0.5f));
        pAtmos->LensFlare_SetLensParams(2, pGlobals->GetTex("flare2"), 2 * 1.8, 15, RGBA(1, 1, 1, 0.5f), RGBA(0.7f, 1, 1, 0.5f));
        pAtmos->LensFlare_SetLensParams(3, pGlobals->GetTex("flare3"), 2 * 1, 6, RGBA(1, 0.1f, 0, 0.5f), RGBA(0.5f, 1, 1, 0.5f));
        
//    pAtmos->Rain_Enable(true);
//    pAtmos->Rain_Init(0.1f, int iRainTexture, float fFallSpeed = 1.0f, float fWindX = 0.0f, float fWindZ = 0.0f, float fRandomValue = 0.1f, float fDistRain = 20.0f, float fRainParticleSize = 1.0f, float fRainRadius = 20.0f, int iRainGeneratorSpeed = 20, float fRainSpeed = 1.0f);

        pAtmos->Clouds_Create(1, 4000.0f, 4000.0f);
//    pAtmos->Clouds_SetLayerParameters(1, 1000.0f, pGlobals->GetTex("Clouds"));
    pAtmos->Clouds_SetLayerParameters(1, 2000.0f, pGlobals->GetTex("Clouds"));
 //   pAtmos->Clouds_SetLayerParameters(3, 3000.0f, pGlobals->GetTex("Clouds"));
    pAtmos->Clouds_SetLayerAnimation(1, true, 0.02f, 0.02f);
//    pAtmos->Clouds_SetLayerAnimation(2, true, 0.01f, 0.02f);
//    pAtmos->Clouds_SetLayerAnimation(3, true, 0.02f, 0.01f);
    pAtmos->Clouds_Enable(true);
	    
    
    // creating actor
    pPlayerActor = pScene->CreateActor("Actor");
    // loading models
    pPlayerActor->Load("C:\\Media\\Character.X");

    pPlayerActor->SetTexture( pGlobals->GetTex("PlayerTexture"), -1);
    pPlayerActor->SetMaterial(0);
    pPlayerActor->PlayAnimation(20);
    pPlayerActor->SetScale(0.2f,0.2f,0.2f);
    
    PlayerPosition = new cTV_3DVECTOR;
    PlayerPosition->x = 150.0f;
    PlayerPosition->z = 150.0f;
    PlayerPosition->y = pLand->GetHeight(PlayerPosition->x, PlayerPosition->z);
    pPlayerActor->SetPosition(PlayerPosition->x, PlayerPosition->y, PlayerPosition->z);
 /*
    pFloorMesh = pScene->CreateMeshBuilder();
    pFloorMesh->LoadXFile("C:\\Media\\brige.x");
    pFloorMesh->SetMaterial(0);
    pFloorMesh->SetCullMode(cTV_FRONT_CULL);
    
    MeshPosition = new cTV_3DVECTOR(100.0f, pLand->GetHeight(MeshPosition->x, MeshPosition->z),100.0f);
    pFloorMesh->SetPosition(MeshPosition->x, MeshPosition->y, MeshPosition->z);
*/
   // pLightEngine = new CTVLightEngine();
/*	cTV_LIGHT* TestLight = new cTV_LIGHT;
	memset(TestLight, 0, sizeof(cTV_LIGHT));

        LightPosition = new cTV_3DVECTOR(PlayerPosition->x, PlayerPosition->y, PlayerPosition->z);
        LightDirection = new cTV_3DVECTOR(0.5f,0.5f,0.5f); // TODO
        
	// Set the light properties.
	// Be sure to use LightColor macro and not DXColor macro.
	TestLight->ambient = cTV_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	TestLight->diffuse = cTV_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	TestLight->type = cTV_LIGHT_DIRECTIONAL;
	TestLight->direction = *LightDirection;
	pLightEngine->CreateLight(TestLight, "Test Light");*/
    
    
    
    TheTree = new CTVMesh*[tree_quantity];
    for(int i = 0; i<tree_quantity; i++)
    {
        int x = rand()%1024;
        int y = rand()%1024;
        float z = pLand->GetHeight(x,y);
        TheTree[i] = pScene->CreateBillboard(pGlobals->GetTex("TreeTexture"), x, y+64, z,64,-64);
        TheTree[i]->SetBillboardType(cTV_BILLBOARD_YAXIS);
        TheTree[i]->SetAlphaTest(true,32);
    }
    
    
    //We set the initial values of movement
    sngWalk = 0;
    sngStrafe = 0;
    
    // setting camera
    //Something else new : we set the camera vectors which are the
    //position of the camera (the point of view) and where we are
    //looking at. We start by centering the camera position at the
    //vector 0,20,0 and the look at the vector 50,20,50
    sngPositionX = 0;
    sngPositionY = 20;
    sngPositionZ = 0;
    sngAngleX = 0;
    sngAngleY = 0;
    
    //Init input after main init
    pInput = new CTVInputEngine();
    pInput->Initialize(true, true);

    
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
    pEngine->Clear(false);						//Clear the screen
    pLand->Render();
//    pWater->Render();
//    pFloorMesh->Render();
    pScene->RenderAllMeshes();
    pPlayerActor->Render();
    pAtmos->Atmosphere_Render();
    
//    CEGUI::System::getSingleton().renderGUI();

    
    pEngine->RenderToScreen();					//Render the screen
}

//Input sub
void input() {
    //Check if we pressed the UP arrow key, if so, then we are
    //walking forward.
    if(pInput->IsKeyPressed(cTV_KEY_UP)) {
        sngWalk = 1;
    }
    
    //Check if we pressed the DOWN arrow key, if so, then we are
    //pressing the breaks.
    if(pInput->IsKeyPressed(cTV_KEY_DOWN)) {
        sngBrake = 0.002f;
    }
    else {
        //We are not pressing the brakes, let the tank float.
        if(sngWalk > 0.0f)
            sngBrake = 0.0005f;
        else if(sngWalk < 0.0f)
            sngBrake = -0.0005f;
    }
    
    //Check if we pressed the LEFT arrow key, if so, then strafe
    //on the left.
    if(pInput->IsKeyPressed(cTV_KEY_LEFT)) {
        sngStrafe = 0.5f;
    }
    //If we are not strafing left, maybe we want to strafe to the
    //right, using the RIGHT arrow? If so, set strafe to negative.
    if(pInput->IsKeyPressed(cTV_KEY_RIGHT)) {
        sngStrafe = -0.5f;
    }
    
    //Now, for the mouse input...
    int tmpMouseX = 0; int tmpMouseY = 0;
    bool tmpMouseB1 = false; bool tmpMouseB2 = false; bool tmpMouseB3 = false; bool tmpMouseB4 = false;
    int tmpMouseScrollOld = 0; int tmpMouseScrollNew = 0;
    
    //Get the movement of the mouse.
    pInput->GetMouseState(&tmpMouseX, &tmpMouseY, &tmpMouseB1, &tmpMouseB2, &tmpMouseB3, &tmpMouseB4, &tmpMouseScrollNew);
    
    //Update the tank angle.
    sngAngleY -= tmpMouseX / 500.0f;
    sngAngleX -= tmpMouseY / 500.0f;
    
    //Okay, now for the smothing of the movement... Update
    //the forward and backward (walk) movement.
    
    if(sngAngleX > 1.3f)
        sngAngleX = 1.3f;
    if(sngAngleX < -1.3f)
        sngAngleX = -1.3f;
    
    fTimeElapsed = pEngine->TimeElapsed();
    bool not_moving = (sngWalk == 0.0f) && (sngStrafe == 0.0f);
    
    if(sngWalk > 0.0f)
    {
        sngWalk -= 0.005 * fTimeElapsed;
        if(sngWalk < 0.0f)
            sngWalk = 0.0f;
    }
    else if(sngWalk < 0.0f) {
        sngWalk += 0.005 * fTimeElapsed;
        if(sngWalk > 0.0f)
            sngWalk = 0.0f;
    }
    
    if(sngStrafe > 0.0f)
    {
        sngStrafe -= 0.01 * fTimeElapsed;
        if(sngStrafe < 0.0f)
            sngStrafe = 0.0f;
    } else if(sngStrafe < 0.0f) {
        sngStrafe += 0.01 * fTimeElapsed;
        if(sngStrafe > 0.0f)
            sngStrafe = 0.0f;
    }
    
    if((sngWalk == 0.0f) && (sngStrafe == 0.0f))
    {
        // just stopped
        if(!not_moving) {
            pPlayerActor->SetAnimationID(0);
            pPlayerActor->PlayAnimation(20);
        }
    } else 
    {
        // just started
        if(not_moving) {
            pPlayerActor->SetAnimationID(1);
            pPlayerActor->PlayAnimation(50);
        }
    }        
    
    //Update the vectors using the angles and positions. But this
    //time, we don't update the camera position but the tank
    //vector position.
    PlayerPosition->x += (cosf(sngAngleY) * sngWalk / 5.0f * fTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * sngStrafe / 5.0f * fTimeElapsed);
    PlayerPosition->z += (sinf(sngAngleY) * sngWalk / 5.0f * fTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * sngStrafe / 5.0f * fTimeElapsed);
    PlayerPosition->y = pLand->GetHeight(PlayerPosition->x, PlayerPosition->z) + 2.0f;
    
    //From the tank position vector, we update the mesh position.
    pPlayerActor->SetPosition( PlayerPosition->x, PlayerPosition->y, PlayerPosition->z);
    
    //From the angle variable, we update the tank rotation, and from
    //the strafe speed, we update the tank roll.
    pPlayerActor->SetRotation((sngStrafe * 10.0f), 90.0f + (sngAngleY * -57.295f), 0.0f);
    
    //With the new values of the tank vector, we update the camera
    //position and look at.
    float tmpLookAtX = 0.0f; float tmpLookAtZ = 0.0f; float tmpLookAtY = 0.0f;
    tmpLookAtX = PlayerPosition->x - (cosf(sngAngleY) * 125.0f);
    tmpLookAtZ = PlayerPosition->z - (sinf(sngAngleY) * 125.0f);
    tmpLookAtY = PlayerPosition->y - (tanf(sngAngleX) * 125.0f);
    
    //With the new values of the tank vector, we update the camera
    //position and look at.
    pScene->SetCamera(tmpLookAtX, tmpLookAtY, tmpLookAtZ, PlayerPosition->x, PlayerPosition->y, PlayerPosition->z);
    
    if(pInput->IsKeyPressed(cTV_KEY_ESCAPE))		//Check if ESCAPE has been pressed.
    {
        PostQuitMessage(0);
    }
}


//Unload sub
void unload() {
    //Uninitialize the variables
    pScene->DestroyAllMeshes();
    delete pPlayerActor;
    delete pAtmos;
    delete pLand;
//    delete pWater;
    delete pEffect;
    delete pScene;
//    delete pFloorMesh;
//    delete pLightEngine;
    for(int i = 0; i < tree_quantity; i++)
        delete TheTree[i];
    delete TheTree;
    delete pInput;
    delete pGlobals;
//    delete TVInternal;
    delete pEngine;
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
