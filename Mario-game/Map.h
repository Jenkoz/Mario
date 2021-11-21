#pragma once
#include "Sprites.h"
#include "GameGlobal.h"
#include "Camera.h"
#include "GameGlobal.h"
#include <vector>
#include <fstream>



#define ID_TEXTURE_MAP_1 40
#define TILE_WIDTH 16
#define TILE_HEIGHT 16


class CMaps
{
private:
	CSprites* sprites = CSprites::GetInstance();
	static CMaps* __instance;
	int col;
	int row;
	int maxCol;
	vector<vector<int>> mapTiles;
public:

	CMaps();
	~CMaps();
	void LoadResourses(int mapId, LPCWSTR pathImg, int maxCol, int maxRow, LPCWSTR pathTxt);
	void Render();
	float GetWidthMap();
	float GetHeightMap();
	static CMaps* GetInstance();
};
