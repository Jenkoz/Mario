#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"


#define BRICK_WIDTH 16

#define BRICK_BBOX_WIDTH 15
#define BRICK_BBOX_HEIGHT 15

#define BRICK_BOUNCING_DEFLECT_Y 0.2f

#define BRICK_TYPE_DISABLE 1
#define BRICK_TYPE_NORMAL 2
#define BRICK_TYPE_QUESTION 3

#define BRICK_STATE_BOUNCING 1
#define BRICK_STATE_IDLING 2
#define BRICK_STATE_BREAK 0

#define BRICK_ITEM_TYPE_ULTILITY	1 
#define BRICK_ITEM_TYPE_DCOIN		2
#define BRICK_ITEM_TYPE_COIN		3
#define BRICK_ITEM_TYPE_PSWITCH		4


#define ID_ANI_BRICK_TYPE_DISABLE 10000
#define ID_ANI_BRICK_TYPE_NORMAL 10001
#define ID_ANI_BRICK_TYPE_QUESTION 10002

class CBrick : public CGameObject {
	float start_y;
	int type;
	int itemId;
public:
	BOOLEAN isCoinPBBrick;

	CBrick(float x, float y, int _type, int _itemId);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RevealItem();

	virtual int isBlocking(){ return 1; }
	virtual int IsCollidable() { return 1; }
	float GetEndPlatform() { return (x + BRICK_BBOX_WIDTH); }

	int GetType() { return this->type; }
	int GetItemType() { return this->itemId; }
	void SetType(int _type) { type = _type; }
	void SetState(int state);
};