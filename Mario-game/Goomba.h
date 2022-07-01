#pragma once
#include "GameObject.h"

#define LEVEL_GOOMBA 1
#define LEVEL_PARAGOOMBA 2

#define BBOX_GROUND_FIXED 1

#define GOOMBA_GRAVITY			0.0025f
#define GOOMBA_WALKING_SPEED	0.03f

#define GOOMBA_DIE_DEFLECT_SPEED	0.15f
#define PARAGOOMBA_BOUNCING_SPEED	0.125f
#define PARAGOOMBA_JUMPING_SPEED 0.35f;
#define WING_GOOMBA_SPEED_WALKING_WHEN_ONAIR 0.03f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14

#define GOOMBA_BBOX_HEIGHT_DIE 7

#define PARAGOOMBA_BBOX_WIDTH 20
#define PARAGOOMBA_BBOX_HEIGHT 18
#define PARAGOOMBA_JUMP_BBOX_HEIGHT 20
#define PARAGOOMBA_BOUNCING_MAX_STACK 2


#define GOOMBA_DIE_TIMEOUT 500
#define PARAGOOMBA_WING_WALKING_TIMEOUT	750
#define PARAGOOMBA_BOUNCING_TIMEOUT	250
#define PARAGOOMBA_JUMPING_TIMEOUT	500



#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_DEFLECT_LEFT 210
#define GOOMBA_STATE_DIE_DEFLECT_RIGHT 220


#define PARAGOOMBA_STATE_WALKING 300
#define PARAGOOMBA_STATE_WING_WALKING 400
#define PARAGOOMBA_STATE_BOUNCING 500
#define PARAGOOMBA_STATE_JUMPING 600
#define PARAGOOMBA_STATE_DIE 700

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001	
#define ID_ANI_PARAGOOMBA_WALKING 5010
#define ID_ANI_PARAGOOMBA_WING_WALKING 5011
#define ID_ANI_PARAGOOMBA_BOUNCING 5012
#define ID_ANI_PARAGOOMBA_JUMPING 5013
#define ID_ANI_PARAGOOMBA_DIE 5014

class CGoomba : public CGameObject
{
	int level;
	float start_y;

protected:
	float ax;				
	float ay; 
	int jumpingStack = 0;

	ULONGLONG jumpingTimer;
	ULONGLONG die_start;
	ULONGLONG walkingTimer;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y, int lvl);
	void SetLevel(int lvl) { this->level = lvl; }
	int GetLevel() { return level; }
	virtual void SetState(int state);
};