#pragma once
#include "JustForShow.h"
#include "Digit.h"

#define MONEYCOUNTER_BBOX_WIDTH		16
#define MONEYCOUNTER_BBOX_HEIGHT	7

class MoneyCounter : public JustForShow
{
	Digit* unit;
	Digit* tens;
public:
	MoneyCounter():JustForShow(){
		unit = new Digit();
		tens = new Digit();
	}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void AddMoney(int val);
};

