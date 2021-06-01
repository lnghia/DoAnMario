#pragma once
#include "GameObject.h"
#include "Mario.h"

class HiddenItem : public CGameObject
{
protected:
	DWORD deflect_start = 0;

	float containerX;
	float containerY;

	bool passedDestrucPoint = 0;
public:
	HiddenItem() {}
	HiddenItem(float x, float y);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	virtual void GotObsorbed(CMario* mario);
};

