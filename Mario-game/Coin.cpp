#include "Coin.h"
#include "Brick.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "debug.h"

void CCoin::ChangeCoinToBrick()
{
	CGameObject* obj = NULL;
	vector<LPGAMEOBJECT> coObjects = ((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->GetObjects();
	if (GetTickCount64() - reverseTimeOut > COIN_TIMEOUT_COINTOBRICK && reverseTimeOut)
	{
		this->Delete(); 
		reverseTimeOut = 0;
		obj = new CBrick(x, y, BRICK_TYPE_NORMAL, BRICK_ITEM_TYPE_COIN);
		obj->SetPosition(x, y);
		((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->GetObjects().push_back(obj);
		((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->LoadObject(obj);
	}
}

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (!wasBBrick)
		animations->Get(ID_ANI_COIN)->Render(x, y);
	else animations->Get(ID_ANI_COIN_BR)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (wasBBrick)
		ChangeCoinToBrick();

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}