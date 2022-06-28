#pragma once
#include "GameObject.h"

#define VENUS_TRAP_SPEED 0.035f

#define VENUS_TRAP_TYPE_RED 1
#define VENUS_TRAP_TYPE_GREEN 2

#define VENUS_TRAP_RED_BBOX_WIDTH 16
#define VENUS_TRAP_RED_BBOX_HEIGHT 32

#define VENUS_TRAP_GREEN_BBOX_WIDTH 16
#define VENUS_TRAP_GREEN_BBOX_HEIGHT 24


#define VENUS_TRAP_IDLING_TIMEOUT		2000
#define VENUS_TRAP_AIMING_TIMEOUT		2000
#define VENUS_TRAP_SHOOTING_TIMEOUT	500


#define VENUS_TRAP_STATE_GROWING_UP		100
#define VENUS_TRAP_STATE_MOVING_DOWN	200
#define VENUS_TRAP_STATE_AIMING			300
#define VENUS_TRAP_STATE_SHOOTING		400
#define VENUS_TRAP_STATE_IDLING			500


#define ID_ANI_VENUS_TRAP_RED_LEFT_DOWN 8010
#define ID_ANI_VENUS_TRAP_RED_LEFT_UP 8011
#define ID_ANI_VENUS_TRAP_RED_RIGHT_DOWN 8012
#define ID_ANI_VENUS_TRAP_RED_RIGHT_UP 8013

#define ID_ANI_VENUS_TRAP_GREEN_LEFT_DOWN 8020
#define ID_ANI_VENUS_TRAP_GREEN_LEFT_UP 8021
#define ID_ANI_VENUS_TRAP_GREEN_RIGHT_DOWN 8022
#define ID_ANI_VENUS_TRAP_GREEN_RIGHT_UP 8023

class CVenusTrap : public CGameObject
{
protected:

	BOOLEAN isGrowingUp;
	BOOLEAN isMovingDown;
	BOOLEAN isAiming;
	BOOLEAN isShooting;
	BOOLEAN isIdling;

	ULONGLONG start_growingUp;
	ULONGLONG start_movingDown;
	ULONGLONG start_aiming;
	ULONGLONG start_shooting;
	ULONGLONG start_idling;

	float x_start, y_start;
	int nx, ny;
	int type;
	int lastAni;
	float marioX, marioY;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CVenusTrap(float x, float y, int type);
	virtual void SetState(int state);

	void ShootingFireball();

	//start
	void StartGrowingUp()
	{
		isGrowingUp = true;
	}
	void StartMovingDown()
	{
		isMovingDown = true;
	}
	void StartAiming()
	{
		isAiming = true;
		start_aiming = GetTickCount64();
	}
	void StartShooting()
	{
		isShooting = true;
		start_shooting = GetTickCount64();
	}
	void StartIdling()
	{
		isIdling = true;
		start_idling = GetTickCount64();
	}

	//stop
	void StopGrowingUp()
	{
		isGrowingUp = false;
	}
	void StopMovingDown()
	{
		isMovingDown = false;
	}
	void StopAiming()
	{
		isAiming = false;
		start_aiming = 0;
	}
	void StopShooting()
	{
		isShooting = false;
		start_shooting = 0;
	}
	void StopIdling()
	{
		isIdling = false;
		start_idling = 0;
	}
};