#include "Koopa.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y, int lvl) :CGameObject(x, y)
{
	this->level = lvl;
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	switch (this->level)
	{
	case 1:
	{
		SetState(KOOPA_STATE_WALKING_LEFT);
		break;
	}
	default:
		DebugOut(L"[ERROR] Invalid Koopa type: %d\n", this->level);
		return;
	}
}



void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == SHELL_STATE_IDLING)
	{
		left = x - SHELL_IDLING_BBOX_WIDTH / 2;
		top = y - SHELL_IDLING_BBOX_HEIGHT / 2;
		right = left + SHELL_IDLING_BBOX_WIDTH;
		bottom = top + SHELL_IDLING_BBOX_HEIGHT;
	}
	else if (state == SHELL_STATE_ROLLING_LEFT || state == SHELL_STATE_ROLLING_RIGHT)
	{
		left = x - SHELL_ROLLING_BBOX_WIDTH / 2;
		top = y - SHELL_ROLLING_BBOX_HEIGHT / 2;
		right = left + SHELL_ROLLING_BBOX_WIDTH;
		bottom = top + SHELL_ROLLING_BBOX_HEIGHT;
	}
	else if (state == KOOPA_STATE_WALKING_LEFT || state == KOOPA_STATE_WALKING_RIGHT)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == SHELL_STATE_IDLING) && (GetTickCount64() - die_start > SHELL_IDLING_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopa::Render()
{
	int aniId = -1;
	if (state == KOOPA_STATE_WALKING_LEFT || state == KOOPA_STATE_WALKING_RIGHT)
		aniId = ID_ANI_KOOPA_WALKING;
	else if (state == SHELL_STATE_IDLING)
		aniId = ID_ANI_SHELL_IDLING;
	else if (state == SHELL_STATE_ROLLING_LEFT || state == SHELL_STATE_ROLLING_RIGHT)
		aniId = ID_ANI_SHELL_ROLLING;
	

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SHELL_STATE_IDLING:
		die_start = GetTickCount64();
		vx = SHELL_IDLING_SPEED;
		break;
	case KOOPA_STATE_WALKING_LEFT:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_WALKING_RIGHT:
		vx = KOOPA_WALKING_SPEED;
		break;
	case SHELL_STATE_ROLLING_LEFT:
		vx = -SHELL_ROLLING_SPEED;
		break;
	case SHELL_STATE_ROLLING_RIGHT:
		vx = SHELL_ROLLING_SPEED;
		break;
	}
}
