#pragma once
#include "InteractivableTransObject.h"

class PipeHitBox : public InteractivableTransObject
{
public:
	PipeHitBox();
	PipeHitBox(int width, int height) :InteractivableTransObject(width, height) {
		canBeStandOn = true;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int GetWidth();
	int GetHeight();
};

