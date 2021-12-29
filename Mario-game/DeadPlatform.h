#pragma once

#include "Platform.h"

class CDeadPlatform : public CPlatform
{

public:
	CDeadPlatform(float x, float y,
		float cell_width, float cell_height, int length, int depth, int type,
		int sprite_id) : CPlatform(x, y,
			cell_width, cell_height, length, depth, type,
			sprite_id) {}
	//void GetBoundingBox(float& l, float& t, float& r, float& b);
};

