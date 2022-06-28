#include "Tail.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopa.h"
#include "VenusTrap.h"
#include "PiranhaPlant.h"
#include "Brick.h"

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH/2;
	t = y - TAIL_BBOX_HEIGHT/2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}
void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CVenusTrap*>(e->obj))
		OnCollisionWithVenusTrap(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);
}
//
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	if (state == TAIL_STATE_HIDING)
		return;


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::Render() 
{
	if (state == TAIL_STATE_HIDING)
		return;
	RenderBoundingBox();
}

void CTail::SetState(int state)
{
	switch  (state)
	{
	case TAIL_STATE_HITTING:
		vx = 0.2f * nx;
		vy = 0;
		break;
	case TAIL_STATE_HIDING:
		vy = 0;
		break;
	}
	CGameObject::SetState(state);
}
