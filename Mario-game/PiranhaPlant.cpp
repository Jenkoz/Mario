#include "PiranhaPlant.h"

CPiranhaPlant::CPiranhaPlant(float x, float y)
{
	this->x_start = x;
	this->y_start = y;
	this->lastAni = -1;

	isMovingDown = false;
	isIdlingUp = false;
	isIdlingDown = false;


	start_idlingUp = 0;
	start_idlingDown = 0;

	this->SetState(PIRANHA_PLANT_STATE_GROWING_UP);
}

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIRANHA_PLANT_BBOX_WIDTH / 2;
	top = y - PIRANHA_PLANT_BBOX_HEIGHT / 2;
	right = left + PIRANHA_PLANT_BBOX_WIDTH;
	bottom = top + PIRANHA_PLANT_BBOX_HEIGHT;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((this->y <= y_start - PIRANHA_PLANT_BBOX_HEIGHT) && isGrowingUp)
	{
		StopGrowingUp();
		SetState(PIRANHA_PLANT_STATE_IDLING_UP);
	}
	if ((GetTickCount64() - start_idlingUp > PIRANHA_PLANT_IDLING_UP_TIMEOUT) && isIdlingUp)
	{
		StopIdlingUp();
		SetState(PIRANHA_PLANT_STATE_MOVING_DOWN);
	}
	if ((this->y >= y_start) && isMovingDown)
	{
		StopMovingDown();
		SetState(PIRANHA_PLANT_STATE_IDLING_DOWN);
	}
	if ((GetTickCount64() - start_idlingDown > PIRANHA_PLANT_IDLING_DOWN_TIMEOUT) && isIdlingDown)
	{
		StopIdlingDown();
		SetState(PIRANHA_PLANT_STATE_GROWING_UP);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPiranhaPlant::Render()
{
	int aniId = ID_ANI_PIRANHA_PLANT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPiranhaPlant::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CPiranhaPlant::SetState(int state)
{
	switch (state)
	{
	case PIRANHA_PLANT_STATE_GROWING_UP:
		vy = -PIRANHA_PLANT_SPEED;
		StartGrowingUp();
		break;
	case PIRANHA_PLANT_STATE_MOVING_DOWN:
		vy = PIRANHA_PLANT_SPEED;
		StartMovingDown();
		break;
	case PIRANHA_PLANT_STATE_IDLING_UP:
		vy = 0;
		StartIdlingUp();
		break;
	case PIRANHA_PLANT_STATE_IDLING_DOWN:
		vy = 0;
		StartIdlingDown();
		break;
	}
	CGameObject::SetState(state);
}
