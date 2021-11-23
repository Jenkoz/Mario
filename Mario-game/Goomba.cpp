#include "Goomba.h"
#include "debug.h"

CGoomba::CGoomba(float x, float y, int lvl) :CGameObject(x, y)
{
	this->level = lvl;
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	jumpingStack = -1;
	die_start = -1;
	start_y = y;
	switch (this->level)
	{
		case 1:
		{
			SetState(GOOMBA_STATE_WALKING);
			break;
		}
		case 2:
		{
			SetState(PARAGOOMBA_STATE_WALKING);
			break;
		}
		default:
			DebugOut(L"[ERROR] Invalid Goomba type: %d\n", this->level);
			return;
	}
}



void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else if (state == GOOMBA_STATE_WALKING)
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT - 1;
	}
	else if (state == PARAGOOMBA_STATE_WALKING || 
			state == PARAGOOMBA_STATE_BOUNCING || 
			state == PARAGOOMBA_STATE_JUMPING)
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - PARAGOOMBA_BBOX_HEIGHT / 2;
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT - 1;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}



void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (dynamic_cast<CGoomba*>(e->obj)) return; 
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}

	if (level == 2)
	{
		if ((state == PARAGOOMBA_STATE_WALKING) && (GetTickCount64() - walkingTimer > PARAGOOMBA_WALKING_TIMEOUT))		
		{
			walkingTimer = 0;
			SetState(PARAGOOMBA_STATE_BOUNCING);
		}
		if (state == PARAGOOMBA_STATE_BOUNCING &&(GetTickCount64() - jumpingTimer > PARAGOOMBA_BOUNCING_TIMEOUT))
		{
			jumpingTimer = 0;
			if (jumpingStack < 2)
			{
				SetState(PARAGOOMBA_STATE_BOUNCING);
				jumpingStack++;
			}
			else SetState(PARAGOOMBA_STATE_JUMPING);

		}	
		if (state == PARAGOOMBA_STATE_JUMPING && (GetTickCount64() - jumpingTimer > PARAGOOMBA_JUMPING_TIMEOUT))
		{
			y = start_y;
			jumpingTimer = 0;
			SetState(PARAGOOMBA_STATE_WALKING);
		}
	}



	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	
	int aniId = -1;
	if (state == GOOMBA_STATE_WALKING)
		aniId = ID_ANI_GOOMBA_WALKING;
	else if (state == PARAGOOMBA_STATE_WALKING)
		aniId = ID_ANI_PARAGOOMBA_WALKING;
	else if (state == GOOMBA_STATE_DIE)
		aniId = ID_ANI_GOOMBA_DIE;
	else if (state == PARAGOOMBA_STATE_JUMPING)
		aniId = ID_ANI_PARAGOOMBA_JUMPING;
	else if (state == PARAGOOMBA_STATE_BOUNCING)
		aniId = ID_ANI_PARAGOOMBA_BOUNCING;
	

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case PARAGOOMBA_STATE_WALKING:
			walkingTimer = GetTickCount64();
			vx = -GOOMBA_WALKING_SPEED;
			ay = GOOMBA_GRAVITY;
			jumpingStack = 0;
			break;
		case PARAGOOMBA_STATE_BOUNCING:
			jumpingTimer = GetTickCount64();
			vy = -PARAGOOMBA_BOUNCING_SPEED;
			break;
		case PARAGOOMBA_STATE_JUMPING:
			jumpingTimer = GetTickCount64();
			vy = -PARAGOOMBA_JUMPING_SPEED;
			if (vx < 0)
				vx = -WING_GOOMBA_SPEED_WALKING_WHEN_ONAIR;
			else if (vx > 0)
				vx = WING_GOOMBA_SPEED_WALKING_WHEN_ONAIR;
			break;
	}
}
