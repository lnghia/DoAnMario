#pragma once
#include "InteractivableTransObject.h"

class PipeHitBox : public InteractivableTransObject
{
public:
	PipeHitBox();
	PipeHitBox(int width, int height) :InteractivableTransObject(width, height) {
		canBeStandOn = true;
	}
};

