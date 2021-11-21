#include "Mushroom.h"

#include "Mario.h"
#include "Brick.h"
#include "Brick.h"



CMushroom::CMushroom(float x, float y) : CGameObject(x, y)
{
	ax = 0; 
	ay = 0; 
	y_start = y; 
	type = MUSHROOM_TYPE_RED;
	SetState(MUSHROOM_STATE_RISING);
}

void CMushroom::Render()
{
	int aniId = -1;
	
	if (type == MUSHROOM_TYPE_GREEN)
		aniId = ID_ANI_MUSHROOM_GREEN;
	else if (type == MUSHROOM_TYPE_RED)
		aniId = ID_ANI_MUSHROOM_RED;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}


void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == MUSHROOM_STATE_MOVING)
	{
		if (!e->obj->IsBlocking()) return;
		if (dynamic_cast<CMushroom*>(e->obj)) return;

		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	}
}


void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (this->y < y_start - 16.0f && GetState() == MUSHROOM_STATE_RISING)
		SetState(MUSHROOM_STATE_MOVING);
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::SetType(int type)
{
	if (type == MARIO_LEVEL_SMALL)
		this->type = MUSHROOM_TYPE_RED;
	else this->type = MUSHROOM_TYPE_GREEN;
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MUSHROOM_STATE_RISING:
		vy = -MUSHROOM_RISE_SPEED;
		break;
	case MUSHROOM_STATE_MOVING:
		vx = MUSHROOM_MOVING_SPEED;
		ay = MUSHROOM_GRAVITY;
		break;
	}
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}