#pragma once
#include "Map.h"
#include "GameGlobal.h"
class CCamera
{
private:
	static CCamera* __instance;

	float cam_x; // cam_x
	float cam_y; // cam_y
	int width; // width screen
	int height; // height screen
public:

	CCamera();
	~CCamera();

	void SetCamPos(float x, float y) { this->cam_x = x; this->cam_y = y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }


	int GetHeight() { return height; }
	int GetWidht() { return width; }
	void Update();

	static CCamera* GetInstance();

};
