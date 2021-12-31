#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Camera.h"
#include "Tail.h"

#include "Goomba.h"
#include "Koopa.h"
#include "Coin.h"
#include "DCoin.h"
#include "Portal.h"
#include "Brick.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "PSwitch.h"
#include "DeadPlatform.h"


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
	HandleMarioUntouchable();
	// kicking state
	HandleMarioKicking();
	// whiping state
	HandleMarioWhippingTail();
	 //pipe handle
	HandleMarioEnterPipe();
	HandleMarioStackSpeed();
	HandleMarioFlying();
	HandleMarioFallingDown();	

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::HandleMarioGetInjured()
{
	if (untouchable == 0)
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

float CMario::GetCenter()
{
	if (level != MARIO_LEVEL_SMALL)
		return x + MARIO_BIG_BBOX_WIDTH;
	else return x + MARIO_SMALL_BBOX_WIDTH;
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
		if (e->ny < 0) 
			isOnPlatform = true;
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
	else if (dynamic_cast<CPSwitch*>(e->obj))
		OnCollisionWithPSwitch(e);
	else if (dynamic_cast<CDeadPlatform*>(e->obj))
		OnCollisionWithDeadPlatform(e);
}

void CMario::HandleMarioUntouchable()
{
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
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
			if (brick->GetType() == BRICK_TYPE_QUESTION)
			{
				brick->SetState(BRICK_STATE_BOUNCING);
			}
			if (brick->GetType() == BRICK_TYPE_NORMAL)
				if (brick->GetItemType() != BRICK_ITEM_TYPE_PSWITCH)
					brick->Delete();
				else
				{
					brick->SetType(BRICK_TYPE_DISABLE);
					brick->RevealItem();
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
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (untouchable == 0)
			{
				HandleMarioGetInjured();
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
		CoinUp();
		coin->Delete();

	}
}


void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = dynamic_cast<CPortal*>(e->obj);
		if (e->ny != 0)
		{
			if (e->ny < 0 && (CGame::GetInstance()->IsKeyDown(DIK_DOWN)))
			{
				StartPipeDown();
			}
			if (e->ny > 0 && (CGame::GetInstance()->IsKeyDown(DIK_UP)))
			{
				StartPipeUp();
			}
		}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (e->ny < 0)
	{
		// jump on top >> make koopa a shell
		if (koopa->GetState() != SHELL_STATE_IDLING)
		{
			if (koopa->GetState() == KOOPA_STATE_WALKING_LEFT || koopa->GetState() == KOOPA_STATE_WALKING_RIGHT)
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->setLastState(koopa->GetState());
				koopa->SetState(SHELL_STATE_IDLING);
			}
		}
	}
	else // hit by koopa
	{
		if (koopa->GetState() != SHELL_STATE_IDLING)
		{
			HandleMarioGetInjured();
		}
	}

	// handle the shell
	if (e->nx != 0)
	{
		if (koopa->GetState() == SHELL_STATE_IDLING)
		{
			//Pick the shell
				if ((state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT))
				{	
					isHolding = true;
					koopa->isBeingHeld = true;
				}
				else // KICK THE SHELL
				{
					if (e->nx > 0)
						koopa->SetState(SHELL_STATE_ROLLING_LEFT);
					else koopa->SetState(SHELL_STATE_ROLLING_RIGHT);
					SetState(MARIO_STATE_KICK);
					isHolding = false;
					koopa->isBeingHeld = false;
					koopa->isVulnerable = false;
				}
		}
	}
	else // hit by SHELL
	{
		if (koopa->GetState() != SHELL_STATE_IDLING)
		{
			HandleMarioGetInjured();
		}
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	
	// Eat mushroom
	if (mushroom->GetState() == MUSHROOM_STATE_MOVING)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			if (mushroom->GetType() == MUSHROOM_TYPE_RED)
				SetLevel(MARIO_LEVEL_BIG);
			else LifeUp();
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

void CMario::OnCollisionWithPSwitch(LPCOLLISIONEVENT e)
{
	CPSwitch* pSwitch = dynamic_cast<CPSwitch*>(e->obj);
	if (e->ny < 0)
		if (pSwitch->GetState() != PSWITCH_STATE_PRESSED)
		{
			pSwitch->SetState(PSWITCH_STATE_PRESSED);
			pSwitch->timeoutToBrick = GetTickCount64();
		}
}

void CMario::OnCollisionWithDeadPlatform(LPCOLLISIONEVENT e)
{
	CDeadPlatform* dPlatform = dynamic_cast<CDeadPlatform*>(e->obj);
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (isPipeDown || isPipeUp)
		aniId = ID_ANI_MARIO_RACCOON_ENTERING_PIPE;
	else
		if (!isOnPlatform)
		{
			if (nx >= 0)
			{
				if (abs(ax) == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
				else 
					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
				if(isHolding)
					aniId = ID_ANI_MARIO_SMALL_HOLDING_JUMP_RIGHT;
			}
			else
			{
				if (abs(ax) == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
				else
					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
				if (isHolding)
					aniId = ID_ANI_MARIO_SMALL_HOLDING_JUMP_LEFT;
			}
		}
		else
			if (vx == 0)
			{
				if (nx > 0)
				{
					aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
					if (isHolding)
					{
						aniId = ID_ANI_MARIO_SMALL_HOLDING_IDLE_RIGHT;
					}
				}
				else
				{
					aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
					if (isHolding)
					{
						aniId = ID_ANI_MARIO_SMALL_HOLDING_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
					if (isHolding)
						aniId = ID_ANI_MARIO_SMALL_HOLDING_RUN_RIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else  if (vx < 0)
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
					if (isHolding)
						aniId = ID_ANI_MARIO_SMALL_HOLDING_RUN_LEFT;
				}
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
	if (isPipeDown || isPipeUp)
		aniId = ID_ANI_MARIO_RACCOON_ENTERING_PIPE;
	else
		if (!isOnPlatform)
		{
			if (nx >= 0)
			{
				if (abs(ax) == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
				if (isHolding)
					aniId = ID_ANI_MARIO_RACCOON_HOLDING_JUMP_RIGHT;
				if (isFlapping )
					aniId = ID_ANI_MARIO_RACCOON_FLAPPING_RIGHT;
				if (isFlying)
					aniId = ID_ANI_MARIO_RACCOON_FLYING_RIGHT;
			}
			else
			{
				if (abs(ax) == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
				else
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
				if (isHolding)
					aniId = ID_ANI_MARIO_RACCOON_HOLDING_JUMP_LEFT;
				if (isFlapping)
					aniId = ID_ANI_MARIO_RACCOON_FLAPPING_LEFT;
				if (isFlying)
					aniId = ID_ANI_MARIO_RACCOON_FLYING_LEFT;
			}
		}
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
						if (nx > 0)
						{
							aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
							if (isHolding)
								aniId = ID_ANI_MARIO_RACCOON_HOLDING_IDLE_RIGHT;
							if (isWhipping)
								aniId = ID_ANI_MARIO_RACCOON_WHIPPING_RIGHT;
						}
						else
						{
							aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
							if (isHolding)
								aniId = ID_ANI_MARIO_RACCOON_HOLDING_IDLE_LEFT;
							if (isWhipping)
								aniId = ID_ANI_MARIO_RACCOON_WHIPPING_LEFT;
						}
					}
					else if (vx > 0)
					{
						if (ax < 0)
							aniId = ID_ANI_MARIO_RACCOON_BRAKING_RIGHT;
						else if (ax == MARIO_ACCEL_RUN_X)
						{
							aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
							if (isHolding)
								aniId = ID_ANI_MARIO_RACCOON_HOLDING_RUN_RIGHT;
						}
						else if (ax == MARIO_ACCEL_WALK_X)
							aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
						if (isWhipping)
							aniId = ID_ANI_MARIO_RACCOON_WHIPPING_RIGHT;
					}
					else  if (vx < 0)
					{
						if (ax > 0)
							aniId = ID_ANI_MARIO_RACCOON_BRAKING_LEFT;
						else if (ax == -MARIO_ACCEL_RUN_X)
						{
							aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
							if (isHolding)
								aniId = ID_ANI_MARIO_RACCOON_HOLDING_RUN_LEFT;
						}
						else if (ax == -MARIO_ACCEL_WALK_X)
							aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
						if (isWhipping)
							aniId = ID_ANI_MARIO_RACCOON_WHIPPING_LEFT;
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
	if (isPipeDown||isPipeUp)
		aniId = ID_ANI_MARIO_ENTERING_PIPE;
	else
		if (!isOnPlatform)
		{
			if (nx >= 0)
			{
				if (abs(ax) == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
				if (isHolding)
					aniId = ID_ANI_MARIO_HOLDING_JUMP_RIGHT;
			}
			else
			{
				if (abs(ax) == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
				else
					aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
				if (isHolding)
					aniId = ID_ANI_MARIO_HOLDING_JUMP_LEFT;
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
					if (nx > 0)
					{
						aniId = ID_ANI_MARIO_IDLE_RIGHT;
						if (isHolding) 
						{
							aniId = ID_ANI_MARIO_HOLDING_IDLE_RIGHT;
						}
					}
					else
					{
						aniId = ID_ANI_MARIO_IDLE_LEFT;
						if (isHolding)
						{
							aniId = ID_ANI_MARIO_HOLDING_IDLE_LEFT;
						}
					}
				}
				else if (vx > 0)
				{
					if (ax < 0)
						aniId = ID_ANI_MARIO_BRAKING_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
					{
						aniId = ID_ANI_MARIO_RUNNING_RIGHT;
						if (isHolding)
							aniId = ID_ANI_MARIO_HOLDING_RUN_RIGHT;
					}
					else if (ax == MARIO_ACCEL_WALK_X)
						aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
				else  if (vx < 0)
				{
					if (ax > 0)
						aniId = ID_ANI_MARIO_BRAKING_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X)
					{
						aniId = ID_ANI_MARIO_RUNNING_LEFT;
						if (isHolding)
							aniId = ID_ANI_MARIO_HOLDING_RUN_LEFT;
					}
					else if (ax == -MARIO_ACCEL_WALK_X)
						aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
	
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

	if (level != MARIO_LEVEL_RACCOON)
		animations->Get(aniId)->Render(x, y);
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (!isWhipping )
		{
			if(nx > 0)
				animations->Get(aniId)->Render(x - 4, y);
			else if (nx < 0)
				animations->Get(aniId)->Render(x + 4, y);
		}
		else
			if (nx > 0)
				animations->Get(aniId)->Render(x + 4, y);
			else if (nx < 0)
				animations->Get(aniId)->Render(x - 4, y);
	}

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
		isRunning = true;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isRunning = true;
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
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 4;
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
		if (isSitting && level != MARIO_LEVEL_SMALL)
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
	case MARIO_STATE_KICK:
		isKicking = true;
		kicking_start = GetTickCount64();
		break;
	case MARIO_STATE_WHIPE:
		isWhipping = true;
		whipping_start = GetTickCount64();
		break;
	case MARIO_STATE_FLAPPING:
		isFlapping = true;
		flapping_start = GetTickCount64();
		if (nx > 0)
			maxVx = MARIO_SPEED_FALL_SLOW_X;
		else maxVx = -MARIO_SPEED_FALL_SLOW_X;
		vy = MARIO_SPEED_FALL_SLOW_Y;
		break;
	case MARIO_STATE_FLYING:
		isFlying = true;
		flying_start = GetTickCount64();
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		else
			vy = -MARIO_FLY_MAX_STACK_SPEED_Y;
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

void CMario::HandleMarioStackSpeed()
{
	int resetStack = 0;
	float stack = MARIO_RUNNING_SPEED / MARIO_RUNNING_STACKS;

	if (speedStack == MARIO_RUNNING_STACKS)
		speedStack = MARIO_RUNNING_STACKS;
	else	speedStack = (int)(abs)(vx / stack);

}

void CMario::HandleMarioFallingDown()
{
	if (GetTickCount64() - flapping_start > MARIO_FLAPPING_TIME && isFlapping)
	{
		flapping_start = 0;
		isFlapping = false;
	}
}


void CMario::HandleMarioFlying()
{
	if (GetTickCount64() - flying_start > MARIO_FLYING_TIME && isFlying)
	{
		flying_start = 0;
		isFlying = false;
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

void CMario::HandleMarioKicking()
{
	if (GetTickCount64() - kicking_start > MARIO_KICKING_TIME && kicking_start)
	{
		isKicking = false;
		kicking_start = 0;
	}

}

void CMario::HandleMarioEnterPipe()
{
	if (GetTickCount64() - pipeDown_start < MARIO_PIPE_TIME && isPipeDown)
	{
		vy = 0.03f;
	}
	if (GetTickCount64() - pipeDown_start >= MARIO_PIPE_TIME && isPipeDown)
	{
		if (currentZone == 1)
		{
			SwitchZone();
			StartPipeDown();
		}
		else if (currentZone == 2)
		{
			StopPipeDown();
		}
	}
	if (GetTickCount64() - pipeUp_start < MARIO_PIPE_TIME && isPipeUp)
	{
		vy = -0.03f;
	}
	if (GetTickCount64() - pipeUp_start >= MARIO_PIPE_TIME && isPipeUp)
	{
		if (currentZone == 2)
		{
			SwitchZone();
			StartPipeUp();
		}
		else if (currentZone == 1)
		{
			StopPipeUp();
		}
	}
}

void CMario::HandleMarioWhippingTail()
{
	/*if (tail == NULL && isWhipping)
	{

		if (nx > 0)
		{
			tail = new CTail(x - TAIL_BBOX_WIDTH + 2, y);
			tail->SetPosition(x - TAIL_BBOX_WIDTH + 2,y);
			if (tail != NULL)
				DebugOut(L"tail init left to right\n");
		}
		else
		{
			tail = new CTail(x + MARIO_BIG_BBOX_WIDTH - 2, y);
			tail->SetPosition(x + MARIO_BIG_BBOX_WIDTH - 2,y);
			if (tail != NULL)
				DebugOut(L"tail init right to left\n");
		}
		((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->GetObjects().push_back(tail);
		((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->LoadObject(tail);
	}*/

	if (GetTickCount64() - whipping_start > MARIO_WHIPING_TIME && whipping_start)
	{
		isWhipping = false;
		whipping_start = 0;
	}
}



