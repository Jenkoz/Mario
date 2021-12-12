#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Camera.h"

#include "Goomba.h"
#include "Koopa.h"
#include "Coin.h"
#include "DCoin.h"
#include "Portal.h"
#include "Brick.h"
#include "Mushroom.h"
#include "Leaf.h"

#include "Collision.h"

CMario* CMario::__instance = NULL;

CMario* CMario::GetInstance()
{
	if (__instance == NULL)
		__instance = new CMario();
	return __instance;
}
CMario::CMario()
{

}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
}

void CMario::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CCamera::GetInstance()->GetCamPos(cx, cy);
	if (level != MARIO_LEVEL_RACCOON)
		CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
	else
	{
		if (nx > 0)
			CGame::GetInstance()->Draw(x - cx + 4, y - cy, bbox, &rect, BBOX_ALPHA);
		else if (nx < 0)
			CGame::GetInstance()->Draw(x - cx - 4, y - cy, bbox, &rect, BBOX_ALPHA);
	}
}

void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (e->ny > 0)
	{
		// Big mario can hit normal brick
		if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
		{
			if (brick->GetType() != BRICK_TYPE_DISABLE)
			{
				brick->SetState(BRICK_STATE_BOUNCING);
			}
		}
		// Small mario can only hit the question brick
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (brick->GetType() == BRICK_TYPE_QUESTION)
			{
				brick->SetState(BRICK_STATE_BOUNCING);
			}
		}
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> make paragoomba a goomba, if there's a goomba, it kills goomba
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{ 
			if (goomba->GetLevel() == 2)
			{
				DebugOut(L">>> GOOMBA LEVEL %d >>> \n", goomba->GetLevel());
				goomba->SetLevel(1);
				goomba->SetState(GOOMBA_STATE_WALKING);
				//DebugOut(L">>> GOOMBA LEVEL DOWN to %d >>> \n", goomba->GetState());
			}
			else
				goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level != MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	CCoin* coin = dynamic_cast<CCoin*>(e->obj);
	//DebugOut(L">>Coin state = %d<<\n", coin->GetState());
	if (coin->GetState() == COIN_STATE_IDLE)
	{
		coin->Delete();
		coin++;
	}
}


void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	// jump on top >> make koopa a shell
	if (e->ny < 0)
	{
		if (koopa->GetState() != SHELL_STATE_IDLING)
		{
			if (koopa->GetState() == KOOPA_STATE_WALKING_LEFT || koopa->GetState() == KOOPA_STATE_WALKING_RIGHT)
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->SetState(SHELL_STATE_IDLING);
			}
		}
		
	}
	else // hit by koopa
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() != SHELL_STATE_IDLING)
			{
				if (level != MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
	// kick the shell
	if (e->nx != 0)
	{
		if (koopa->GetState() == SHELL_STATE_IDLING)
		{
			if (e->nx > 0)
				koopa->SetState(SHELL_STATE_ROLLING_LEFT);
			else koopa->SetState(SHELL_STATE_ROLLING_RIGHT);
		}
	}
	else // hit by SHELL
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() != SHELL_STATE_IDLING)
			{
				if (level != MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	
	// Eat mushroom
	if (mushroom->GetState() == MUSHROOM_STATE_MOVING)
	{
		//DebugOut(L"state mushroom: %d \n", mushroom->GetState());
		if (level == MARIO_LEVEL_SMALL)
		{
			SetLevel(MARIO_LEVEL_BIG);
			e->obj->Delete();
		}
		else if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
		{
			LifeUp();
			e->obj->Delete();
		}
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	// Big Mario eat Leaf
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (level == MARIO_LEVEL_BIG)
	{
		SetLevel(MARIO_LEVEL_RACCOON);
		e->obj->Delete();
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

//
// Get animation ID for Raccoon Mario
//
int CMario::GetAniIdRaccoon() 
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
	}
	//if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP || isHolding || isKick) {
	//	/*if (nx > 0) {
	//		aniId = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
	//		if (isTailFlying) {
	//			aniId = MARIO_ANI_TAIL_FLY_UP_RIGHT;
	//		}
	//		if (isFlapping)
	//			aniId = MARIO_ANI_TAIL_FLAPPING_RIGHT;
	//		else if (isFlappingTailFlying)
	//			aniId = MARIO_ANI_TAIL_FLY_FLAPPING_RIGHT;
	//		else if (isHolding) {
	//			aniId = MARIO_ANI_TAIL_HOLD_RUNNING_RIGHT;
	//		}
	//		else if (isKick)
	//		{
	//			aniId = MARIO_ANI_TAIL_KICKING_RIGHT;
	//		}
	//	}
	//	if (nx < 0) {
	//		aniId = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
	//		if (isTailFlying) {
	//			aniId = MARIO_ANI_TAIL_FLY_UP_LEFT;
	//		}
	//		if (isFlapping)
	//			aniId = MARIO_ANI_TAIL_FLAPPING_LEFT;
	//		else if (isFlappingTailFlying)
	//			aniId = MARIO_ANI_TAIL_FLY_FLAPPING_LEFT;
	//		else if (isHolding) {
	//			aniId = MARIO_ANI_TAIL_HOLD_RUNNING_LEFT;
	//		}
	//		else if (isKick)
	//		{
	//			aniId = MARIO_ANI_TAIL_KICKING_LEFT;
	//		}
	//	}*/
	//}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SITTING_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X) {
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
				}

				/*if (!isOnPlatform) {
					aniId = ID_ANI_MARIO_RACCOON_JUMP_RIGHT;
					if (isTailFlying) {
						aniId = ID_ANI_MARIO_RACCOON_FLY_UP_RIGHT;
						if (isFlappingTailFlying)
							aniId = ID_ANI_MARIO_RACCOON_FLY_FLAPPING_RIGHT;
					}
					if (isFlapping)
						aniId = ID_ANI_MARIO_RACCOON_FLAPPING_RIGHT;
				}*/
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;

				//if (!isOnPlatform) {
				//	aniId = ID_ANI_MARIO_RACCOON_JUMPINGUP_LEFT;
				//	if (isTailFlying) {
				//		aniId = ID_ANI_MARIO_RACCOON_FLY_UP_LEFT;
				//		if (isFlappingTailFlying)
				//			aniId = ID_ANI_MARIO_RACCOON_FLAPPING_LEFT;
				//	}
				//	if (isFlapping)
				//		aniId = ID_ANI_MARIO_RACCOON_FLAPPING_LEFT;
				//}
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
	{
		aniId = GetAniIdBig();
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		aniId = GetAniIdSmall();
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		aniId = GetAniIdRaccoon();
	}

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}



void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

