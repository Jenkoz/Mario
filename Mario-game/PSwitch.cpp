#include "PSwitch.h"
#include "Brick.h"
#include "PlayScene.h"

CPSwitch::CPSwitch(float x, float y) : CGameObject(x, y)
{
	isAppear = true;
	y_start = y;
	SetState(PSWITCH_STATE_RISING);
	isPressed = false;
}

void CPSwitch::Render()
{
	int aniId = - 1;
	if (!isAppear || isDeleted)
		return;
	if (state == PSWITCH_STATE_PRESSED)
		aniId = ID_ANI_PSWTICH_PRESSED;
	else
		aniId = ID_ANI_PSWITCH_IDLE;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PSWITCH_BBOX_WIDTH / 2;
	t = y - PSWITCH_BBOX_HEIGHT / 2;
	r = l + PSWITCH_BBOX_WIDTH;
	if (state == PSWITCH_STATE_PRESSED)
		b = y + PSWITCH_BBOX_PRESSED_HEIGHT;
	else
		b = y + PSWITCH_BBOX_HEIGHT;
}

void CPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted)
		return;
	y += vy * dt;
	CGameObject::Update(dt);
	if (this->y < y_start - 15.0f && GetState() == PSWITCH_STATE_RISING)
		SetState(PSWITCH_STATE_IDLE);
	if (isPressed)
		ChangeCoinToBrick();

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPSwitch::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PSWITCH_STATE_IDLE:
		isPressed = false;
		vy = 0;
		break;
	case PSWITCH_STATE_RISING:
		vy = -0.03f;
		y_start = y;
		break;
	case PSWITCH_STATE_PRESSED:
		vy = 0;
		ChangeBrickToCoin();
		isPressed = true;
		break;
	}
}


void CPSwitch::ChangeBrickToCoin() 
{	
	vector<LPGAMEOBJECT> coObjects = ((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->GetObjects();
		for (UINT i = 0; i < coObjects.size(); i++)
		{
			if (dynamic_cast<CBrick*>(coObjects.at(i)) && !coObjects.at(i)->IsDeleted())
			{
				CBrick * brick = dynamic_cast<CBrick*>(coObjects.at(i));
				if (brick->GetItemType() != BRICK_ITEM_TYPE_PSWITCH && brick->GetType() == BRICK_TYPE_NORMAL)
				{
					brick = (CBrick*)coObjects[i];
					brick->isCoinPBBrick = true;
					brick->SetState(BRICK_STATE_BREAK);
				}
			}
		}
}

void CPSwitch::ChangeCoinToBrick()
{
	if (GetTickCount64() - timeoutToBrick > PSWITCH_TIMEOUT_COINTOBRICK && isPressed)
	{
		SetState(PSWITCH_STATE_IDLE);
	}
}
