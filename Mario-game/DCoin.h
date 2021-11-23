#pragma once
#include "Coin.h"
#include "PlayScene.h"
#include "Mario.h"


#define DCOIN_GRAVITY 0.2f 
#define DCOIN_BOUNCING_SPEED 0.2f

#define DCOIN_STATE_BOUNCING 1


#define DCOIN_BBOX_WIDTH 15
#define DCOIN_BBOX_HEIGHT	15

#define ID_ANI_DCOIN 11001

class CDCoin : public CCoin 
{
	float start_y;
	int isMultiCoin;
public:
	CDCoin(float x, float y, int _isMultiCoin) : CCoin(x, y) 
	{ 
		isMultiCoin = _isMultiCoin;
		start_y = y;
		SetState(DCOIN_STATE_BOUNCING);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }


	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};