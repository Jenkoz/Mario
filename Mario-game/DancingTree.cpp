#include "DancingTree.h"

void CDancingTree::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_DANCINGTREE)->Render(x, y);
}
void CDancingTree::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - DANCINGTREE_BBOX_WIDTH / 2;
	t = y - DANCINGTREE_BBOX_HEIGHT / 2;
	r = l + DANCINGTREE_BBOX_WIDTH;
	b = t + DANCINGTREE_BBOX_HEIGHT;
}
CDancingTree::CDancingTree(float x, float y) : CGameObject(x, y) 
{
}