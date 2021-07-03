#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH  8
#define TAIL_BBOX_HEIGHT 9

#define TAIL_SPEED		0.2f 


class Tail : public CGameObject
{
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
public:
	float maxX = 0.0f;
	Tail(int nx, float maxX);
};

