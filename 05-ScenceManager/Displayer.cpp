#include "Displayer.h"

Displayer::Displayer()
{
}

void Displayer::Render()
{
	for (auto& c : characters) {
		c->Render();
	}
}

void Displayer::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + characters.size() * DIGIT_BBOX_WIDTH;
	b = t + DIGIT_BBOX_HEIGHT;
}

void Displayer::Add(int val)
{
	int i = characters.size();
	int unit, reminder = 0, temp;

	while ((val || reminder) && i--) {
		unit = val % 10;
		val /= 10;
		temp = characters[i]->GetDigit();
		temp += reminder + unit;
		reminder = temp / 10;
		characters[i]->SetDigit(temp % 10);
	}
}

void Displayer::Sub(int val)
{
	int content = 0;

	for (auto& chr : characters) {
		content = content * 10 + chr->GetDigit();
	}

	if (!content) {
		return;
	}

	int result = 0, remainder = 0;
	int i = characters.size(), unit, temp;

	while ((val || remainder) && i--) {
		unit = val % 10;
		val /= 10;
		temp = characters[i]->GetDigit();
		temp -= (remainder + unit);
		remainder = (temp < 0);
		characters[i]->SetDigit((temp < 0) ? temp + 10 : temp);
	}
}

void Displayer::SetContent(int val)
{
	if (val <= 0) {
		for (auto& c : characters) {
			c->SetDigit(0);
		}
		return;
	}

	int i = characters.size();

	while (val && i--) {
		int unit = val % 10;
		val /= 10;
		characters[i]->SetDigit(unit);
	}

	while (i--) {
		characters[i]->SetDigit(0);
	}
}

void Displayer::SetContent(string& str) {
	for (int i = 0; i < (int)str.length(); ++i) {
		characters[i]->SetDigit(str[i]);
	}
}

void Displayer::UpdatePos(float x, float y)
{
	int i = 0;

	this->x = x;
	this->y = y;

	for (auto& c : characters) {
		c->SetPosition(x + i++ * DIGIT_BBOX_WIDTH, y);
	}
}

int Displayer::GetContent()
{
	int sum = 0;

	for (auto& chr : characters) {
		sum = sum * 10 + chr->GetDigit();
	}

	return sum;
}

void Displayer::SetAniSet(int ani_set)
{
	animation_set = CAnimationSets::GetInstance()->Get(ani_set);
	for (auto& c : characters) {
		c->SetAniSet(ani_set);
	}
}
