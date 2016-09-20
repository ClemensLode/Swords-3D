#pragma once
#include "tv_types.h"
#include "CTVViewport.h"

class EXPORTCLASS CTVEngine
{
public:
	int iEngineIndex;

	CTVEngine(void);
	~CTVEngine(void);
	

	bool Init3DWindowed(HWND hWnd, bool bEnableTL = true);
	bool Init3DFullscreen(int iWidth, int iHeight, int iBPP = 32, bool bEnableTNL = true, bool bEnableVSync = false, cCONST_TV_DEPTHBUFFERFORMAT eDepthBufferFormat = cTV_DEPTHBUFFER_BESTBUFFER, float fGamma = 1.0f, HWND iOptionalFormHandle = NULL);

 
	int RenderToScreen(void);
	int Clear(bool bOnlyZBuffer = false);
	int GetFPS(void);
	void AddToLog(const char* text);

	// viewport management
	void SetViewport(CTVViewport* newViewport);
	CTVViewport* GetViewport();
	CTVViewport* CreateViewport(HWND newHwnd, const char* sViewportName);
	
	CTVCamera* GetCamera();
	void SetCamera(CTVCamera* newCamera);

	float TimeElapsed();
	float AccurateTimeElapsed();

	void SetSearchDirectory(const char* sSearchDirectory);
	
	// new
	int TickCount();

	void SetBetaKey(const char* sBetaUser, const char* sSerialKey);
	void GetVideoMode(int* retWidth, int* retHeight, int* retBitDepth);
	void SetAngleSystem(cCONST_TV_ANGLE eAngleSystem); 

	void DisplayFPS(bool bEnable);
	void Screenshot(const char* sFilename, cCONST_TV_IMAGEFORMAT eImageFormat);

	bool SwitchFullscreen(int iWidth, int iHeight, int iBPP = -1, cCONST_TV_DEPTHBUFFERFORMAT eDepthBufferFormat = cTV_DEPTHBUFFER_BESTBUFFER, HWND hOptionalNewHandle = (HWND)-1);
	bool SwitchWindowed(HWND hOptionalNewHandle = NULL);
	bool ResizeDevice();
	void SetDebugFile( const char* sDebugFile); 
	void SetDebugMode(bool bEnable, bool bWriteToDebugConsole = false, bool bWriteToStdOut = false);


	void SetFPUPrecision(bool bKeepDoublePrecision);

	void ShowWinCursor(bool bShow);
	void SetVSync(bool bEnable); 
	void SetAntialiasing(bool bEnable, cCONST_TV_MULTISAMPLE_TYPE eNumSamples); 
	void SetLicenseKey(cCONST_TV_LICENSEMODE eLicenseMode, const char* sLicenseUser, const char* sLicenseKey);
	void SetWatermarkParameters(cCONST_TV_WATERMARKPLACE ePlace, float fAlpha = 0.8f);
	void ReleaseAll(); 
	void AllowMultithreading(bool bAllow);
	void SetInitAdapter(int iAdapter);


	int RenderToOtherHWND(HWND hWnd);
	void EnableProfiler(bool bEnable, bool bSilentProfiler = false);
	void EnableSmoothTime(bool bEnable, int iFramesAverage = 10);
	void SetInternalShaderVersion(cCONST_TV_SHADERMODEL eShaderModel);
	void SetWindowTitle(const char* sWindowName);
	cCONST_TV_SHADERMODEL GetInternalShaderVersion();
	bool Init3DNoRender(HWND hWnd);

	cCONST_TV_ANGLE GetAngleSystem();


};
