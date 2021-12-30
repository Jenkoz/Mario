#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	float width;
	float height; 

	int zone;
public:
	CPortal(float l, float t, float r, float b, int scene_id, int zone);
	virtual void Render();
	int GetZone() { return zone; }
	void RenderBoundingBox(void);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return scene_id;  }
	int IsBlocking() { return 0; }
};