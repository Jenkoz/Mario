#include "HUD.h"
#include "PlayScene.h"
#include "Camera.h"
#include "Mario.h"


void CHUD::Render() 
{
	CAnimations::GetInstance()->Get(ID_ANI_HUD_MAIN)->Render(x, y);
}

void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCamera::GetInstance()->GetCamPos(x, y);
	x += CGame::GetInstance()->GetBackBufferHeight()/2;
	y += float(CGame::GetInstance()->GetBackBufferHeight() - HUD_BBOX_HEIGHT/2);
}

void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - HUD_BBOX_WIDTH / 2;
	top = y - HUD_BBOX_HEIGHT / 2;
	right = left + HUD_BBOX_WIDTH / 2;
	bottom = top + HUD_BBOX_HEIGHT / 2;
}

