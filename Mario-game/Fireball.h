#pragma once

#include "GameObject.h"
#define FIREBALL_VX_SPEED 0.045f
#define FIREBALL_VY_SPEED 0.035f

#define FIREBALL_BBOX_WIDTH 8
#define FIREBALL_BBOX_HEIGHT 8

#define ID_ANI_FIREBALL	8500 
class CFireball : public CGameObject
{
	float x_start, y_start;

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnNoCollision(DWORD dt);

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	CFireball(float x, float y, int nx, int ny);
};