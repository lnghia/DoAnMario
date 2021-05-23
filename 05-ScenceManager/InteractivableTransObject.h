#pragma once
#include "GameObject.h"

class InteractivableTransObject : public CGameObject
{
protected:
	int width;
	int height;
public:
	InteractivableTransObject();
	InteractivableTransObject(int width, int height);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

