#pragma once
#include "GameObject.h"
#include "Game.h"

#define BOOMERANG_SPIN_SPEED_VY		-0.03f 
#define BOOMERANG_SPIN_SPEED_VX		0.11f 

#define BOOMERANG_GRAVITY_X			0.007f
#define BOOMERANG_GRAVITY_Y			0.002f

#define BOOMERANG_ACCELERATE	
#define BOOMERANG_HEIGHT_

#define BOOMERANG_BBOX_WIDTH	16
#define BOOMERANG_BBOX_HEIGHT	15

#define BOOMERANG_GRAVITY		0.0002f

#define BOOMERANG_ANI_RIGHT					1
#define BOOMERANG_ANI_LEFT					0
#define BOOMERANG_ANI_BEING_HOLDED_LEFT		2
#define BOOMERANG_ANI_BEING_HOLDED_RIGHT	3

#define BOOMERANG_ANI_SET	110000

#define BOOMERANG_BEINGHOLDED_TIME	300

class Boomerang : public CGameObject
{
	float originY = 0;

	bool isHolded = 1;

	DWORD start_flying = 0;
public:
	int nx = 0;
	int	ny = 0;
	Boomerang() {};
	Boomerang(float y, int nx);

	void UpdateY(float val);

	void SetIsHolded(bool val);
	bool GetIsHolded();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
};