#include "Tail.h"
#include "PlayScene.h"
#include "Mario.h"

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH/2;
	t = y - TAIL_BBOX_HEIGHT/2;
	r = l + TAIL_BBOX_WIDTH/2;
	b = t + TAIL_BBOX_HEIGHT/2;
}
//
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (GetTickCount64() - hit_start >= TAIL_HIT_TIME && hit_start != 0)
		hit_start = 0;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) 
	{
		if (x < mario->GetX())
			nx = -1;
		else
			nx = 1;
		/*if (mario->GetMarioDirection() > 0)
		{
			if (mario->turningStack == 1 || mario->turningStack == 5)
				x = mario->x - TAIL_BBOX_WIDTH;
			else if (mario->turningStack == 3)
				x = mario->x + MARIO_BIG_BBOX_WIDTH;
			else x = mario->x - 8;
		}
		else
		{
			if (mario->turningStack == 1 || mario->turningStack == 5)
				x = mario->x + MARIO_BIG_BBOX_WIDTH;
			else if (mario->turningStack == 3)
				x = mario->x - TAIL_BBOX_WIDTH;
			else x = mario->x + MARIO_BIG_BBOX_WIDTH;
		}*/
		y = mario->GetY() + TAIL_BBOX_WIDTH * 2 + 1;
		//y = mario->y;
	}
}

void CTail::Render() {
	if (hit_start) {
		if (nx < 0)
			CSprites::GetInstance()->Get(TAIL_HIT_SPRITE_ID)->Draw(x - TAIL_BBOX_WIDTH, y - TAIL_BBOX_HEIGHT - 5);
		if (nx > 0)
			CSprites::GetInstance()->Get(TAIL_HIT_SPRITE_ID)->Draw(x + TAIL_BBOX_WIDTH - 2, y - TAIL_BBOX_HEIGHT - 5);
	}
	RenderBoundingBox();
}
