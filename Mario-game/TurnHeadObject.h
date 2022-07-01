#pragma once
#include "GameObject.h"
#define TURN_HEAD_OBJECT_WALKING_SPEED 0.035f
#define TURN_HEAD_OBJECT_GRAVITY 0.002f

#define TURN_HEAD_OBJECT_WIDTH 4
#define TURN_HEAD_OBJECT_BBOX_WIDTH 4
#define TURN_HEAD_OBJECT_BBOX_HEIGHT 15

class CTurnHeadObject : public CGameObject
{
protected:
	float ax;
	float ay;
	float x_start, y_start;
	BOOLEAN isOnPlatform = false;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


public:
	CTurnHeadObject(float x, float y);
	void setSpeed(float vX) { this->vx = vX; }
	BOOLEAN getIsOnPlatform() { return this->isOnPlatform; }
	float getY() { return y; }
};