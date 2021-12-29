#include "Map.h"
#include "Textures.h"
#include "Mario.h"
CMaps* CMaps::__instance = NULL;
CMaps::CMaps() {}

int w_id;



void CMaps::LoadResourses(int mapId, LPCWSTR pathTile, int maxCol, int maxRow, 
	LPCWSTR backgroundPathTxt, LPCWSTR graphicPathTxt, LPCWSTR shadingPathTxt)
{
	this->maxCol = maxCol;
	LoadBackgrounds(backgroundPathTxt);
	LoadGraphics(graphicPathTxt);
	LoadShadings(shadingPathTxt);

	w_id = mapId;


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
void CMaps::LoadGraphics(LPCWSTR pathTxt)
{
	ifstream File;
	File.open(pathTxt);
	File >> col >> row;
	mapTiles_Graphics = new int*[row];
	for (int r = 0; r < row; ++r)
	{
		mapTiles_Graphics[r] = new int[col];
		for (int c = 0; c < col; ++c)
		{
			File >> mapTiles_Graphics[r][c];
		}
	}
	File.close();
}
void CMaps::LoadShadings(LPCWSTR pathTxt)
{
	ifstream File;
	File.open(pathTxt);
	File >> col >> row;
	mapTiles_Shadings = new int* [row];
	for (int r = 0; r < row; ++r)
	{
		mapTiles_Shadings[r] = new int[col];
		for (int c = 0; c < col; ++c)
		{
			File >> mapTiles_Shadings[r][c];
		}
	}
	File.close();
}
void CMaps::LoadBackgrounds(LPCWSTR pathTxt)
{
	ifstream File;
	File.open(pathTxt);
	File >> col >> row;
	mapTiles_Backgrounds = new int* [row];
	for (int r = 0; r < row; ++r)
	{
		mapTiles_Backgrounds[r] = new int[col];
		for (int c = 0; c < col; ++c)
		{
			File >> mapTiles_Backgrounds[r][c];
		}
	}
	File.close();
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
			// render background
			if (mapTiles_Backgrounds[i][j] != -1)
			{
				float x = j * TILE_WIDTH;
				float y = i * TILE_HEIGHT;
				sprites->Get(mapTiles_Backgrounds[i][j] + w_id + 1)->Draw(x, y);
			}
			// render graphics
			if (mapTiles_Graphics[i][j] != -1)
			{
				float x = j * TILE_WIDTH;
				float y = i * TILE_HEIGHT;
				sprites->Get(mapTiles_Graphics[i][j] + w_id + 1)->Draw(x, y);
			}
			// render shadings
			if (mapTiles_Shadings[i][j] != -1)
			{
				float x = j * TILE_WIDTH;
				float y = i * TILE_HEIGHT;
				sprites->Get(mapTiles_Shadings[i][j] + w_id + 1)->Draw(x, y);
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