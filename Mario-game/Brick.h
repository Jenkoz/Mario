#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define BRICK_WIDTH 16

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_GRAVITY 0.02f
#define BRICK_JUMP_DEFLECT_Y 0.3f

#define BRICK_TYPE_DISABLE 1
#define BRICK_TYPE_NORMAL 2
#define BRICK_TYPE_QUESTION 3

#define ID_ANI_BRICK_TYPE_DISABLE 10000
#define ID_ANI_BRICK_TYPE_NORMAL 10001
#define ID_ANI_BRICK_TYPE_QUESTION 10002

class CBrick : public CGameObject {
	float ay;
	float currentY;
	int type;
public:
	CBrick(float x, float y, int _type) : CGameObject(x, y) { this->type = _type; ay = BRICK_GRAVITY; currentY = y; vy = 0; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void OnNoCollision(DWORD dt);
	virtual int isBlocking(){ return 1; }
	int GetType() { return type; }
	void SetType(int _type) { type = _type; }
};