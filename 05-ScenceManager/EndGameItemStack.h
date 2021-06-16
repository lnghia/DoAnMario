#pragma once
#include "JustForShow.h"
#include "ItemBox.h"

#define ENDGAMECARDSTACK_BBOX_WIDTH		72
#define ENDGAMECARDSTACK_BBOX_HEIGHT	28

#define ITEM_X	4
#define ITEM_Y	6

class EndGameItemStack : public JustForShow
{
	vector<ItemBox*> itemStack;
public:
	EndGameItemStack() {};
	EndGameItemStack(float x, float y);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void UpdatePos(float x, float y);
	void SetAniSet(int ani_set);
};

