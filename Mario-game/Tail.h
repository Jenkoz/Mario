#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH		8
#define TAIL_BBOX_HEIGHT	6
#define TAIL_STATE_HIT_FRONT 2
#define TAIL_STATE_HIT_BACK	 1
#define TAIL_STATE_HIDE 0

#define TAIL_HIT_TIME 100

class CTail :public CGameObject
{
	
	/*void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);*/
	ULONGLONG hit_start = 0;
	BOOLEAN isWhackkingBack = false;
	BOOLEAN isWhackkingFront = false;

public:
	CTail(float x, float y) : CGameObject(x, y) 
	{
		SetState(isWhackkingBack);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	void SetState(int state);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};