#include "Camera.h"
#include "Mario.h"
#include "Map.h"
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

	/*if (cy >= CAM_Y_POSITION_MONEY_ZONE)
		player->SetPlayerIntoMoneyZone();
	else if (cy >= CAM_Y_POSITION_TERRAIN_ZONE)
		player->SetPlayesrOnTerrainZone();
	if (player->isOnTerrainZone)
		cy = CAM_Y_POSITION_TERRAIN_ZONE;
	else if (player->isIntoMoneyZone)
		cy = CAM_Y_POSITION_MONEY_ZONE;*/
	SetCamPos(cx, 304);
}


CCamera::~CCamera()
{
}