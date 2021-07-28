#include "FloatingCoin.h"

#define COIN_BBOX_HEIGHT 16
#define COIN_BBOX_WIDTH	16

FloatingCoin::FloatingCoin(float x, float y)
{
	this->x = x;
	this->y = y;

	vx = 0;
	vy = 0;

	renderPriority = 999;

	interactivable = 1;
	invisible = 0;
	isActive = 1;
}

void FloatingCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void FloatingCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void FloatingCoin::Render()
{
	animation_set->at(0)->Render(x, y);
}

void FloatingCoin::GetObsorbed()
{
	interactivable = 0;
	invisible = 1;
	isActive = 0;
}
