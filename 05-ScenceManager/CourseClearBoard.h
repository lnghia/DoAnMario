#pragma once
#include "JustForShow.h"

#define CARD_TYPE_STAR		0
#define CARD_TYPE_FLOWER	1
#define CARD_TYPE_MUSHROOM	2

#define COURSE_BOARD_BBOX_WIDTH		248
#define COURSE_BOARD_BBOX_HEIGHT	73

class CourseClearBoard : public JustForShow
{
public:
	int cardType = 0;

	CourseClearBoard() { invisible = 1; };
	CourseClearBoard(int camW, int camH);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void RenderCurrFrame();

	void UpdatePos(float x, float y);

	void SetCardType(int val);
};

