#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (type == BRICK_TYPE_DISABLE)
		aniId = ID_ANI_BRICK_TYPE_DISABLE;
	else if (type == BRICK_TYPE_NORMAL)
		aniId = ID_ANI_BRICK_TYPE_NORMAL;
	else if (type == BRICK_TYPE_QUESTION)
		aniId = ID_ANI_BRICK_TYPE_QUESTION;
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}


void CBrick::Update(DWORD dt)
{

}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}