#pragma once
#include "GameObject.h"
#include "Grid.h"
#include "Brick.h"

#define CHOSEN_ITEM_VY	-0.06f

#define CHOSEN_ITEM_ANI_STAR	0
#define CHOSEN_ITEM_ANI_FLOWER		1
#define CHOSEN_ITEM_ANI_MUSHROOM	2

#define CHOSEN_ITEM_STATE_STAR		0
#define CHOSEN_ITEM_STATE_FLOWER	1
#define CHOSEN_ITEM_STATE_MUSHROOM	2

#define CHOSEN_ITEM_ANI_SET		200057	

class ChoosenEndGameItem : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void RenderCurrFrame();

	ChoosenEndGameItem();
};

