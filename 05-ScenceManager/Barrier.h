#pragma once
#include "GameObject.h"
#include "PipeHitBox.h"

class Barrier : public PipeHitBox
{
	int id;

public:
	bool moveUp = 0;
	bool moveDown = 0;
	bool moveRight = 0;
	bool moveLeft = 0;

	Barrier();
	Barrier(int width, int height) :PipeHitBox(width, height) {
		canBeStandOn = true;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetId(int val);
	int GetId();
};

