#pragma once
#include "JustForShow.h"
#include "ObjectCheatSheet.h"

#define ITEMBOX_BBOX_WIDTH	24
#define ITEMBOX_BBOX_HEIGHT	28

#define ITEMBOX_ANI			2048

#define ITEMBOX_ANI_EMPTY		200048
#define ITEMBOX_ANI_MUSHROOM	200049
#define ITEMBOX_ANI_FLOWER		200050
#define ITEMBOX_ANI_STAR		200051

class ItemBox : public JustForShow
{
public:
	JustForShow* item = NULL;
	int itemType = OBJECT_TYPE_STAR_CARD;

	ItemBox() {};
	ItemBox(int itemTypeId, int itemTypeAni, float x, float y);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetItem(int itemType);

	void RefreshAniSet();

	void UpdatePos(float x, float y);
};

