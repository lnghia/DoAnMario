#pragma once
#include "GameObject.h"
#include "BrokenBrick.h"
#include "FloatingCoin.h"
#include "Grid.h"
#include "Animations.h"

#define PSWITCH_STATE_APPEAR	0
#define PSWITCH_STATE_NOT_TRIGGER	1
#define PSWITCH_STATE_TRIGGER	2

#define PSWITCH_ANI_SET			999996

#define PSWITCH_ANI_APPEAR		2
#define PSWITCH_ANI_NOT_TRIGGERD	0
#define PSWITCH_ANI_TRIGGERED		1

#define PSWITCH_BBOX_HEIGHT	16
#define PSWITCH_BBOX_WIDTH	16
#define PSWITCH_BBOX_TRIGGER_HEIGHT	7
#define PSWITCH_BBOX_TRIGGER_WIDTH	16

#define PSWITCH_APPEAR_TIME		400
#define PSWITCH_TRIGGER_TIME	5000

#define COIN_ANI_SET	90

class PSwitch : public HiddenItem
{
	vector<FloatingCoin*> coins;
public:
	DWORD start_appear = NULL;
	DWORD start_trigger = NULL;

	vector<BrokenBrick*> affectedBricks;

	PSwitch() {}
	PSwitch(float x, float y) {
		containerX = x;
		containerY = y;

		this->x = x;
		this->y = y - 16;

		interactivable = 0;
		invisible = 0;
		renderPriority = 100;
		canBeStandOn = 0;

		vx = 0;
		vy = 0;

		animation_set = CAnimationSets::GetInstance()->Get(PSWITCH_ANI_SET);
	};

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	
	void trigger();
	void appear();
};

