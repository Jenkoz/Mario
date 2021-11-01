#include "Camera.h"
#include "Mario.h"
#include "Map.h"
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
RECT CCamera::GetBound()
{
	RECT bound;

	bound.left = cam_x;
	bound.top = cam_y;
	bound.right = cam_x + width;;
	bound.bottom = cam_y + height;
	return bound;
}

void CCamera::Update(float cx, float cy)
{
	/*DebugOut(L" %f %f \n", cx, cy);*/
	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;
	if (cx < 0) cx = 0;
	//DebugOut(L" %f %f \n", cx, cy);
	float mapEnd = CMaps::GetInstance()->GetWidthMap();
	if (cx >= mapEnd - game->GetBackBufferWidth() / 2) cx = mapEnd - game->GetBackBufferWidth() / 2;
	
	SetCamPos(cx, 256.f/*cy*/);
	//DebugOut(L" %f %f \n", cx, cy);
}


CCamera::~CCamera()
{
}