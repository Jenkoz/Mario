#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_IDLE 11

#define ID_ANI_COIN 11000
#define ID_ANI_COIN_BR 11002

#define COIN_TIMEOUT_COINTOBRICK 6000

class CCoin : public CGameObject 
{
	BOOLEAN wasBBrick;
public:
	ULONGLONG reverseTimeOut;
	CCoin(float x, float y) : CGameObject(x, y)
	{
		SetState(COIN_STATE_IDLE);
		wasBBrick = false;
		reverseTimeOut = 0;
	}
	CCoin(float x, float y, BOOLEAN _isBBrick) : CGameObject(x, y) 
	{ 
		SetState(COIN_STATE_IDLE);
		wasBBrick = _isBBrick;
		if (wasBBrick)
			reverseTimeOut = GetTickCount64();
		else reverseTimeOut = 0;
	}

	void ChangeCoinToBrick();

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};