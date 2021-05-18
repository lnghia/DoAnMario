#include "JustForShow.h"

JustForShow::JustForShow()
{
	interactivable = 0;
}

void JustForShow::Render()
{
	animation_set->at(0)->Render(x, y);
}

void JustForShow::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + JUST_FOR_SHOW_BBOX_WIDTH;
	b = y + JUST_FOR_SHOW_BBOX_HEIGHT;
}
