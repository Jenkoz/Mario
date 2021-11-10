#include "DCoin.h"
#include "Brick.h"

void CDCoin::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	// animation coin land the brick then disappear
	if (e->ny < 0 && state == DCOIN_STATE_BOUNCING)
	{
		SetState(DCOIN_STATE_DELETED);
	}
}

void CDCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_DCOIN)->Render(x, y);
}

void CDCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy = ay * dt;

	if (this->y < y_start - 48.0f && GetState() == DCOIN_STATE_BOUNCING)
		vy -= vy;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CDCoin::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == DCOIN_STATE_BOUNCING)
	{
		if (!e->obj->IsBlocking()) return;
		if (dynamic_cast<CBrick*>(e->obj))
			OnCollisionWithBrick(e);
	}
}

void CDCoin::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CDCoin::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case DCOIN_STATE_BOUNCING:
		ay = DCOIN_BOUNCING_SPEED;
		vy = DCOIN_GRAVITY;
		break;
	case DCOIN_STATE_IDLING:
		vy = 0;
		ay = 0;
		break;
	case DCOIN_STATE_DELETED:
		this->Delete();
		break;
	}
}
