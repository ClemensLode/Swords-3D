#pragma once
#include "tv_types.h"

class EXPORTCLASS CTVDeviceInfo
{
public:
	int iDeviceInfoIndex;
	CTVDeviceInfo(void);
	~CTVDeviceInfo(void);

	int GetAdapterCount();
	void SetCurrentAdapter(int iAdapter);


	int GetDisplayModeCount();
	cTV_MODEFORMAT GetDisplayMode(int iIndex);
	cTV_MODEFORMAT GetCurrentDisplayMode();


	bool IsPixelShaderSupported();
	bool IsVertexShaderSupported();
	bool IsStencilShadowsSupported();
	bool IsBumpmappingSupported();
	bool IsReflectionSupported();
	bool IsDoubleSidedStencilSupported();

	float GetMaxPixelShaderVersion();
	float GetMaxVertexShaderVersion();

	char* GetAdapterName();
	char* GetDriverName();
	int GetAdapterID();
	int GetMaxAnisotropy();


};
