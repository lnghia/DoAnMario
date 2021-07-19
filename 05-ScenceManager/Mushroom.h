#pragma once
#include "GameObject.h"
#include "Coin.h"
#include "Brick.h"
#include "QBrick.h"
#include "PipeHitBox.h"
#include "ColorBrickHitBox.h"
#include "Mario.h"
#include "HiddenItem.h"

#define MUSHROOM_STATE_APPEAR	0
#define MUSHROOM_STATE_RUN		1

#define MUSHROOM_BBOX_HEIGHT	16
#define MUSHROOM_BBOX_WIDTH		16

#define MUSHROOM_MOVING_LEFT	-0.065f
#define MUSHROOM_MOVING_RIGHT	0.065f
#define MUSHROOM_RISING_UP		-0.06f

#define MUSHROOM_GRAVITY		0.002f

#define MUSHROOM_POINT			1000

#define MUSHROOM_ANI_SET	93

class Mushroom : public HiddenItem
{
public:
	Mushroom() {}
	Mushroom(float x, float y) :HiddenItem(x, y) {
		containerX = x;
		containerY = y;

		vx = 0;
		vy = MUSHROOM_RISING_UP;

		state = MUSHROOM_STATE_APPEAR;

		animation_set = CAnimationSets::GetInstance()->Get(MUSHROOM_ANI_SET);
	};

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

};

