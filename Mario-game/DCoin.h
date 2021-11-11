#pragma once
#include "Coin.h"


#define DCOIN_GRAVITY 0.2f 
#define DCOIN_BOUNCING_SPEED 0.2f

#define DCOIN_STATE_IDLING 1
#define DCOIN_STATE_BOUNCING 2
#define DCOIN_STATE_FALLING 3

#define DCOIN_BBOX_WIDTH 7
#define DCOIN_BBOX_HEIGHT	13

#define ID_ANI_DCOIN 11001

class CDCoin : public CCoin 
{
	float ay;
	float start_y;
	int isMultiCoin;
public:
	CDCoin(float x, float y, int _isMultiCoin) : CCoin(x, y) 
	{ 
		isMultiCoin = _isMultiCoin;
		start_y = y;
		ay = 0;
		SetState(DCOIN_STATE_IDLING);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};