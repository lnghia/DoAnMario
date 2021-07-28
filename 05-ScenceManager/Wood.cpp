#include "Wood.h"

Wood::Wood()
{
	canBeStandOn = 1;
	isActive = 1;
	interactivable = 1;
}

void Wood::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void Wood::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WOOD_BBOX_WIDTH;
	b = y + WOOD_BBOX_HEIGHT;
}