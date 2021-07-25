#pragma once
#include "JustForShow.h"
#include "Displayer.h"
#include "ItemBox.h"
#include "EndGameItemStack.h"
#include "Game.h"
#include "Textures.h"
#include "TimeTicker.h"
#include "SpeedBar.h"

#define BOARD_BBOX_WIDTH	234
#define	BOARD_BBOX_HEIGHT	48

#define BACKGOUND_TEX_ID	140

#define WORLDFIELD_X	16
#define WORLDFIELD_Y	136

#define WORLDNUM_X	38
#define WORLDNUM_Y	8

#define PLAYERCHAR_X	16
#define PLAYERCHAR_Y	143

#define LIVES_X	30
#define LIVES_Y	16

#define POINT_X	53
#define POINT_Y	16

#define TIME_X	125
#define TIME_Y	16

#define CLOCK_X	128
#define CLOCK_Y	143

#define DOLLAR_X	136
#define DOLLAR_Y	134

#define MONEY_X	133
#define MONEY_Y	8

#define ITEMSTACK_X	161
#define ITEMSTACK_Y	1

#define SPEEDBAR_X	53
#define SPEEDBAR_Y	7

class Board :public JustForShow
{
	static Board* __instance;

	Displayer* point;
	Displayer* money;
	TimeTicker* time;
	Displayer* lives;
	Displayer* worldNum;
	Displayer* worldField;
	JustForShow* playerChar;
	JustForShow* clock;
	JustForShow* dollarSign;
	EndGameItemStack* cardStack;
	SpeedBar* speedBar;
public:
	Board();

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void UpdatePos(float x, float y);
	void UpdateComponentPos();
	void RenderBackGround();

	Displayer* GetPoint();
	Displayer* GetMoney();
	TimeTicker* GetTime();
	Displayer* GetLives();
	Displayer* GetWorldNum();
	Displayer* GetWorldField();
	JustForShow* GetPlayerChar();
	JustForShow* GetClock();
	JustForShow* GetDollarSign();
	EndGameItemStack* GetCardStack();
	SpeedBar* GetSpeedBar();

	void AddLives(int val);
	void SubLives(int val);
	void SetLives(int val);
	int LivesCount();

	void SetAniSet(int ani_set);

	static Board* GetInstance();
};

