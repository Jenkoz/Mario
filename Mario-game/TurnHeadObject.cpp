#include "TurnHeadObject.h"
#include "Mario.h"
CTurnHeadObject::CTurnHeadObject(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = TURN_HEAD_OBJECT_GRAVITY;
	x_start = x;
	y_start = y;
}

void CTurnHeadObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TURN_HEAD_OBJECT_BBOX_WIDTH / 2;
	top = y - TURN_HEAD_OBJECT_BBOX_HEIGHT / 2;
	right = left + TURN_HEAD_OBJECT_BBOX_WIDTH;
	bottom = top + TURN_HEAD_OBJECT_BBOX_HEIGHT;
}

void CTurnHeadObject::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

};

void CTurnHeadObject::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
			isOnPlatform = true;
	}
}


void CTurnHeadObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (y > y_start) 
	{
		isOnPlatform = false;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CTurnHeadObject::Render()
{
	//RenderBoundingBox();
}