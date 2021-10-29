#include "ColourBlock.h"
#include "Utils.h"

void CColourBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_CLBLOCK)->Render(x, y);
	//RenderBoundingBox();
}

void CColourBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - CLBLOCK_BBOX_WIDTH / 2;
	t = y - CLBLOCK_BBOX_HEIGHT / 2;
	r = l + CLBLOCK_BBOX_WIDTH;
	b = t + CLBLOCK_BBOX_HEIGHT;
}