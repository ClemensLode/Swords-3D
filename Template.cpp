// Template.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Template.h"

#include "CTVEngine.h"
#include "CTVScene.h"
#include "CTVInputEngine.h"
#include "tv_types.h"

#define MAX_LOADSTRING 100

//Setup TrueVision3D
CTVEngine* pEngine;
CTVInputEngine* pInput;
CTVScene* pScene;
CTVMesh* pTeapot;
//CTVGlobals* pGlobals;

void render();
void input();
void unload();

HWND formHWND;
bool bDoLoop;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void AppPath(char* PathOfFile, char* ret_Path)
{							
	char* found = strrchr(PathOfFile, '\\');
	if(!found)
	{
		// check with '/' path format
		found = strrchr(PathOfFile, '/');
		if(!found)
		{
			// no path herre it's just a file.
			// so just blank output
			ret_Path[0] = 0;
		}
		else
		{
			// copy just a part of the string
			int size = (int)found - (int)PathOfFile + 1;
			strncpy(ret_Path, PathOfFile, size);
			ret_Path[size] = 0;
		}

	}
	else
	{
		// copy just a part of the string
		int size = (int)found - (int)PathOfFile + 1;
		strncpy(ret_Path, PathOfFile, size);
		ret_Path[size] = 0;
	}		
}

void InitTV3D()
{
	// Create the pEngine Interface first:
	pEngine = new CTVEngine();

	// Set the debug file/options.
	// Do this before the 3D init so it can log any errors found during init.
	char path[256];
	char srchpath[256];
	
	GetModuleFileName(NULL,path,255); 

	AppPath(path,srchpath);	
	
	pEngine->SetDebugMode(true, true);
	pEngine->SetDebugFile(strcat(srchpath, "\\debugfile.txt"));

	// Set your beta-key/license:
	// pEngine->SetLicenseKey(TV_LICENSE_COMMERCIAL, "username", "key");
	// After setting the beta-key/license its time to init the engine:
	pEngine->Init3DWindowed(formHWND, true);

	// Something good to do is to enable the auto-resize feature:
	// Get the default viewport and set autoresize to true for it:
	pEngine->GetViewport()->SetAutoResize(true);

	// Lets display the FPS:
	pEngine->DisplayFPS(true);

	// Set the prefered angle system:
	pEngine->SetAngleSystem(cTV_ANGLE_DEGREE);

	// Now after we are done initializing the TVEngine component lets continue:
	// Create any other components after pEngine init->

	pScene = new CTVScene();
	
	// Input has an additional init method to call->
	pInput = new CTVInputEngine();
	// Lets init both keyboard and mouse:
	pInput->Initialize(true, true);
	

	// Now we have setup the most basic of components->
	// Something to think about, if the component has a diffrent ATL init method
	// then the Object = new pEngine<NAME>, use that one instead->

	// For example:
	// CTVMesh* Mesh;
	// Mesh = Scene->CreateMeshBuilder("MyMesh"); <- Instead of Mesh = new CTVMesh();
	// Same goes for RenderSurface, Viewport etc->
	
	// Setup the boolean for the loop.
	bDoLoop = true;
}

// A method for Processing Mesages that will be used in the loop.


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEMPLATE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TEMPLATE);

	// Call our Init TV3D method.
	InitTV3D();
        //We set the AngleSystem to Degrees
//        pEngine->SetAngleSystem (CONST_TV_ANGLE_DEGREE);

        //Init input after main init, and check if it errors anywhere.
        pInput = new CTVInputEngine();
        pScene = new CTVScene();
//        pGlobals = new CTVGlobals();           
	float fAngle = 0;
	float fTimeElapsed = 0;
	long LoadTexRet;
	LoadTexRet=pScene->LoadTexture ("Media\\texture.bmp", -1, -1, "TeapotTexture");
	if(LoadTexRet==0)
		MessageBox(NULL, "The Texture did not load properly", "Error", MB_OK);

	//We MUST name the mesh, becuase, the way to set optional params in VB is not compatible in C++
	//Setup the Teapot
	pTeapot = pScene->CreateMeshBuilder ("Teapot");
	pTeapot->CreateTeapot ();
	pTeapot->EnableSphereMapping (tvtrue,-1,-1);
	pTeapot->SetTexture (pGlobals->GetTex("TeapotTexture"),-1);
	pTeapot->SetPosition (0,0,10);
	pTeapot->SetRotation (0, fAngle, fAngle);	

	//Main loop	
	while( msg.message!=WM_QUIT)
	{
		if(PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{		
			render();
			input();
		}
	}

	unload();
	return (int) msg.wParam;
}
//Unload sub
void unload()
{
	//Uninitialize the variables
	pScene->DestroyAllMeshes ();

	pScene->Release ();
	pScene = NULL;

	pInput->Release ();
	pInput = NULL;

	pGlobals->Release ();
	pGlobals = NULL;

	pEngine->Release ();
	pEngine = NULL;
	
	//Uninitialize COM
	CoUninitialize();
}

//Render sub
void render()
{
	//Update the rotation
	fTimeElapsed=pEngine->TimeElapsed ();
	fAngle += fTimeElapsed/100;
	pTeapot->SetRotation (fAngle, fAngle, fAngle);

	pEngine->Clear(false);						//Clear the screen
	pScene->RenderAllMeshes (tvfalse);
	pEngine->RenderToScreen ();					//Render the screen	
}

//Input sub
void input()
{
	if(pInput->IsKeyPressed(TV_KEY_ESCAPE))		//Check if ESCAPE has been pressed.
	{		
		PostQuitMessage(0);
	}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEMPLATE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 640, 480, NULL, NULL, hInstance, NULL);
	
   // Store the Created Window's hWnd in a glboal HWND variable.
   formHWND = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_DESTROY:
		// Stop the loop.
		bDoLoop = false;

		// Free TV:
		delete(pEngine);
		pEngine = NULL;		
		

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
