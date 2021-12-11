#include "Leaf.h"
#include "Mario.h"
#include "PlayScene.h"



void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b) 
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}


CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
	y_start = y;
	SetState(LEAF_STATE_UP);
}

void CLeaf::Render()
{
	int aniId = -1;
	if (state == LEAF_STATE_FALLING)
	{
		if (vx >= 0)
			aniId = ID_ANI_LEAF_SWING_RIGHT;
		else
			aniId = ID_ANI_LEAF_SWING_LEFT;
	}
	else if (state == LEAF_STATE_UP)
		aniId = ID_ANI_LEAF_SWING_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {


	x += vx * dt;
	y += vy * dt;

	if (state == LEAF_STATE_FALLING) {
		if (GetTickCount64() - swing_time >= LEAF_FALLING_TIME) {
			vx = -vx;
			swing_time = GetTickCount64();
		}
	}
	if (state == LEAF_STATE_UP) {
		if (y < y_start - LEAF_UP_HEIGHT) {
			SetState(LEAF_STATE_FALLING);
		}
	}
	CGameObject::Update(dt, coObjects);
}

void CLeaf::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_UP:
		vy = -LEAF_SPEED_UP;
		vx = 0;
		y_start = y;
		break;
	case LEAF_STATE_FALLING:
		vy = LEAF_GRAVITY;
		vx = LEAF_SPEED;
		swing_time = GetTickCount64();
		break;
	}
}