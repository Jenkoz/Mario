#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	
	if (state == BRICK_STATE_DISABLE)
		aniId = ID_ANI_BRICK_STATE_DISABLE;
	else if (state == BRICK_STATE_NORMAL)
		aniId = ID_ANI_BRICK_STATE_NORMAL;
	else if (state == BRICK_STATE_QUESTION)
		aniId = ID_ANI_BRICK_STATE_QUESTION;
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}



void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrick::OnNoCollision(DWORD dt)
{

}



void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}