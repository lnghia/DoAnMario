#pragma once
#include "GameObject.h"
#include "Map.h" 
#include "Grid.h"

#define FRAGMENT_SPIN_SPEED_VX	0.07f 
#define FRAGMENT_SPIN_SPEED_VY	0.35f 

#define FRAGMENT_SPIN_SPEED_VX1	0.06f 
#define FRAGMENT_SPIN_SPEED_VY1	0.2f 
#define FRAGMENT_GRAVITY		0.001f

#define FRAGMENT_WIDTH_BBOX		9
#define FRAGMENT_HEIGHT_BBOX	10

class Fragment : public CGameObject
{
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
public:
	int nx = 0;
	int ny = 0;
	Fragment(int side);
};

