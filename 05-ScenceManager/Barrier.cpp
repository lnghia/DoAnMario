#include "Barrier.h"

void Barrier::Render()
{
	//RenderBoundingBox();
}

void Barrier::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void Barrier::SetId(int val)
{
	id = val;
}

int Barrier::GetId()
{
	return id;
}
