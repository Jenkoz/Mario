#pragma once

#include "GameObject.h"

#define PIRANHAPLANT_SPEED 0.015f
#define VENUS_FIRE_TRAP_SPEED 0.015f

#define PIRANHAPLANT_GREEN_BBOX_WIDTH 16
#define PIRANHAPLANT_BBOX_HEIGHT 24



#define PIRANHA_PLANT 1


#define PIRANHAPLANT_TIME	1000

class CPiranhaPlant : public CGameObject
{
protected:
	float marioXX, marioYY;
	float oldX, oldY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPiranhaPlant(float x, float y);
	virtual void SetState(int state);
	void Appear();
};