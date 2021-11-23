#include "Koopa.h"
#include "Goomba.h"
#include "ColourPlatform.h"

#include "Mario.h"
#include "debug.h"
#include "PlayScene.h"

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


void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// kill goomba by hit
	if (goomba->GetState() != GOOMBA_STATE_DIE)
		if (state == SHELL_STATE_ROLLING_LEFT || state == SHELL_STATE_ROLLING_RIGHT)
			if ( e->nx != 0)
			{
				goomba->SetState(GOOMBA_STATE_DIE);
			}
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (state == SHELL_STATE_ROLLING_LEFT || state == SHELL_STATE_ROLLING_RIGHT)
		if (koopa->GetState() != SHELL_STATE_ROLLING_LEFT || koopa->GetState() != SHELL_STATE_ROLLING_RIGHT)
			return;
			/*if (e->nx != 0)
			{
				koopa->SetState(GOOMBA_STATE_DIE);
			}*/
}

void CKoopa::OnCollisionWithColourPlatform(LPCOLLISIONEVENT e)
{
	CColourPlatform* cPlatform = dynamic_cast<CColourPlatform*>(e->obj);
	vector<LPGAMEOBJECT> coObjects = ((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->GetObjects();

	if (state == KOOPA_STATE_WALKING_LEFT || state == KOOPA_STATE_WALKING_RIGHT)

		for (UINT i = 0; i < coObjects.size(); i++)
		{
			if (e->ny < 0 && cPlatform == coObjects[i])
			{
				if (cPlatform == coObjects[i])
				{
					cPlatform = (CColourPlatform*)coObjects[i];
					if (x <= cPlatform->GetBeginPlatform() )
						SetState(KOOPA_STATE_WALKING_RIGHT);
					else if (x + KOOPA_BBOX_WIDTH >= cPlatform->GetEndPlatform())
						SetState(KOOPA_STATE_WALKING_LEFT);
				}
				vy = 0;
			}
		}
}



void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		if (state == KOOPA_STATE_WALKING_LEFT || state == KOOPA_STATE_WALKING_RIGHT)
			if (e->nx > 0)
				SetState(KOOPA_STATE_WALKING_RIGHT);
			else SetState(KOOPA_STATE_WALKING_LEFT);
		else if (state == SHELL_STATE_ROLLING_LEFT || state == SHELL_STATE_ROLLING_RIGHT)
			if (e->nx > 0)
				SetState(SHELL_STATE_ROLLING_RIGHT);
			else SetState(SHELL_STATE_ROLLING_LEFT);
	}
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CColourPlatform*>(e->obj))
		OnCollisionWithColourPlatform(e);
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
	if (state == KOOPA_STATE_WALKING_LEFT)
		aniId = ID_ANI_KOOPA_WALKING_LEFT;
	else if (state == KOOPA_STATE_WALKING_RIGHT)
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;
	else if (state == SHELL_STATE_IDLING)
		aniId = ID_ANI_SHELL_IDLING;
	else if (state == SHELL_STATE_ROLLING_LEFT || state == SHELL_STATE_ROLLING_RIGHT)
		aniId = ID_ANI_SHELL_ROLLING;
	

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
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