#include "DCoin.h"
#include "Brick.h"



void CDCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_DCOIN)->Render(x, y);

	//RenderBoundingBox();
}


void CDCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	CMario* player = ((CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	if (this->y > start_y && GetState() == DCOIN_STATE_BOUNCING)
	{
		y = start_y;
		isDeleted = true;
		player->CoinUp();
	}
	if ((this->y < start_y - 32.0f) && GetState() == DCOIN_STATE_BOUNCING)
	{
		vy = +DCOIN_BOUNCING_SPEED;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CDCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - DCOIN_BBOX_WIDTH / 2;
	t = y - DCOIN_BBOX_HEIGHT / 2;
	r = l + DCOIN_BBOX_WIDTH;
	b = t + DCOIN_BBOX_HEIGHT;
}

void CDCoin::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case DCOIN_STATE_BOUNCING:
		vy = - DCOIN_BOUNCING_SPEED;
		break;
	}
}
