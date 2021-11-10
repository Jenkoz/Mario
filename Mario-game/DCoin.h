#pragma once
#include "Coin.h"


#define DCOIN_GRAVITY 0.01f 
#define DCOIN_BOUNCING_SPEED 0.01f

#define DCOIN_STATE_IDLING 1
#define DCOIN_STATE_BOUNCING 2
#define DCOIN_STATE_DELETED 3

#define DCOIN_BBOX_WIDTH 7
#define DCOIN_BBOX_	13

#define ID_ANI_DCOIN 11001

class CDCoin : public CCoin 
{
	float ay;
	float y_start;
	int isMultiCoin;

	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
public:
	CDCoin(float x, float y, int _isMultiCoin) : CCoin(x, y) 
	{ 
		isMultiCoin = _isMultiCoin;
		y_start = y;
		state = DCOIN_STATE_IDLING;
		ay = 0;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void OnCollisionWith(LPCOLLISIONEVENT e);
	int IsCollidable() { return 1; };
	void OnNoCollision(DWORD dt);
	void SetState(int state);
};