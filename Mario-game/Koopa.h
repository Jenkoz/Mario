#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.001f
#define KOOPA_WALKING_SPEED	0.030f
#define SHELL_ROLLING_SPEED 0.2f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 24

#define SHELL_BBOX_WIDTH 16
#define SHELL_BBOX_HEIGHT 14


#define SHELL_IDLING_TIMEOUT 8000
#define KOOPA_STANDUP_TIMEOUT 1000

#define KOOPA_STATE_WALKING_LEFT 1
#define KOOPA_STATE_WALKING_RIGHT 2
#define KOOPA_STATE_WAKING 3
#define SHELL_STATE_IDLING 4
#define SHELL_STATE_ROLLING_LEFT 5
#define SHELL_STATE_ROLLING_RIGHT 6

#define ID_ANI_KOOPA_WALKING_LEFT 6000
#define ID_ANI_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_SHELL_IDLING 6002
#define ID_ANI_SHELL_ROLLING 6003
#define ID_ANI_KOOPA_WAKING 6004


class CKoopa : public CGameObject
{
	int level;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithColourPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);


protected:
	float ax;
	float ay;

	int last_state;

	ULONGLONG wakingUp_timer;
	ULONGLONG wakingUp_timeout;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }


public:

	BOOLEAN isBeingHeld;
	BOOLEAN isVulnerable;
	void SetLevel(int lvl) { this->level = lvl; }
	CKoopa(float x, float y, int lvl);
	virtual void SetState(int state);

	void switchState();

	void setLastState(int state) { last_state = state; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};