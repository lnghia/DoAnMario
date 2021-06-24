#pragma once
#include "JustForShow.h"
#include "Mario.h"
#include "Animations.h"

#define SPEEDBAR_BBOX_WIDTH		64
#define SPEEDBAR_BBOX_HEIGHT	7

class SpeedBar : public JustForShow
{
	short int level = 0;
	CMario* player = NULL;
	short int step = 0;

	DWORD start_running;
public:
	SpeedBar() { start_running = (DWORD)GetTickCount64(); };
	SpeedBar(float x, float y, CMario* player);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void UpdatePos(float x, float y);

	void SetAniSet(int ani_set);

	void SetPlayer(CMario* player);
};

