#pragma once
#include "GameObject.h"

#define KOOPA_TYPE_RED 1
#define KOOPA_TYPE_GREEN 2
#define KOOPA_TYPE_GREEN_PARA 3

#define KOOPA_GRAVITY 0.001f
#define KOOPA_WALKING_SPEED	0.030f
#define SHELL_ROLLING_SPEED 0.2f
#define PARAKOOPA_BOUNCING_SPEED 0.25f

#define KOOPA_BBOX_WIDTH 15
#define KOOPA_BBOX_HEIGHT 24

#define SHELL_BBOX_WIDTH 15
#define SHELL_BBOX_HEIGHT 14

#define PARAKOOPA_BOUNCING_TIMEOUT 1000
#define SHELL_IDLING_TIMEOUT 8000
#define KOOPA_STANDUP_TIMEOUT 1000

#define KOOPA_STATE_WALKING_LEFT 1
#define KOOPA_STATE_WALKING_RIGHT 2
#define KOOPA_STATE_WAKING 3
#define SHELL_STATE_IDLING 4
#define SHELL_STATE_ROLLING_LEFT 5
#define SHELL_STATE_ROLLING_RIGHT 6
#define KOOPA_STATE_BOUNCING_LEFT 7
#define KOOPA_STATE_BOUNCING_RIGHT 8

#define ID_ANI_RED_KOOPA_WALKING_LEFT 6000
#define ID_ANI_RED_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_RED_SHELL_IDLING 6002
#define ID_ANI_RED_SHELL_ROLLING 6003
#define ID_ANI_RED_SHELL_WAKING 6004

#define ID_ANI_GREEN_KOOPA_WALKING_LEFT		6005
#define ID_ANI_GREEN_KOOPA_WALKING_RIGHT	6006
#define ID_ANI_GREEN_SHELL_IDLING		6007
#define ID_ANI_GREEN_SHELL_ROLLING	6008
#define ID_ANI_GREEN_SHELL_WAKING		6009
#define ID_ANI_GREEN_KOOPA_BOUNCING_LEFT	6010
#define ID_ANI_GREEN_KOOPA_BOUNCING_RIGHT	6011


class CKoopa : public CGameObject
{
	int type;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);

protected:
	float ax;
	float ay;
	int jumpingStack = 0;

	int last_state;

	BOOLEAN isOnPlatform;
	ULONGLONG fly_start;
	ULONGLONG jumpingTimer;
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
	void SetType(int type) { this->type = type; }
	int GetType() { return this->type; }
	CKoopa(float x, float y, int type);
	virtual void SetState(int state);

	void downGrade();

	void switchState();
	float GetCenter() { return x + KOOPA_BBOX_WIDTH; }
	void setLastState(int state) { last_state = state; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};