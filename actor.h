#ifndef _ACTOR_HPP
#define _ACTOR_HPP
#include <windows.h>
#include <math.h>
#include <iostream>
//#include "CTVEngine.h"
//#include "CTVScene.h"
//#include "CTVInputEngine.h"
//#include "CTVTextureFactory.h"
//#include "CTVMaterialFactory.h"
//#include "CTVAtmosphere.h"
#include "CTVGlobals.h"
//#include "CTVMesh.h"
//#include "CTVLightEngine.h"
#include "tv_types.h"
//#include "HelperFunctions.h"
//#include "CTVGraphicEffect.h"
//#include "CTVRenderSurface.h"
//#include "CTVInternalObjects.h"
#include <string>

#define NEW_MOVEMENT_MODE

class MyActor {
	public:
		MyActor(CTVActor* my_actor, CTVLandscape* my_land, float x_pos, float z_pos);
		~MyActor();
		void doWalk(float time_elapsed);
		void doBrake(float time_elapsed);
		void doRun(float time_elapsed);
		void doStrafeLeft(float time_elapsed);
		void doStrafeRight(float time_elapsed);
		const bool rotate(float y_angle);
		void follow(const cTV_3DVECTOR* target_position);
		void move(CTVLandscape* my_land, float time_elapsed);

		void translateMovementKeys(bool up, bool down, bool left, bool right, bool run, bool back, float camera_direction, float time_elapsed);

		void slowDown(float time_elapsed);
		void checkAnimation();
		void determineStatus();
		std::string getStatusString();
		void Render();
		cTV_3DVECTOR getHeadPosition() const;
		const cTV_3DVECTOR* getPosition() const;
		CTVActor* pActor;
	private:
		
		cTV_3DVECTOR* pPosition;
		cTV_3DVECTOR* pOrigPosition;
		cTV_3DVECTOR* pDirection;
		
		float speed;
		float strafe;
		bool did_not_move;
		bool did_walk_slowly;
		bool did_walk_backwards;
		bool did_strafe_left;
		bool did_strafe_right;
		bool did_run;
		bool stop_as_soon_as_possible;

		float followOrderTime;

		static float RunningSpeed;
		static float SprintingSpeed;
		static float BackwardSpeed;
		static float StrafeSpeed;

		static float FollowDistance;
		static float WalkAnimationSpeedFactor;
		static float SprintAnimationSpeedFactor;
};


#endif
