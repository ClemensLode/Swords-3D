#ifndef _CAMERA_HPP
#define _CAMERA_HPP
//#include <windows.h>
#include <math.h>
#include <windows.h>
//#include <iostream>
/*#include "CTVEngine.h"
#include "CTVScene.h"
#include "CTVInputEngine.h"
#include "CTVTextureFactory.h"
#include "CTVMaterialFactory.h"
#include "CTVAtmosphere.h"*/
#include "CTVGlobals.h"
/*#include "CTVMesh.h"
#include "CTVLightEngine.h"*/
#include "tv_types.h"
/*#include "HelperFunctions.h"
#include "CTVGraphicEffect.h"
#include "CTVRenderSurface.h"
#include "CTVInternalObjects.h"*/

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

class MyCamera
{
	public:
		MyCamera();
		~MyCamera();
		void updateMouse(int mouse_dx, int mouse_dy);
		void doZoom(int wheel);
		void updateViewVector(cTV_3DVECTOR lookTo, CTVLandscape* pLand);
		const cTV_3DVECTOR* getDirection() const;
		const cTV_3DVECTOR* getPosition() const;
		cTV_3DVECTOR* getViewVector() const;
	private:
		cTV_3DVECTOR* pPosition;
		cTV_3DVECTOR* pDirection;
		cTV_3DVECTOR* pViewVector;

		float zoom;
};

#endif