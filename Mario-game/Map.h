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
	int** mapTiles_Graphics;
	int** mapTiles_Shadings;
	int** mapTiles_Backgrounds;
	/*vector<vector<int>> mapTiles_Graphics;
	vector<vector<int>> mapTiles_Shadings;
	vector<vector<int>> mapTiles_Backgrounds;*/

public:

	CMaps();
	~CMaps();
	void LoadResourses(int mapId, LPCWSTR pathImg, int maxCol, int maxRow, 
		LPCWSTR backgroundPathTxt, LPCWSTR graphicPathTxt, LPCWSTR shadingPathTxt);
	void LoadGraphics(LPCWSTR pathTxt);
	void LoadShadings(LPCWSTR pathTxt);
	void LoadBackgrounds(LPCWSTR pathTxt);
	void RenderGraphic();
	void RenderShading();
	void RenderBackground();
	float GetWidthMap();
	float GetHeightMap();
	static CMaps* GetInstance();
};
