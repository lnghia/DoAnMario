#include "CourseClearBoard.h"
#include "ObjectCheatSheet.h"

CourseClearBoard::CourseClearBoard(int camW, int camH)
{
}

void CourseClearBoard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + COURSE_BOARD_BBOX_WIDTH;
	b = t + COURSE_BOARD_BBOX_HEIGHT;
}

void CourseClearBoard::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void CourseClearBoard::Render()
{
	int ani = 0;

	if (cardType == OBJECT_TYPE_FLOWER_CARD) {
		ani = 1;
	}
	else if (cardType == OBJECT_TYPE_MUSHROOM_CARD) {
		ani = 2;
	}

	currAni = cardType;
	animation_set->at(ani)->Render(x, y);
}

void CourseClearBoard::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}

void CourseClearBoard::UpdatePos(float x, float y)
{
}

void CourseClearBoard::SetCardType(int val)
{
	cardType = val;
}
