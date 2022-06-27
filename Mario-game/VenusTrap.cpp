#include "VenusTrap.h"
#include "PlayScene.h"
#include "Mario.h"

CVenusTrap::CVenusTrap(float x, float y, int type) :CGameObject(x, y)
{
	this->oldX = x;
	this->oldY = y;
	this->type = type;
	//this->SetState(VENUS_TRAP_STATE_MOVE);
	//this->ny = 1;
	marioXX = 0;
	marioYY = 0;
}

void CVenusTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->type == VENUS_TRAP_TYPE_RED) {
		left = x - VENUS_TRAP_RED_BBOX_WIDTH / 2;
		top = y - VENUS_TRAP_RED_BBOX_HEIGHT / 2;
		right = left + VENUS_TRAP_RED_BBOX_WIDTH;
		bottom = top + VENUS_TRAP_RED_BBOX_HEIGHT;
	}
	else if (this->type == VENUS_TRAP_TYPE_GREEN) {
		left = x - VENUS_TRAP_GREEN_BBOX_WIDTH / 2;
		top = y - VENUS_TRAP_GREEN_BBOX_HEIGHT / 2;
		right = left + VENUS_TRAP_GREEN_BBOX_WIDTH;
		bottom = top + VENUS_TRAP_GREEN_BBOX_HEIGHT;
	}
}

void CVenusTrap::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CVenusTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CVenusTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE playscreen = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)playscreen->GetPlayer();

	mario->GetPosition(marioXX, marioYY);

	if (marioXX < x) 
	{
		nx = -1;
		if (marioYY < y)
			ny = -1;
		else 
			ny = 1;
	}
	else if (marioXX > x) {
		nx = 1;
		if (marioYY < y) {
			ny = -1;
		}
		else {
			ny = 1;
		}
	}
	//appear
	Appear();

	if (this->type == VENUS_TRAP_TYPE_RED) 
	{
		if (state == VENUS_TRAP_STATE_GROWING && this->y <= oldY - VENUS_TRAP_RED_BBOX_HEIGHT) 
		{
			//shoot
			ShootFireball();
		}
		else if (state == VENUS_TRAP_STATE_MOVE && this->y >= oldY) 
		{
			vy = -VENUS_TRAP_SPEED;
		}

	}
	else if (this->type == VENUS_TRAP_TYPE_GREEN) 
	{
		if (state == VENUS_TRAP_STATE_GROWING && this->y <= oldY - VENUS_TRAP_GREEN_BBOX_HEIGHT)
		{
			//shoot
			ShootFireball();
		}
		else if (state == VENUS_TRAP_STATE_MOVE && this->y >= oldY) 
		{
			vy = -VENUS_TRAP_SPEED;
		}
	}
	if (state == VENUS_TRAP_STATE_SHOOT && countFire > 0 && GetTickCount64() - fire_start > 1000) 
	{
		SetState(VENUS_TRAP_STATE_MOVE);
		vy = -vy;
		countFire = 0;
	}



	//
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CVenusTrap::Render()
{
	int aniId = -1;
	LPPLAYSCENE playscreen = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)playscreen->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);

	if (this->type == VENUS_TRAP_TYPE_RED) 
	{
		if (this->state == VENUS_TRAP_STATE_IDE || this->state == VENUS_TRAP_STATE_MOVE)
			if (marioX < x)
				aniId = ID_ANI_VENUS_TRAP_RED_LEFT_DOWN;
			else
				aniId = ID_ANI_VENUS_TRAP_RED_RIGHT_DOWN;
		else if (this->state == VENUS_TRAP_STATE_SHOOT) 
		{
			if (marioX < x) 
			{
				if (marioY < y) 
					aniId = ID_ANI_VENUS_TRAP_RED_LEFT_UP;
				else 
					aniId = ID_ANI_VENUS_TRAP_RED_LEFT_DOWN;
			}
			else if (marioX > x) 
			{
				if (marioY < y)
					aniId = ID_ANI_VENUS_TRAP_RED_RIGHT_UP;
				else
					aniId = ID_ANI_VENUS_TRAP_RED_RIGHT_DOWN;
			}

		}
	}
	else if (this->type == VENUS_TRAP_TYPE_GREEN) 
	{
		if (this->state == VENUS_TRAP_STATE_IDE || this->state == VENUS_TRAP_STATE_MOVE)
			if (marioX < x)
				aniId = ID_ANI_VENUS_TRAP_GREEN_LEFT_DOWN;
			else
				aniId = ID_ANI_VENUS_TRAP_GREEN_RIGHT_DOWN;
		else if (this->state == VENUS_TRAP_STATE_SHOOT) 
		{
			if (marioX < x) 
			{
				if (marioY < y)
					aniId = ID_ANI_VENUS_TRAP_GREEN_LEFT_UP;
				else
					aniId = ID_ANI_VENUS_TRAP_GREEN_LEFT_DOWN;
			}
			else if (marioX > x) {
				if (marioY < y)
					aniId = ID_ANI_VENUS_TRAP_GREEN_RIGHT_UP;
				else
					aniId = ID_ANI_VENUS_TRAP_GREEN_RIGHT_DOWN;
			}
		}
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CVenusTrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case VENUS_TRAP_STATE_IDE:
		vx = 0;
		vy = 0;
		break;
	case VENUS_TRAP_STATE_MOVE:
		vy = -VENUS_TRAP_SPEED;
		break;
	case VENUS_TRAP_STATE_SHOOT:
		fire_start = GetTickCount64();
		vy = 0;
		break;
	}
}