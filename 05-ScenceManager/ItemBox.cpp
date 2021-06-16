#include "ItemBox.h"
#include "Utils.h"
#include "Animations.h"

ItemBox::ItemBox(int itemTypeId, int itemTypeAni, float x, float y)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	switch (itemTypeId) {
	case OBJECT_TYPE_MUSHROOM_CARD:
		item = new JustForShow();

		break;
	case OBJECT_TYPE_FLOWER_CARD:
		item = new JustForShow();

		break;
	case OBJECT_TYPE_STAR_CARD:
		item = new JustForShow();
	
		break;
	}

	if (item) {
		item->SetAnimationSet(animation_sets->Get(itemTypeAni));
		item->SetPosition(x + 4, y + 6);
	}
}

void ItemBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + ITEMBOX_BBOX_WIDTH;
	b = t + ITEMBOX_BBOX_HEIGHT;
}

void ItemBox::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void ItemBox::Render()
{
	animation_set->at(0)->Render(x, y);
	if (item) {
		item->Render();
		return;
	}
	//DebugOut(L"[Debug] ITEM BOX IS EMPTY\n");
}
