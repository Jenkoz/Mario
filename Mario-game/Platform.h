#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected: 
	int type;				// type of platform
	int length;				// Unit: cell 
	int depth;
	float cellWidth;
	float cellHeight;
	int spriteId;
public: 
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length, int depth, int type,
		int sprite_id) :CGameObject(x, y)
	{
		this->length = length;
		this->depth = depth;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->type = type;
		this->spriteId = sprite_id;
	}
	virtual int isBlocking() { return 1; }
	virtual float GetEndPlatform() { return (x + length * cellWidth); }
	virtual float GetX() { return x; }
	virtual void Render();
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void RenderBoundingBox();
};

typedef CPlatform* LPPLATFORM;