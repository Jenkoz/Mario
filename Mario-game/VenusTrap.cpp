#include "VenusTrap.h"
#include "PlayScene.h"
#include "Fireball.h"
#include "Mario.h"

CVenusTrap::CVenusTrap(float x, float y, int type) :CGameObject(x, y)
{
	nx = 0;
	ny = 0;
	this->x_start = x;
	this->y_start = y;
	this->type = type;
	this->lastAni = -1;

	isMovingDown = false;
	isAiming = false;
	isShooting = false;
	isIdling = false;

	start_aiming = 0;
	start_shooting = 0;
	start_idling = 0;

	this->SetState(VENUS_TRAP_STATE_GROWING_UP);

	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();

	mario->GetPosition(marioX, marioY);
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
	y += vy * dt;
}

void CVenusTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CVenusTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Handle Moving up 
	if (this->type == VENUS_TRAP_TYPE_RED) //RED
	{
		// then aiming
		if ((this->y <= y_start - VENUS_TRAP_RED_BBOX_HEIGHT) && isGrowingUp)
		{
			StopGrowingUp();
			SetState(VENUS_TRAP_STATE_AIMING);
		}
		//then stay idling
		if ((this->y >= y_start) && isMovingDown)
		{
			StopMovingDown();
			SetState(VENUS_TRAP_STATE_IDLING);
		}
	}
	else if (this->type == VENUS_TRAP_TYPE_GREEN) //GREEN
	{
		// then aiming
		if ((this->y <= y_start - VENUS_TRAP_GREEN_BBOX_HEIGHT) && isGrowingUp)
		{
			StopGrowingUp();
			SetState(VENUS_TRAP_STATE_AIMING);
		}
		//then stay idling
		if ((this->y >= y_start) && isMovingDown)
		{
			StopMovingDown();
			SetState(VENUS_TRAP_STATE_IDLING);
		}
	}
	//Handle Aiming then shoot 
	if ((GetTickCount64() - start_aiming > VENUS_TRAP_AIMING_TIMEOUT) && isAiming)
	{
		StopAiming();
		SetState(VENUS_TRAP_STATE_SHOOTING);
	}
	//Handle Shooting then moving down
	if ((GetTickCount64() - start_shooting > VENUS_TRAP_SHOOTING_TIMEOUT) && isShooting)
	{
		StopShooting();
		SetState(VENUS_TRAP_STATE_MOVING_DOWN);
	}
	//Handle Handle Idling then growing up
	if ((GetTickCount64() - start_idling > VENUS_TRAP_IDLING_TIMEOUT) && isIdling)
	{
		StopIdling();
		SetState(VENUS_TRAP_STATE_GROWING_UP);
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CVenusTrap::Render()
{
	int aniId = -1;
	LPPLAYSCENE playscreen = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)playscreen->GetPlayer();
	float marioXX, marioYY;
	mario->GetPosition(marioXX, marioYY);
	if (this->type == VENUS_TRAP_TYPE_RED) 
	{
		if (this->state != VENUS_TRAP_STATE_MOVING_DOWN || this->state != VENUS_TRAP_STATE_SHOOTING)
		{
			if (marioYY < y)
			{
				if (marioXX < x)
					aniId = ID_ANI_VENUS_TRAP_RED_LEFT_UP;
				else 
					aniId = ID_ANI_VENUS_TRAP_RED_RIGHT_UP;
			}
			else
			{
				if (marioXX < x)
					aniId = ID_ANI_VENUS_TRAP_RED_LEFT_DOWN;
				else
					aniId = ID_ANI_VENUS_TRAP_RED_RIGHT_DOWN;
			}
			this->lastAni = aniId;
		}
		else aniId = this->lastAni;
	}
	else if (this->type == VENUS_TRAP_TYPE_GREEN || this->state != VENUS_TRAP_STATE_SHOOTING)
	{
		if (this->state != VENUS_TRAP_STATE_MOVING_DOWN)
		{
			if (marioYY < y)
			{
				if (marioXX < x)
					aniId = ID_ANI_VENUS_TRAP_GREEN_LEFT_UP;
				else
					aniId = ID_ANI_VENUS_TRAP_GREEN_RIGHT_UP;
			}
			else
			{
				if (marioXX < x)
					aniId = ID_ANI_VENUS_TRAP_GREEN_LEFT_DOWN;
				else
					aniId = ID_ANI_VENUS_TRAP_GREEN_RIGHT_DOWN;
			}
			this->lastAni = aniId;
		}
		else aniId = this->lastAni;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CVenusTrap::SetState(int state)
{
	switch (state)
	{
	case VENUS_TRAP_STATE_GROWING_UP:
		StartGrowingUp();
		vy = -VENUS_TRAP_SPEED;
		break;
	case VENUS_TRAP_STATE_AIMING:
		StartAiming();
		vy = 0;
		break;
	case VENUS_TRAP_STATE_MOVING_DOWN:
		StartMovingDown();
		vy = VENUS_TRAP_SPEED;
		break;
	case VENUS_TRAP_STATE_IDLING:
		StartIdling();
		vy = 0;
		break;
	case VENUS_TRAP_STATE_SHOOTING:
		StartShooting();
		ShootingFireball();
		vy = 0;
		break;
	}
	CGameObject::SetState(state);
}

void CVenusTrap::ShootingFireball()
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	float marioXX, marioYY;
	mario->GetPosition(marioXX, marioYY);

	if (marioYY < y)
	{
		if (marioXX < x)
			nx = -1;
		else
			nx = 1;
		ny = -1;
	}
	else
	{
		if (marioXX < x)
			nx = -1;
		else
			nx = 1;
		ny = 1;
	}
	CGameObject* obj = NULL;
	if (marioX < x) 
		obj = new CFireball(x - FIREBALL_BBOX_WIDTH, y - FIREBALL_BBOX_HEIGHT, nx, ny);
	else 
		obj = new CFireball(x + FIREBALL_BBOX_WIDTH, y - FIREBALL_BBOX_HEIGHT / 2, nx, ny);
	obj->SetPosition(x, y);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->LoadObject(obj);
}
