#pragma once
#include "JustForShow.h"

#define DIGIT_BBOX_WIDTH	8
#define DIGIT_BBOX_HEIGHT	8

class Digit : public JustForShow
{
	UINT digit;
public:
	Digit() { digit = 0; };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void Add(const int& val, int& reminder);
	void Sub(const int& val, int& reminder);

	void SetDigit(UINT val);
	int GetDigit();

	void SetAniSet(int ani_set);
};

