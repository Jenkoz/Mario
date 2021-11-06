#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_MOVING_SPEED 0.08f
#define MUSHROOM_RISE_SPEED 0.01f

#define MUSHROOM_STATE_IDLE 100
#define MUSHROOM_STATE_RISING 200
#define MUSHROOM_STATE_MOVING 300

#define MUSHROOM_TYPE_RED 1
#define MUSHROOM_TYPE_GREEN 2


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define ID_ANI_MUSHROOM_RED   12000
#define ID_ANI_MUSHROOM_GREEN 12001

class CMushroom : public CGameObject 
{
	float ax, ay;
	float y_start;
	int type;
public:
	CMushroom(float x, float y); 
	virtual void Render();
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsBlocking() { return 0; }
	virtual int IsCollidable() { return 1; };
 	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);
	int GetState() { return this->state; }
	virtual void SetState(int state);
	void SetType(int type);
};