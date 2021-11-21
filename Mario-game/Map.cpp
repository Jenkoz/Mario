#include "Map.h"
#include "Textures.h"
#include "Mario.h"
CMaps* CMaps::__instance = NULL;
CMaps::CMaps() {}

int w1id;


void CMaps::LoadResourses(int mapId, LPCWSTR pathTile, int maxCol, int maxRow, LPCWSTR pathTxt) {
	ifstream File;
	this->maxCol = maxCol;
	//Get map-tiles's sprites
	File.open(pathTxt);
	File >> col >> row;
	mapTiles.resize(row);
	for (int r = 0; r < row; ++r) {
		mapTiles[r].resize(col);
		for (int c = 0; c < col; ++c) {
			File >> mapTiles.at(r).at(c);
		}
	}
	File.close();

	w1id = mapId;

	//Load all sprites from pathIMG
	CTextures* tex = CTextures::GetInstance();
	tex->Add(mapId, pathTile);
	LPTEXTURE texMap = tex->Get(mapId);

	int idTile = 1;
	for (int r = 0; r < maxRow; r++)
	{
		for (int c = 0; c < maxCol; c++)
		{
			int left = (TILE_WIDTH + 1) * c;
			int top = (TILE_HEIGHT + 1) * r;
			int right =  (TILE_WIDTH + 1) * (c + 1);
			int bottom = (TILE_HEIGHT + 1) * (r + 1);
			sprites->Add(idTile + mapId, left, top, right, bottom, texMap);

			idTile++;
		}
	}
}
void CMaps::Render() 
{
	float cam_x, cam_y;
	CCamera::GetInstance()->GetCamPos(cam_x, cam_y);
	//DebugOut(L"x = %f, y = %f \n", cam_x, cam_y);
	for (int i = cam_y/TILE_HEIGHT; i < (cam_y + SCREEN_HEIGHT)/TILE_HEIGHT; ++i)
	{
		for (int j = cam_x/TILE_WIDTH; j < (cam_x + SCREEN_WIDTH)/TILE_WIDTH; ++j)
		{
			if (mapTiles[i][j] >= 0)
			{
				float x = j * TILE_WIDTH;
				float y = i * TILE_HEIGHT;
				sprites->Get(mapTiles.at(i).at(j) + w1id + 1)->Draw(x, y);
			}
		}
	}
}


float CMaps::GetWidthMap()
{
	return col * TILE_WIDTH - TILE_WIDTH * 6;
}
float CMaps::GetHeightMap()
{
	return row * TILE_HEIGHT;
}

CMaps* CMaps::GetInstance()
{
	if (__instance == NULL)
		__instance = new CMaps();
	return __instance;
}

CMaps::~CMaps() {
}