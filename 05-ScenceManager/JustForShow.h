#pragma once
#include "GameObject.h"

#define JUST_FOR_SHOW_BBOX_WIDTH  16
#define JUST_FOR_SHOW_BBOX_HEIGHT 16

class JustForShow : public CGameObject
{
public:
	JustForShow();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};