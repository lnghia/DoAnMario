#pragma once
#include "JustForShow.h"
#include "ObjectCheatSheet.h"

#define ITEMBOX_BBOX_WIDTH	24
#define ITEMBOX_BBOX_HEIGHT	28

class ItemBox : public JustForShow
{
	JustForShow* item;
public:
	ItemBox() {};
	ItemBox(int itemTypeId, int itemTypeAni, float x, float y);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
};

