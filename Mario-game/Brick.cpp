#include "Brick.h"
#include "Mushroom.h"
#include "DCoin.h"
#include "debug.h"
#include "Leaf.h"
#include "PSwitch.h"

CBrick::CBrick(float x, float y, int _type, int _itemId) : CGameObject(x, y)
{
	this->type = _type;
	this->itemId = _itemId;
	start_y = y;
	SetState(BRICK_STATE_IDLING);
	isCoinPBBrick = false;
}

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



void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;

	if (state == BRICK_STATE_BREAK)
	{
		RevealItem();
		Delete();
	}
	if (this->y < start_y - 8.0f && state == BRICK_STATE_BOUNCING && type == BRICK_TYPE_QUESTION)
	{
		vy = BRICK_BOUNCING_DEFLECT_Y;
	}
	if (this->y > start_y - 1 && state == BRICK_STATE_BOUNCING && type == BRICK_TYPE_QUESTION)
	{
		y = start_y;
		SetState(BRICK_STATE_IDLING);
		SetType(BRICK_TYPE_DISABLE);
		RevealItem();
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICK_STATE_IDLING:
		vy = 0.0f;
		break;
	case BRICK_STATE_BOUNCING:
		vy = - BRICK_BOUNCING_DEFLECT_Y;
		break;
	case BRICK_STATE_BREAK:
		vy = 0.0f;
		break;
	}
}



void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::RevealItem()
{
	CGameObject* obj = NULL;
	CMario* player = ((CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	switch (itemId)
	{
	case 1:
	{
		if (player->GetLevel() == MARIO_LEVEL_SMALL)
			obj = new CMushroom(x, y, MUSHROOM_TYPE_RED);
		else if (player->GetLevel() == MARIO_LEVEL_BIG || player->GetLevel() == MARIO_LEVEL_RACCOON)
			obj = new CLeaf(x, y);
		break;
	}
	case 2:
		obj = new CDCoin(x, y, 0);
		break;
	case 3:
		obj = new CCoin(x, y, isCoinPBBrick);
		break;
	case 4:
		obj = new CPSwitch(x, y);
		break;
	case 5:
		obj = new CMushroom(x, y, MUSHROOM_TYPE_GREEN);
		break;
	}
	obj->SetPosition(x, y);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->LoadObject(obj);
}
