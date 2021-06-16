#pragma once
#include "GameObject.h"
#include "Digit.h"
#include "JustForShow.h"

class Displayer : public JustForShow
{
	vector<Digit*> characters;
public:
	Displayer();

	Displayer(int charNum) {
		characters.resize(charNum);
		for (auto& c : characters) {
			c = new Digit();
		}
	}

	Displayer(int charNum, float x, float y) {
		int i = 0;

		this->x = x;
		this->y = y;

		characters.resize(charNum);
		for (auto& c : characters) {
			c = new Digit();
			c->SetPosition(x + i++ * DIGIT_BBOX_WIDTH, y);
		}
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void Add(int val);
	void SetContent(int val);
	void SetContent(string& str);	
	void UpdatePos(float x, float y);

	void SetAniSet(int ani_set);
};

