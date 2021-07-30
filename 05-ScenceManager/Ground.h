#pragma once
#include "GameObject.h"

class Ground : public CGameObject
{
	int width;
	int height;
public:
	Ground();
	Ground(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	int GetWidth();
	int GetHeight();
};

