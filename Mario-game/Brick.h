#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define BRICK_WIDTH 16

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_TYPE_DISABLE 1
#define BRICK_TYPE_NORMAL 2
#define BRICK_TYPE_QUESTION 3

#define ID_ANI_BRICK_TYPE_DISABLE 10000
#define ID_ANI_BRICK_TYPE_NORMAL 10001
#define ID_ANI_BRICK_TYPE_QUESTION 10002

class CBrick : public CGameObject {
	int type;
public:
	CBrick(float x, float y, int _type) : CGameObject(x, y) { type = _type; }
	void Render();
	void SetType(int _type) { type = _type; }
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};