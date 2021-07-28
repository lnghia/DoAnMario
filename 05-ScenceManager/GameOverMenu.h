#pragma once
#include "JustForShow.h"

#define OPTION_1	0
#define OPTION_2	1

#define GAMEOVER_MENU_BBOX_WIDTH	128
#define GAMEOVER_MENU_BBOX_HEIGHT	64

class GameOverMenu : public JustForShow
{
public:
	int option = 0;

	GameOverMenu() {};
	GameOverMenu(int camW, int camH);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void UpdatePos(float x, float y);
};

