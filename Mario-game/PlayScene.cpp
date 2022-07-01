#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Camera.h"

// PLAYER
#include "Mario.h"

// OBJECTS
#include "Goomba.h"
#include "Koopa.h"
#include "Coin.h"
#include "DCoin.h"
#include "Mushroom.h"
#include "Platform.h"
#include "ColourPlatform.h"
#include "DeadPlatform.h"
#include "Portal.h"
#include "HUD.h"
#include "VenusTrap.h"
#include "PiranhaPlant.h"


// MAP
#include "Map.h"
// KEY
#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAPS	7

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2


#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; (float)i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;
	CGameObject *pipeObj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		int sceneID = atoi(tokens[3].c_str());
		obj = new CMario(x,y,sceneID); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GOOMBA: 
	{
		int lvl_goomba = atoi(tokens[3].c_str());
		obj = new CGoomba(x, y, lvl_goomba); break;
	}
	case OBJECT_TYPE_KOOPA:
	{
		int type_koopa = atoi(tokens[3].c_str());
		obj = new CKoopa(x, y, type_koopa); break;
	}
	case OBJECT_TYPE_PIRANHA_PLANT:
	{
		pipeObj = new CPiranhaPlant(x, y);
		break;
	}
	case OBJECT_TYPE_VENUS_TRAP:
	{
		int type_venusTrap = atoi(tokens[3].c_str());
		pipeObj = new CVenusTrap(x, y, type_venusTrap);
		break;
	}
	case OBJECT_TYPE_HUD: 
	{
		if (hud != NULL)
		{
			DebugOut(L"[ERROR] HUD object was created before!\n");
			return;
		}
		obj = new CHUD(x, y); 
		hud = (CHUD*)obj;
		break;
	}
	case OBJECT_TYPE_BRICK: 
	{
		int _type = atoi(tokens[3].c_str());
		int _itemId = atoi(tokens[4].c_str());
		obj = new CBrick(x, y, _type, _itemId); 
		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int depth = atoi(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		int sprite_id = atoi(tokens[8].c_str());

		if (type == 1 )
			obj = new CPlatform(
				x, y,
				cell_width, cell_height, length, depth, type,
				sprite_id
			);
		else if (type == 2)
			obj = new CColourPlatform(
				x, y,
				cell_width, cell_height, length, depth, type,
				sprite_id
			);
		else if (type == 3)
			obj = new CDeadPlatform(
				x, y,
				cell_width, cell_height, length, depth, type,
				sprite_id
			);

		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		int zone = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id, zone);
		break;
	}


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (obj)
	{
		obj->SetPosition(x, y);
		objects.push_back(obj);
	}
	else
	{
		pipeObj->SetPosition(x, y);
		pipeObjects.push_back(pipeObj);
	}
}

void CPlayScene::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return;

	int mapId = atoi(tokens[0].c_str());
	wstring path_img = ToWSTR(tokens[1]);
	int maxCol = atoi(tokens[2].c_str());
	int maxRow = atoi(tokens[3].c_str());
	wstring background_path_txt = ToWSTR(tokens[4]);
	wstring graphic_path_txt = ToWSTR(tokens[5]);
	wstring shading_path_txt = ToWSTR(tokens[6]);

	CMaps::GetInstance()->LoadResourses(mapId, path_img.c_str(), maxCol, maxRow, 
		background_path_txt.c_str(), graphic_path_txt.c_str(), shading_path_txt.c_str());
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[MAP]") { section = SCENE_SECTION_MAPS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAPS: _ParseSection_MAPS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] != player)
			coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < pipeObjects.size(); i++)
	{
		coObjects.push_back(pipeObjects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < pipeObjects.size(); i++)
	{
		pipeObjects[i]->Update(dt, &coObjects);
	}

	/*if (hud != NULL)
		hud->Update(dt, &coObjects);*/

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	CCamera::GetInstance()->Update();
	if (hud != NULL)
		hud->Update(dt, &coObjects);
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	CMaps::GetInstance()->RenderBackground();
	CMaps::GetInstance()->RenderShading();
	for (int i = 0; (float)i < pipeObjects.size(); i++)
		pipeObjects[i]->Render();
	CMaps::GetInstance()->RenderGraphic();
	for (int i = 0; (float)i < objects.size(); i++)
		objects[i]->Render();

}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
	for (it = pipeObjects.begin(); it != pipeObjects.end(); it++)
	{
		delete (*it);
	}
	pipeObjects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; (float)i < objects.size(); i++)
		delete objects[i];
	for (int i = 0; (float)i < pipeObjects.size(); i++)
		delete pipeObjects[i];

	pipeObjects.clear();
	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

void CPlayScene::LoadObject(LPGAMEOBJECT obj)
{
	objects.push_back(obj);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}
	for (it = pipeObjects.begin(); it != pipeObjects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
	pipeObjects.erase(
		std::remove_if(pipeObjects.begin(), pipeObjects.end(), CPlayScene::IsGameObjectDeleted),
		pipeObjects.end());
}