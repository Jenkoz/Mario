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
void CTail::OnCollisionWithVenusTrap(LPCOLLISIONEVENT e)
{

}
void CTail::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{

}
void CTail::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
		if (e->nx != 0)
		{
			if (brick->GetType() == BRICK_TYPE_QUESTION)
			{
				brick->SetType(BRICK_TYPE_DISABLE);
				brick->RevealItem();
			}
			if (brick->GetType() == BRICK_TYPE_NORMAL)
				if (brick->GetItemType() != BRICK_ITEM_TYPE_PSWITCH)
					brick->Delete();
				else
				{
					brick->SetType(BRICK_TYPE_DISABLE);
					brick->RevealItem();
				}
		}
	vector<LPGAMEOBJECT> coObjects = ((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->GetObjects();
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (e->nx != 0) 
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		if (goomba->GetLevel() == LEVEL_GOOMBA)
		{
			vx = -TAIL_ATTACK_DEFLECT * nx;
			if (goomba->GetState() != GOOMBA_STATE_DIE)
				goomba->SetState(GOOMBA_STATE_DIE);
		}
		else if (goomba->GetLevel() == LEVEL_PARAGOOMBA)
		{
			if (goomba->GetState() != PARAGOOMBA_STATE_DIE)
			{
				vx = -TAIL_ATTACK_DEFLECT * nx;
				if (goomba->GetState() != PARAGOOMBA_STATE_WALKING)
					goomba->SetState(PARAGOOMBA_STATE_WALKING);
				else 
					goomba->SetState(PARAGOOMBA_STATE_DIE);
			}
		}
	}

}
void CTail::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa->GetState() != SHELL_STATE_IDLING || koopa->GetState() != SHELL_STATE_ROLLING_LEFT || koopa->GetState() != SHELL_STATE_ROLLING_RIGHT)
	{
		vx = -TAIL_ATTACK_DEFLECT * nx;
		if (e->nx != 0)
			koopa->SetState(SHELL_STATE_IDLING);
	}
	else if (koopa->GetState() == SHELL_STATE_IDLING)
	{
		vx = -TAIL_ATTACK_DEFLECT * nx;
		if (e->nx > 0)
			koopa->SetState(SHELL_STATE_ROLLING_LEFT);
		else if (e->nx < 0)
			koopa->SetState(SHELL_STATE_ROLLING_RIGHT);
	}
}
//
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	if (state == TAIL_STATE_HIDING)
		return;


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
}

void CTail::Render() 
{
	if (state == TAIL_STATE_HIDING)
		return;
	//RenderBoundingBox();
}

void CTail::SetState(int state)
{
	switch  (state)
	{
	case TAIL_STATE_HITTING:
		vx = TAIL_ATTACK_RANGE * nx;
		vy = 0;
		break;
	case TAIL_STATE_HIDING:
		vy = 0;
		break;
	}
	CGameObject::SetState(state);
}
