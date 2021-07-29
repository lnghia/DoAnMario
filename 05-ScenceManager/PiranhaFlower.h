#pragma once
#include "PiranhaPlant.h"

#define SHORT_GREEN_FLOWER_BBOX_HEIGHT	24

#define PIRANHAFLOWER_ANI_DIE	1

#define PIRANHAFLOWER_STATE_DIE	8

class PiranhaFlower : public PiranhaPlant
{
public:
	PiranhaFlower() {};
	PiranhaFlower(float pipeX, float pipeY, float pipeWidth, float pipeHeight, LPGAMEOBJECT player, int level = 0);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

