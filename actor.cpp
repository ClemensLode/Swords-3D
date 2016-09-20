#include "actor.h"
#include <math.h>
#include <sstream>

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

MyActor::MyActor(CTVActor* my_actor, CTVLandscape* my_land, float x_pos, float z_pos):
	pActor(my_actor),
	pPosition(new cTV_3DVECTOR(x_pos, 0.0f, z_pos)),
	pOrigPosition(new cTV_3DVECTOR(x_pos, 0.0f, z_pos)),
	pDirection(new cTV_3DVECTOR(0.0f, 0.0f, 0.0f)),
	speed(0.0f),
	strafe(0.0f),
	did_not_move(true),
	did_walk_slowly(false),
	did_walk_backwards(false),
	did_run(false),
	did_strafe_left(false),
	did_strafe_right(false),
	stop_as_soon_as_possible(false),
	followOrderTime(0.0f)
{
	pPosition->y = my_land->GetHeight(pPosition->x, pPosition->z);
	pOrigPosition->y = pPosition->y;
	pActor->SetPosition(pPosition->x, pPosition->y, pPosition->z);
}

MyActor::~MyActor() {
	delete pActor;
	delete pPosition;
	delete pOrigPosition;
	delete pDirection;
}

// rotates view of actor and slows the actor down accordingly
const bool MyActor::rotate(float y_angle_rad) {
	float diff = y_angle_rad - pDirection->y;
	while(diff > M_PI)
		diff -= 2.0f * M_PI;
	if(diff > 0.1f || diff < -0.1f)
		diff /= 4.0f;
	pDirection->y += diff;
	diff = abs(diff);

	speed -= (speed * diff / M_PI);
	strafe -= (strafe * diff / M_PI);

	if(diff < M_PI / 18.0f)
		return true;
	else return false;
}

// main AI of the Non-Player Actors (very simple)
// decides whether (on basis of the distance to the player and the distance to the starting position)
//   whether to follow the player or return to the starting position
void MyActor::follow(const cTV_3DVECTOR* target_position)
{
	cTV_3DVECTOR direct_line = *target_position - *pPosition;
	cTV_3DVECTOR direct_orig_line = *pOrigPosition - *pPosition;

	// is the actor currently not heading home? is player in range but not very close?
	// => rotate and increase speed to get close to the player
	if((followOrderTime == 0.0f) && (direct_line.Length() <= FollowDistance) && (direct_line.Length() > FollowDistance / 2.0f))
	{
		speed += (SprintingSpeed - speed) * (direct_line.Length() / (5.0f * FollowDistance));
		if(speed > SprintingSpeed)
			speed = SprintingSpeed;
		// rotate so the actor looks at the player
		rotate(atan2(direct_line.z, direct_line.x));
	} else 
	// no? then, if the actor is not already there, head towards starting point
	if(direct_orig_line.Length() > 10.0f)
	{
		speed += (SprintingSpeed - speed) * (direct_orig_line.Length() / (5.0f * FollowDistance));
		if(speed > SprintingSpeed)
			speed = SprintingSpeed;
		rotate(atan2(direct_orig_line.z, direct_orig_line.x));
		// follow this order at least 10 seconds
		followOrderTime = 10000.0f;
	}
}

// move actor according to the speed and set the rotation
void MyActor::move(CTVLandscape* my_land, float time_elapsed) {
	pPosition->x += (cosf(pDirection->y) * speed * time_elapsed) + (cosf(pDirection->y + M_PI / 2.0f) * strafe * time_elapsed);
	pPosition->z += (sinf(pDirection->y) * speed * time_elapsed) + (sinf(pDirection->y + M_PI / 2.0f) * strafe * time_elapsed);
	pPosition->y = my_land->GetHeight(pPosition->x, pPosition->z) + 2.0f;

	pActor->SetRotation(0.0f, 270.0f + pDirection->y * (-180.0f / M_PI), 0.0f);
	pActor->SetPosition( pPosition->x, pPosition->y, pPosition->z);
}

// slow down the actor
void MyActor::slowDown(float time_elapsed) {
	if(followOrderTime > 0.0f) {
		followOrderTime -= time_elapsed;
		if(followOrderTime < 0.0f)
			followOrderTime = 0.0f;
	}
	if(strafe < 0.0f) {
		strafe += StrafeSpeed * time_elapsed / 2000.0f;
	} else if(strafe > 0.0f) {
		strafe -= StrafeSpeed * time_elapsed / 2000.0f;
	}

	if(speed < 0.0f)
	{
		speed += BackwardSpeed * time_elapsed / 2000.0f;
		if(speed > 0.0f)
			speed = 0.0f;
	} else if(speed > 0.0f)
	{
		if(speed <= RunningSpeed)
			speed -= RunningSpeed * time_elapsed / 2000.0f;
		else
			speed -= SprintingSpeed * time_elapsed / 4000.0f;
		if(speed < 0.0f)
			speed = 0.0f;
	}
}

// ------- INCREASE SPEED --------
void MyActor::doWalk(float time_elapsed) {
	if(speed < RunningSpeed)
	{
		speed += RunningSpeed * time_elapsed / 500.0f;
		if(speed > RunningSpeed)
			speed = RunningSpeed;
	}
}

void MyActor::doRun(float time_elapsed) {
	if(speed < SprintingSpeed)
	{
		speed += SprintingSpeed * time_elapsed / 1000.0f;
		if(speed > SprintingSpeed)
			speed = SprintingSpeed;
	}
}

void MyActor::doBrake(float time_elapsed) {
	if(speed > 0.0f)
	{
		if(speed < RunningSpeed)
			speed -= RunningSpeed * time_elapsed / 2000.0f;
		else 
			speed -= SprintingSpeed * time_elapsed / 2000.0f;
		if(speed < 0.0f)
			speed = 0.0f;
	} else if(speed > - BackwardSpeed)
	{
		speed -= BackwardSpeed * time_elapsed / 500.0f;
		if(speed < -BackwardSpeed)
			speed = -BackwardSpeed;
	}
}

void MyActor::doStrafeLeft(float time_elapsed) {
	if(strafe > 0.0f) {
		strafe -= StrafeSpeed * time_elapsed / 500.0f;
	} else if(strafe > -StrafeSpeed) {
		strafe -= StrafeSpeed * time_elapsed / 1000.0f;
	}
}

void MyActor::doStrafeRight(float time_elapsed) {
	if(strafe < 0.0f)
	{
		strafe += StrafeSpeed * time_elapsed / 500.0f;
	} else if(strafe < StrafeSpeed)
	{
		strafe += StrafeSpeed * time_elapsed / 1000.0f;
	}
}
// ------- END INCREASE SPEED --------


// set animation id and animation speed according to the actor's previous actions and current status
void MyActor::checkAnimation() {
	if((stop_as_soon_as_possible) && ((int)pActor->GetKeyFrame() % (int)pActor->GetAnimationLength(pActor->GetAnimationID())) == 0)
	{
		pActor->SetAnimationID(0);
		pActor->PlayAnimation(0.002f);
		stop_as_soon_as_possible = false;
	} else

	if((speed == 0.0f) && (strafe == 0.0f))
	{
		// just stopped
		if(!did_not_move) {
			stop_as_soon_as_possible = true;
			if(((int)pActor->GetKeyFrame() % (int)pActor->GetAnimationLength(pActor->GetAnimationID())) > (int)pActor->GetAnimationLength(pActor->GetAnimationID())/2)
				pActor->PlayAnimation(0.002f);
			else
				pActor->PlayAnimation(-0.002f);
		}
		// else -> no change.
	} else  // => is moving now
	{
		// just started
		if(did_not_move) {
			pActor->SetAnimationID(1);
		}
		// else -> no change
	}

	if((speed > RunningSpeed) && pActor->GetAnimationID() != 2)
	{
			pActor->SetAnimationID(2);
	} else if((speed <= RunningSpeed) && pActor->GetAnimationID() == 2)
	{
			pActor->SetAnimationID(1);
	} 
//	else if((speed == 0.0f) && (strafe == 0.0f))
//	{
//		pActor->SetAnimationID(0);
//	}

	if((speed > 0.01f && speed <= RunningSpeed) || (speed < -0.01f) || (((speed < -0.0f) || 
		((speed > 0.01f) && (speed <= RunningSpeed))) && ((strafe < -0.01f) || (strafe > 0.01f))))
	{
		pActor->PlayAnimation(WalkAnimationSpeedFactor * (speed + abs(strafe)/10.0f));
	} else if((speed <= 0.01f) && (speed >= -0.01f) && (strafe <= 0.01f) && (strafe >= -0.01f)) {
		pActor->PlayAnimation(WalkAnimationSpeedFactor * 0.01);
	} else
	{
		pActor->PlayAnimation(SprintAnimationSpeedFactor * (speed + abs(strafe)/10.0f));
	}
}

void MyActor::translateMovementKeys(bool up, bool down, bool left, bool right, bool run, bool back, float camera_direction, float time_elapsed)
{
	if(up && down) {up = false; down = false;}
	if(left && right) {left = false; right = false;}
	if(run && back) {run = false; back = false;}
	if(back) {
		if(up) 
			{down = true; up = false;}
		else if(down) 
			{up = true; down = false;}

		if(left) 
			{right = true; left = false;}
		else
			if(right) {left = true; right = false;}
	}
	float angle = M_PI;

	if(up && left) {angle = angle / 4.0f;}
	else if(up && right) { angle = angle * 7.0f / 4.0f;}
	else if(down && left) { angle = angle * 3.0f / 4.0f;}
	else if(down && right) { angle = angle * 5.0f / 4.0f;}
	else if(up) { angle = 0.0f;}
	else if(down) {}
	else if(left) { angle = angle / 2.0f;}
	else if(right) { angle = angle * 3.0f / 2.0f;}
	else { angle = 0.0f; }

	if(up || angle != 0.0f) {
		if(rotate(camera_direction + angle)) {
			if(run) {
				doRun(time_elapsed);
			} else if(back)	{
				doBrake(time_elapsed);
			}
			else {
				doWalk(time_elapsed);
			}
		}
	}
}

// check what the player is currently doing
void MyActor::determineStatus() {
	did_not_move = ((speed == 0.0f) &&(strafe == 0.0f)) || (pActor->GetAnimationID() == 0);
	did_walk_slowly = (((speed > 0.0f) && (speed <= RunningSpeed)) || (strafe != 0) || (speed < 0.0f));
	did_walk_backwards = (speed < 0.0f);
	did_run = (speed > RunningSpeed);
	did_strafe_left = (strafe < 0.0f);
	did_strafe_right = (strafe > 0.0f);
}

std::string MyActor::getStatusString() {
	std::ostringstream os;
	os << "Current Speed: " << speed << "\n [not moving: " << did_not_move << ", walks: " << did_walk_slowly << ", runs: " << did_run << "\n" << "]";
	return os.str();
}

void MyActor::Render() {
	pActor->Render();

}

cTV_3DVECTOR MyActor::getHeadPosition() const {
	cTV_3DVECTOR t;
	t.x = pPosition->x;
	t.y = pPosition->y + 50.0f;
	t.z = pPosition->z;
	return t;
}

const cTV_3DVECTOR* MyActor::getPosition() const {
	return pPosition;
}

float MyActor::RunningSpeed = 0.06f;
float MyActor::StrafeSpeed = 0.05f;
float MyActor::BackwardSpeed = 0.04f;
float MyActor::SprintingSpeed = 0.12f;
float MyActor::FollowDistance = 300.0f;
float MyActor::WalkAnimationSpeedFactor = 0.04f;
float MyActor::SprintAnimationSpeedFactor = 0.015f;
