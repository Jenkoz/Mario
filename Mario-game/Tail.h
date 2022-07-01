#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH		5
#define TAIL_BBOX_HEIGHT	5

#define TAIL_STATE_HITTING	100
#define TAIL_STATE_HIDING	 200

#define TAIL_ATTACK_DEFLECT 0.05f
#define TAIL_ATTACK_RANGE 0.05f
#define TAIL_HIT_TIME 100


class CTail :public CGameObject
{
	float x_start;
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithVenusTrap(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);

public:

	void OnNoCollision(DWORD dt);

	CTail(float x, float y) : CGameObject(x, y) 
	{
		SetState(TAIL_STATE_HIDING);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	void SetState(int state);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void Attack(float x, float y, int nx) 
	{
		this->x = x;
		this->y = y;
		this->x_start = x;
		this->nx = nx;

		this->SetState(TAIL_STATE_HITTING);
	};
	int GetWidth() { return TAIL_BBOX_WIDTH; };
};