#include "PipeHitBox.h"

PipeHitBox::PipeHitBox()
{

}

void PipeHitBox::Render()
{
	RenderBoundingBox();
}

void PipeHitBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

int PipeHitBox::GetHeight()
{
	return height;
}

int PipeHitBox::GetWidth()
{
	return width;
}