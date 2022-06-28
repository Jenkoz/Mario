#include "Fireball.h"

#include "FireBall.h"
#include "GameGlobal.h"
#include "Camera.h"
CFireball::CFireball(float x, float y, int nx, int ny) :CGameObject(x, y)
{
	x_start = x;
	y_start = y;
	vx = FIREBALL_VX_SPEED * nx;
	vy = FIREBALL_VY_SPEED * ny;
}

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIREBALL_BBOX_WIDTH / 2;
	top = y - FIREBALL_BBOX_HEIGHT / 2;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}


void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cam_x, cam_y;
	CCamera::GetInstance()->GetCamPos(cam_x, cam_y);
	if (y >= cam_y + SCREEN_HEIGHT || y <= cam_y) {
		this->Delete();
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CFireball::Render()
{
	int aniId = -1;
	if (vx < 0) {
		aniId = ID_ANI_FIREBALL_LEFT;
	}
	else if (vx > 0)
		aniId = ID_ANI_FIREBALL_RIGHT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CFireball::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
