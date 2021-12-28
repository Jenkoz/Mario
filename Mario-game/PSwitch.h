#pragma once
#include "GameObject.h"

#define PSWITCH_BBOX_WIDTH			14
#define PSWITCH_BBOX_HEIGHT			14
#define PSWITCH_BBOX_PRESSED_HEIGHT	7

#define ID_ANI_PSWITCH_IDLE		17070
#define ID_ANI_PSWTICH_PRESSED	17071


#define PSWITCH_STATE_IDLE		0
#define PSWITCH_STATE_RISING	1
#define PSWITCH_STATE_PRESSED	2

#define PSWITCH_TIMEOUT_COINTOBRICK 6000

class CPSwitch :public CGameObject
{
	float y_start;
	BOOLEAN isAppear;
public:
	ULONGLONG timeoutToBrick;
	BOOLEAN isPressed;
	CPSwitch(float x, float y);

	virtual void Render();

	bool GetIsAppear() { return isAppear; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void SetState(int state);

	void ChangeBrickToCoin();
	void ChangeCoinToBrick();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
};