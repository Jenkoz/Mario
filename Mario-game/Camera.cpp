#include "Camera.h"
#include "Mario.h"
#include "Map.h"
#include "HUD.h"
#include "PlayScene.h"
CCamera* CCamera::__instance = NULL;

CCamera* CCamera::GetInstance()
{
	if (__instance == NULL)
		__instance = new CCamera();
	return __instance;
}

CCamera::CCamera()
{
	cam_x = 0;
	cam_y = 0;
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT;
}

void CCamera::Update()
{
	CMario* player = ((CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	float cx, cy;
	player->GetPosition(cx, cy);
	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;
	if (cx < 0) cx = 0;
	float mapEnd = CMaps::GetInstance()->GetWidthMap();
	if (cx + game->GetBackBufferWidth() >= mapEnd) 
		cx = mapEnd - game->GetBackBufferWidth();
	if (cy < MAX_SKYZONE_HEIGHT)
		cy = MAX_SKYZONE_HEIGHT;

	if (player->GetY() < CAM_Y_POSITION_TERRAIN_ZONE + 20)
		SetCamPos(cx, cy + HUD_BBOX_HEIGHT / 2);
	else if (player->GetCurrentZone() == MARIO_IN_TERRAIN_ZONE)
		SetCamPos(cx, CAM_Y_POSITION_TERRAIN_ZONE + HUD_BBOX_HEIGHT / 2 );
	else if (player->GetCurrentZone() == MARIO_IN_OTHER_ZONE)
		SetCamPos(cx, CAM_Y_POSITION_MONEY_ZONE + HUD_BBOX_HEIGHT / 2 );

}



CCamera::~CCamera()
{
}