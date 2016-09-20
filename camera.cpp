#include "camera.h"

MyCamera::MyCamera():
	pPosition(new cTV_3DVECTOR(0.0f, 20.0f, 0.0f)),
	pDirection(new cTV_3DVECTOR(0.0f, 0.0f, 0.0f)),
	pViewVector(new cTV_3DVECTOR(0.0f, 0.0f, 0.0f)),
	zoom(1.0f)
{}

MyCamera::~MyCamera() {
	delete pPosition;
	delete pDirection;
	delete pViewVector;
}

// rotates the camera according to the relative movement of the mouse
void MyCamera::updateMouse(int mouse_dx, int mouse_dy)
{
	pDirection->y -= mouse_dx / 500.0f;
	pDirection->x -= mouse_dy / 500.0f;
	if(pDirection->x > 1.3f)
		pDirection->x = 1.3f;
	if(pDirection->x < -1.3f)
		pDirection->x = -1.3f;
}

// updates the camera direction to look at a certain point (e.g. the player's head) 
// makes sure that the camera is not positioned below the surface
void MyCamera::updateViewVector(cTV_3DVECTOR lookTo, CTVLandscape* pLand)
{
	pViewVector->x = lookTo.x - (cosf(pDirection->y) * 125.0f * zoom);
	pViewVector->z = lookTo.z - (sinf(pDirection->y) * 125.0f * zoom);
	pViewVector->y = lookTo.y + 100.0f - (tanf(pDirection->x) * 125.0f * zoom);
	if(pLand->GetHeight(pViewVector->x, pViewVector->z) + 10.0f > pViewVector->y)
	{
		pViewVector->y = pLand->GetHeight(pViewVector->x, pViewVector->z) + 10.0f;
	}
}

// zoom in/out
void MyCamera::doZoom(int wheel) {

	if((wheel > 0) && (zoom > 0.1f))
		zoom /= 1.2f;
	if((wheel < 0) && (zoom < 10.0f))
		zoom *= 1.2f;
}

const cTV_3DVECTOR* MyCamera::getDirection() const {
	return pDirection;
}

const cTV_3DVECTOR* MyCamera::getPosition() const {
	return pPosition;
}

cTV_3DVECTOR* MyCamera::getViewVector() const {
	return pViewVector;
}