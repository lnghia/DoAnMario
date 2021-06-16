#pragma once
#include "Displayer.h"
#include "JustForShow.h"
#include "Displayer.h"

class GameBoard : public JustForShow 
{
	Displayer* world;
	Displayer* money;
	Displayer* time;
	Displayer* lives;
	Displayer* world;
	Displayer* point;

public:
	GameBoard() {
		money = new Displayer();
		time = new Displayer();
		lives = new Displayer();
		world = new Displayer();
		point = new Displayer();
	};

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
};

