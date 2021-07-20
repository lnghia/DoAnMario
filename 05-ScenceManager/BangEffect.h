#pragma once
#include "JustForShow.h"
#include "Grid.h"

#define BANG_EFFECT_ANI_SET	100044

#define LIFE_SPAN			200

#define BANG_EFFECT_BBOX_WIDTH	16
#define BANG_EFFECT_BBOX_HEIGHT	16

class BangEffect : public JustForShow
{
	DWORD start_live = 0;
public:
	BangEffect() { start_live = (DWORD)GetTickCount64(); renderPriority = 102; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void RenderCurrFrame();
	
};

