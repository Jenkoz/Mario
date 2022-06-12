#include "HUD.h"
#include "PlayScene.h"
#include "Camera.h"
#include "Mario.h"


CHUD::CHUD(float x, float y) : CGameObject(x, y)
{

}

void CHUD::Render()
{

	float xx = x - 20.0f;
	float yy = y - 4.0f;
	CAnimations::GetInstance()->Get(ID_ANI_HUD_MAIN)->Render(x, y);
		CAnimations::GetInstance()->Get(ID_ANI_HUD_POWER)->Render(x + 100, y);
		CAnimations::GetInstance()->Get(ID_ANI_HUD_POWER)->Render(x + 124, y);
		CAnimations::GetInstance()->Get(ID_ANI_HUD_POWER)->Render(x + 148, y);
	for (int i = 0; i < MARIO_RUNNING_STACKS + 1; i++)
	{
		if (i < MARIO_RUNNING_STACKS)
			CAnimations::GetInstance()->Get(ID_ANI_STACK_OFF)->Render(xx + 9 * i, yy);
		else 
			if (speedStack < MARIO_RUNNING_STACKS)
				CAnimations::GetInstance()->Get(ID_ANI_POWER_OFF)->Render(xx + 9 * i + 4, yy);
			else
				CAnimations::GetInstance()->Get(ID_ANI_POWER_ON)->Render(xx + 9 * i + 4, yy);
	}
	for (int i = 0; i < speedStack; i++)
	{
		if (i < MARIO_RUNNING_STACKS)
			CAnimations::GetInstance()->Get(ID_ANI_STACK_ON)->Render(xx + 9 * i, yy);
	}
}

void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CCamera::GetInstance()->GetCamPos(cx, cy);
	
	x = cx + (HUD_BBOX_WIDTH/2) + 16;
	y = cy + (CGame::GetInstance()->GetBackBufferHeight() - (HUD_BBOX_HEIGHT)/2) - 4;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	speedStack = mario->GetSpeedStack();
}

void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - HUD_BBOX_WIDTH /2;
	top = y - HUD_BBOX_HEIGHT /2;
	right = left + HUD_BBOX_WIDTH ;
	bottom = top + HUD_BBOX_HEIGHT ;
}

