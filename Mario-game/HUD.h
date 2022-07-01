#pragma once
#include "GameObject.h"
#include "debug.h"

#define HUD_BBOX_WIDTH 153
#define HUD_BBOX_HEIGHT 25

#define ID_ANI_HUD_MAIN 81000
#define ID_ANI_HUD_POWER 81005
#define ID_ANI_HUD_POWER_MUSHROOM 81006
#define	ID_ANI_HUD_POWER_FLOWER	81007
#define ID_ANI_HUD_POWER_STAR 81008

#define	ID_ANI_STACK_ON 81001
#define	ID_ANI_STACK_OFF 81002
#define	ID_ANI_POWER_ON 81003
#define	ID_ANI_POWER_OFF 81004

#define POWER_NONE 0;
#define POWER_MUSHROOM 1;
#define POWER_FLOWER 2;
#define POWER_STAR 3;

#define POWER_BOX_1_POS 100
#define POWER_BOX_2_POS 124
#define POWER_BOX_3_POS 148



class CHUD : public CGameObject
{
	int speedStack = 0;
public:
	ULONGLONG speedStart_start = 0;
	CHUD(float x, float y);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//void SetState(int state);
	int IsBlocking() { return 0; }
	

};

