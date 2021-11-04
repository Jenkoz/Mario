#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define BRICK_WIDTH 16

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_GRAVITY 0.001f
#define BRICK_JUMP_DEFLECT_Y 0.3f

#define BRICK_STATE_DISABLE 1
#define BRICK_STATE_NORMAL 2
#define BRICK_STATE_QUESTION 3

#define ID_ANI_BRICK_STATE_DISABLE 10000
#define ID_ANI_BRICK_STATE_NORMAL 10001
#define ID_ANI_BRICK_STATE_QUESTION 10002

class CBrick : public CGameObject {
	float ay;
	int jumpCount;
public:
	CBrick(float x, float y, int _type) : CGameObject(x, y) { SetState(_type); ay = BRICK_GRAVITY; jumpCount = 0; vy = 0; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);
};