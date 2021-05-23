#include "InteractivableTransObject.h"

InteractivableTransObject::InteractivableTransObject()
{
	invisible = 1;
}

InteractivableTransObject::InteractivableTransObject(int width, int height)
{
	invisible = 1;
	this->width = width;
	this->height = height;
}

void InteractivableTransObject::Render()
{
}

void InteractivableTransObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void InteractivableTransObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void InteractivableTransObject::SetState(int state)
{
}
