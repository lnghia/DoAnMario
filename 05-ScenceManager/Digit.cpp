#include "Digit.h"

void Digit::Render()
{
	animation_set->at(digit)->Render(x, y);
}

void Digit::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + DIGIT_BBOX_WIDTH;
	b = t + DIGIT_BBOX_HEIGHT;
}

void Digit::Add(const int& val, int& reminder)
{
	digit += val;
	if (digit > 9) {
		reminder = digit / 10;
		digit %= 10;
	}
	else {
		reminder = 0;
	}
}

void Digit::Sub(const int& val, int& reminder) {
	if (val > digit) {
		digit = 10 + digit - val;
		reminder = 1;
	}
	else {
		digit -= val;
		reminder = 0;
	}
}

void Digit::SetDigit(UINT val)
{
	digit = val;
}

int Digit::GetDigit() {
	return (int)digit;
}

void Digit::SetAniSet(int ani_set)
{
	animation_set = CAnimationSets::GetInstance()->Get(ani_set);
}
