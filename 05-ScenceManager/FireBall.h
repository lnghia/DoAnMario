#pragma once
#include "GameObject.h"
#include "Game.h"

#define PLANTFIREBALL_BBOX_WIDTH  8
#define PLANTFIREBALL_BBOX_HEIGHT 9

#define PLANTFIREBALL_SPIN_RIGHT	0
#define PLANTFIREBALL_SPIN_LEFT		1
#define PLANTFIREBALL_VX	0.05f 
#define PLANTFIREBALL_VY	0.04f

#define PLANTFIREBALL_STATE_SPIN	100

class FireBall : public CGameObject
{
public:
	FireBall(int nx, int ny);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
};

