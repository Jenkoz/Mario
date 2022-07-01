#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define DANCINGTREE_BBOX_HEIGHT 15
#define DANCINGTREE_BBOX_WIDTH	15

#define ID_ANI_DANCINGTREE 2502

class CDancingTree : public CGameObject 
{
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 1; }
public:
	CDancingTree(float x, float y);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};