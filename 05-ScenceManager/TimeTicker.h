#pragma once
#include "JustForShow.h"
#include "Displayer.h"
#include "GameObject.h"
#include "Utils.h"

#define TIMER	300

class TimeTicker : public CGameObject
{
	TimeTicker* instance = NULL;
	DWORD start = NULL;
	Displayer* displayer = NULL;

	int timer;
	int currMoment;

	int width = 0;
	int height = 0;

	bool isTicking = 0;
public:
	TimeTicker() {
		timer = TIMER;
		currMoment = timer;
		displayer = new Displayer(3);
	};
	TimeTicker(float x, float y);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void UpdatePos(float x, float y);
	void StartTicking();
	void SetAniSet(int ani_set);

	bool GetIsTicking();
	int GetCurrMoment();

	void Reset();
};

