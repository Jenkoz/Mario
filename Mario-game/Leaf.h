#pragma once
#include "GameObject.h"


#define LEAF_GRAVITY		0.025f
#define LEAF_SPEED			0.075f
#define LEAF_SPEED_UP		0.05f

#define LEAF_FALLING_TIME	500

#define LEAF_UP_HEIGHT	 48

#define LEAF_BBOX_WIDTH  15
#define LEAF_BBOX_HEIGHT 15

#define LEAF_STATE_UP		1
#define LEAF_STATE_FALLING	2

#define ID_ANI_LEAF_SWING_LEFT		12100
#define ID_ANI_LEAF_SWING_RIGHT		12101




class CLeaf :
	public CGameObject
{
	float y_start;
	ULONGLONG swing_time;
public:
	CLeaf(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);


	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }

	int GetState() { return this->state; }
	void SetState(int state);
};