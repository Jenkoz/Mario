#include "WorldMapSceneKeyHandler.h"
#include "WorldMapScene.h"
#include "Mario.h"
#include "Portal.h"


void CWorldMapSceneKeyHandler::KeyState(BYTE* states)
{
}

void CWorldMapSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CMario* mario = (CMario*)((LPWORLDMAP)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN: {
		mario->MoveDown();
		break;
	}
	case DIK_UP: {
		mario->MoveUp();
		break;
	}
	case DIK_LEFT: {
		mario->MoveLeft();
		break;
	}
	case DIK_RIGHT: {
		mario->MoveRight();
		break;
	}
	case DIK_S: {
		CGame::GetInstance()->InitiateSwitchScene(PLAY_SCENE_1_1);
		CWorldMapScene* WorldMapScene = (LPWORLDMAP)CGame::GetInstance()->GetCurrentScene();
		WorldMapScene->Clear();
		break;
	}
	default:
		break;
	}
}

void CWorldMapSceneKeyHandler::OnKeyUp(int KeyCode)
{
}