#include "ColorBrickHitBox.h"

ColorBrickHitBox::ColorBrickHitBox()
{
}

ColorBrickHitBox::ColorBrickHitBox(int width, int height, float x, float y)
{
	this->width = width;
	this->height = height;
	
	this->x = x;
	this->y = y;

	//invisible = 0;
}

void ColorBrickHitBox::Render()
{
	//RenderBoundingBox();
}

void ColorBrickHitBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
