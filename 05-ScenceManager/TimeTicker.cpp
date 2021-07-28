#include "TimeTicker.h"

TimeTicker::TimeTicker(float x, float y)
{
	timer = 300;
	displayer = new Displayer(3, x, y);
	displayer->SetContent(timer);
	this->width = 3 * DIGIT_BBOX_WIDTH;
	this->height = DIGIT_BBOX_HEIGHT;
}

void TimeTicker::Render()
{
	displayer->Render();
}

void TimeTicker::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (isTicking) {
		currMoment = (timer - ((DWORD)GetTickCount64() - start) / 1000);
		displayer->SetContent(currMoment);
	}
	else {
		displayer->SetContent(0);
	}
}

void TimeTicker::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + 3 * DIGIT_BBOX_WIDTH;
	b = t + DIGIT_BBOX_HEIGHT;
}

void TimeTicker::UpdatePos(float x, float y)
{
	int i = 0;

	this->x = x;
	this->y = y;

	displayer->UpdatePos(x, y);
}

void TimeTicker::StartTicking()
{
	start = (DWORD)GetTickCount64();
	currMoment = (timer - ((DWORD)GetTickCount64() - start) / 1000);
	isTicking = 1;
}

void TimeTicker::StopTicking()
{
	isTicking = 0;
}

void TimeTicker::SetTime(int time)
{
	displayer->SetContent(time);
}

void TimeTicker::SetAniSet(int ani_set)
{
	displayer->SetAniSet(ani_set);
}

bool TimeTicker::GetIsTicking()
{
	return isTicking;
}

int TimeTicker::GetCurrMoment()
{
	return currMoment;
}

void TimeTicker::Reset()
{
	timer = currMoment = TIMER;
	isTicking = 0;
}
