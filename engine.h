#ifndef _ENGINE_H
#define _ENGINE_H

#include "camera.h"
#include "actor.h"
#include "gui.h"
#include "character_creation.h"

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



class Engine {
private:
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

CTVMesh* Room;


HWND windowHandle;
GUI* gui;

float arrowDirection;
// ----------- END OBJECTS -----------

int my_font;
int black_material, glow_material, tree_material, normal_material;
bool oldRetButton1, oldRetButton2, oldRetButton3, oldRetButton4, menuShown;
public:
	void initGUI(CharacterCreation* character_creation);
	Engine(HWND handle, int screen_x, int screen_y);
	~Engine();
	void render();
	void input();
};
#endif