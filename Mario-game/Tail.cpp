#include "Tail.h"
#include "PlayScene.h"
#include "Mario.h"

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH/2;
	t = y - TAIL_BBOX_HEIGHT/2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}
//
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	if (GetTickCount64() - hit_start > TAIL_HIT_TIME && isWhackkingBack)
	{
		hit_start = 0;
		isWhackkingBack = false;
		SetState(TAIL_STATE_HIT_FRONT);
	}
	if (GetTickCount64() - hit_start > TAIL_HIT_TIME && isWhackkingFront)
	{
		
		hit_start = 0;
		isWhackkingFront = false;
		SetState(TAIL_STATE_HIDE);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::Render() 
{
	RenderBoundingBox();
}

void CTail::SetState(int state)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch  (state)
	{
	case TAIL_STATE_HIT_BACK:
		hit_start = GetTickCount64();
		isWhackkingBack = true;
		break;
	case TAIL_STATE_HIT_FRONT:
		if (mario->GetMarioDirection() > 0)
			SetPosition(x + MARIO_BIG_BBOX_WIDTH, y);
		else
			SetPosition(x - MARIO_BIG_BBOX_WIDTH - TAIL_BBOX_WIDTH, y);
		hit_start = GetTickCount64();
		isWhackkingFront = true;
		break;
	case TAIL_STATE_HIDE:
		Delete();
		break;
	}
	
}
