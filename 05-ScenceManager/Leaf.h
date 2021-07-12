#pragma once
#include "GameObject.h"
#include "Game.h"
#include "HiddenItem.h"
#include "Point.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 14

#define LEAF_FALL_RIGHT	0
#define LEAF_FALL_LEFT	1
#define LEAF_FALL_SPEED_VX	0.07f 
#define LEAF_FALL_SPEED_VY	0.02f
#define LEAF_RISING_SPEED_VY	1.0f

#define LEAF_RISING_TIME 800


#define LEAF_STATE_FALLING	100

#define LEAF_POINT		200


class Leaf : public HiddenItem
{
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	float xMax = 0;
	float xMin = 0;
	DWORD rise_start;
	int rising = 0;

	int xOrbit = 0;

	float yOrbit = 0;
	float topY = 0;
	bool topYFirstTimeUpdated = 0;
public:
	Leaf(float x, float y) :HiddenItem(x, y) {
		nx = 1;

		vx = LEAF_FALL_SPEED_VX;
		vy = LEAF_FALL_SPEED_VY;
		xMax = x + LEAF_BBOX_WIDTH * 2;
		xMin = x - 1;
		StartRising();
	};

	void StartRising() { rising = 1; rise_start = (DWORD)GetTickCount64(); }
	void SetState(int state);

	float FallingForward(int tmp);
	float FallingBackward(int tmp);
};