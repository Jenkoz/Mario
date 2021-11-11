#pragma once

#include "Platform.h"

class CColourPlatform : public CPlatform
{

public:
	CColourPlatform(float x, float y,
		float cell_width, float cell_height, int length, int type,
		int sprite_id) : CPlatform(x, y,
			cell_width, cell_height, length, type,
			sprite_id) {}
	//void GetBoundingBox(float& l, float& t, float& r, float& b);
	void GetFilterBlockLeft(int& l) { l = 0; }
	void GetFilterBlockRight(int& r) { r = 0; }
	void GetFilterBlockBottom(int& b) { b = 0; }
};

