#include "Ground.h"

Ground::Ground()
{
	interactivable = 1;
	canBeStandOn = 1;
	isActive = 1;
	invisible = 0;
}

Ground::Ground(int width, int height)
{
	this->width = width;
	this->height = height;

	interactivable = 1;
	canBeStandOn = 1;
	isActive = 1;
	invisible = 0;
}

void Ground::Render()
{
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + width;
	b = t + height;
}
