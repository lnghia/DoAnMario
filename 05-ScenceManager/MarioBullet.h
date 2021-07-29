#pragma once
#include "FireBall.h"
#include "GameObject.h"
#include "Grid.h"


#define MARIOFIREBALL_BBOX_WIDTH  8
#define MARIOFIREBALL_BBOX_HEIGHT 9

#define MARIOFIREBALL_SPIN_RIGHT	0
#define MARIOFIREBALL_SPIN_LEFT		1
#define MARIOFIREBALL_VX	0.22f 
#define MARIOFIREBALL_VY	0.1f

#define MARIOFIREBALL_ANI_HIT_TARGET	2

#define MARIOFIREBALL_STATE_SPIN	100

#define FIREBALL_ANI_SET	610

#define MARIOFIREBALL_HIT_TARGET_TIME	450

#define MARIOFIREBALL_GRAVITY	0.0008f

class MarioBullet : public CGameObject
{
public:
	bool hitTarget = 0;
	DWORD start_hit_target = NULL;

	MarioBullet(int nx, int ny = 1);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	virtual void RenderCurrFrame();
};

