#pragma once

#include "GameObject.h"

#define PIRANHA_PLANT_SPEED 0.035f

#define PIRANHA_PLANT_BBOX_WIDTH 16
#define PIRANHA_PLANT_BBOX_HEIGHT 24

#define PIRANHA_PLANT_STATE_GROWING_UP 100
#define PIRANHA_PLANT_STATE_MOVING_DOWN 200
#define PIRANHA_PLANT_STATE_IDLING_UP 300
#define PIRANHA_PLANT_STATE_IDLING_DOWN 400


#define PIRANHA_PLANT_IDLING_UP_TIMEOUT			1000
#define PIRANHA_PLANT_IDLING_DOWN_TIMEOUT		750

#define ID_ANI_PIRANHA_PLANT	8000

class CPiranhaPlant : public CGameObject
{
protected:
	float x_start, y_start;
	int lastAni;

	BOOLEAN isGrowingUp;
	BOOLEAN isMovingDown;
	BOOLEAN isIdlingUp;
	BOOLEAN isIdlingDown;

	ULONGLONG start_idlingUp;
	ULONGLONG start_idlingDown;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

public:
	CPiranhaPlant(float x, float y);
	virtual void SetState(int state);


	//start
	void StartGrowingUp()
	{
		isGrowingUp = true;
	}
	void StartMovingDown()
	{
		isMovingDown = true;
	}
	void StartIdlingUp()
	{
		isIdlingUp = true;
		start_idlingUp = GetTickCount64();
	}
	void StartIdlingDown()
	{
		isIdlingDown = true;
		start_idlingDown = GetTickCount64();
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
	void StopIdlingUp()
	{
		isIdlingUp = false;
		start_idlingUp = 0;
	}
	void StopIdlingDown()
	{
		isIdlingDown = false;
		start_idlingDown = 0;
	}
};