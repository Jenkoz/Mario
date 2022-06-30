#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"

class CWorldMapScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;
	LPGAMEOBJECT hud;
	vector<LPGAMEOBJECT> objects;
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	CWorldMapScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	vector<LPGAMEOBJECT> GetObjects() { return this->objects; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CWorldMapScene* LPWORLDMAP;
