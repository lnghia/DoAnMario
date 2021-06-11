#pragma once
#include "GameObject.h"
#include "HiddenItem.h"
#include "Point.h"

#define COIN_SPEED_X	0.0f
#define COIN_SPEED_Y	-0.5f
#define COIN_GRAVITY	0.002f


#define COIN_DEFLECT_TIME	100

#define COIN_POINT	100

#define COIN_BBOX_HEIGHT	16
#define COIN_BBOX_WIDTH		16

class Coin : public CGameObject
{
protected:
	DWORD deflect_start = 0;

	float containerX;
	float containerY;

	bool passedDestrucPoint = 0;
public:
	Coin(){}
	Coin(float x, float y);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
};

