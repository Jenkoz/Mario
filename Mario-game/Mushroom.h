#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_MOVING_SPEED 0.08f
#define MUSHROOM_RISE_SPEED 0.01f

//#define MUSHROOM_STATE_IDLE 100
#define MUSHROOM_STATE_RISING 200
#define MUSHROOM_STATE_MOVING 300

#define MUSHROOM_TYPE_RED 1
#define MUSHROOM_TYPE_GREEN 2


#define MUSHROOM_BBOX_WIDTH 15
#define MUSHROOM_BBOX_HEIGHT 15

#define ID_ANI_MUSHROOM_RED   12000
#define ID_ANI_MUSHROOM_GREEN 12001

class CMushroom : public CGameObject 
{
	float ax, ay;
	float y_start;
	int type;
public:
	CMushroom(float x, float y); 
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; };

 	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);

	int GetState() { return this->state; }
	void SetState(int state);
	void SetType(int type);
};