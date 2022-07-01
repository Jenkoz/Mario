#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include "Camera.h"

void CPlatform::Render()
{
	if (this->length <= 0 || this->depth <= 0) return; 
	float xx = x; 
	float yy = y;
	CSprites * s = CSprites::GetInstance();
	for (int yi = 0; yi < this->depth - 1; yi++)
	{
		for (int xi = 0; xi < this->length - 1; xi++)
		{
			s->Get(this->spriteId)->Draw(xx, yy);
			if (this->length != 1)
				xx += this->cellWidth;
		}
		if  (this->depth != 1)
			yy += this->cellHeight;
	}

	//
	//RenderBoundingBox();
	//
}
void CPlatform::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CCamera::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;
	float yy = y - this->cellHeight / 2 + rect.bottom / 2;

	CGame::GetInstance()->Draw(xx - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight * this->depth;
}