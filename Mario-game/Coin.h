#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_IDLE 11

#define ID_ANI_COIN 11000

class CCoin : public CGameObject {
public:
	CCoin(float x, float y) : CGameObject(x, y) { SetState(COIN_STATE_IDLE); }
	void Render();
	void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};