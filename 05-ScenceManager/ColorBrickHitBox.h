#pragma once
#include "GameObject.h"
#include "InteractivableTransObject.h"

class ColorBrickHitBox : public InteractivableTransObject
{
public:
	ColorBrickHitBox();
	ColorBrickHitBox(int width, int height) :InteractivableTransObject(width, height) {
		canBeStandOn = true;
	}
	ColorBrickHitBox(int width, int height, float x, float y);
};

