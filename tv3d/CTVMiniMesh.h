#ifndef MINIMESH_H
#define MINIMESH_H

#pragma once
#include "CTVMesh.h"
#include "CTVShader.h"

typedef unsigned char byte;

class EXPORTCLASS CTVMiniMesh
{

public:
	CTVMiniMesh(void);
	~CTVMiniMesh(void);

	int iMiniMeshIndex;

	int CreateBillboard(float iWidth, float iHeight, bool bCenterHeight = false);
	int CreateFromMesh(CTVMesh* pMesh, bool bPrecomputeScale = true);
	
	void Render();
	void Destroy();

	int SetPositionArray(int iNumPositionElements, cTV_3DVECTOR* pPositionArray);
	int SetRotationArray(int iNumRotationElements, cTV_3DVECTOR* pRotationArray);
	int SetScaleArray(int iNumScaleElements, cTV_3DVECTOR* pScaleArray);
	int SetMatrixArray(int iNumMatrixElements, cTV_3DMATRIX* pMatrixArray);

	void SetFadeOptions(bool bEnableFading, float fFarDistance = 1024, float fStartFadingDistance = 768, float fFadeMaxAlpha = 1.0f);

	void SetTexture(int iTexture);
	void SetMaterial(int iMaterial);
	int GetTexture();
	int GetMaterial();

	void SetPosition(float x, float y, float z, int iIndex = 0);
	cTV_3DVECTOR GetPosition(int iIndex = 0);

	void SetScale(float x, float y, float z, int iIndex = -1);
	cTV_3DVECTOR GetScale(int iIndex = 0);

	void SetRotation(float x, float y, float z, int iIndex = -1);
	cTV_3DVECTOR GetRotation(int iIndex = 0);

	void SetMatrix(cTV_3DMATRIX* mMat, int iIndex = 0);
	cTV_3DMATRIX GetMatrix(int iIndex = 0);

	void SetMaxMeshCount(int iCount); 

	//
	void SetAlphaTest(bool bEnableAlphaTest, int iRefValue = 128, bool bWriteToZBuffer = true);
	void SetBlendingMode(cCONST_TV_BLENDINGMODE eBlendingMode);
	void SetRotationMode(cCONST_TV_BILLBOARDTYPE eRotationMode);
	void SetCullMode(cCONST_TV_CULLING eCulling);

	void Enable(bool bEnable);
	void EnableMiniMesh(int iIndex, bool bEnable);
	int SetEnableArray(int iNumEnableArray, byte* pEnableArray);

	void SetColor(int cColor, int iIndex = -1);
	int GetColor(int iIndex);
	int SetColorArray(int iNumColorArray, int* pColorArray);

	void SetColorMode(bool bUseColor);
	void SetGlobalPosition(float fX, float fY, float fZ);
	void SetAlphaSort(bool bEnable);
	void SetClamping(bool bEnable);


	void SetShader(CTVShader* pCustomShader);
	CTVShader* GetShader();

};

#endif
