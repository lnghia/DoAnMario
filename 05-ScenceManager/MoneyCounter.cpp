#include "MoneyCounter.h"

void MoneyCounter::Render()
{
	unit->Render();
	tens->Render();
}

void MoneyCounter::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + MONEYCOUNTER_BBOX_WIDTH;
	b = t + MONEYCOUNTER_BBOX_HEIGHT;
}

void MoneyCounter::AddMoney(int val)
{
	int u = val % 10;
	int t = val / 10;

	int reminder = 0;
	int temp;

	unit->Add(u, reminder);
	temp = reminder;
	tens->Add(temp, reminder);
	temp = reminder;
	tens->Add(temp, reminder);
}
