#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_CLBLOCK 12000
#define CLBLOCK_WIDTH 16
#define CLBLOCK_BBOX_WIDTH 16
#define CLBLOCK_BBOX_HEIGHT 16

class CColourBlock : public CGameObject {
public:
	CColourBlock(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};