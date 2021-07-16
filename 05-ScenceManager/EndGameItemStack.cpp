#include "EndGameItemStack.h"
#include "Utils.h"

EndGameItemStack::EndGameItemStack(float x, float y)
{
	itemStack.resize(3);
	for (UINT i = 0; i < 3; ++i) {
		itemStack[i] = new ItemBox();
		itemStack[i]->UpdatePos(x + i * ITEMBOX_BBOX_WIDTH, y);
	}
}

void EndGameItemStack::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + ENDGAMECARDSTACK_BBOX_WIDTH;
	b = t + ENDGAMECARDSTACK_BBOX_HEIGHT;
}

void EndGameItemStack::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	for (UINT i = 0; i < 3; ++i) {
		itemStack[i]->UpdatePos(x + i * ITEMBOX_BBOX_WIDTH + ITEM_X, y + ITEM_Y);
	}
}

void EndGameItemStack::Render()
{
	//animation_set->at(0)->Render(x, y);
	for (auto& item : itemStack) {
		if(item) item->Render();
	}
}

void EndGameItemStack::UpdatePos(float x, float y)
{
	this->x = x;
	this->y = y;

	for (UINT i = 0; i < 3; ++i) {
		if (itemStack[i]) {
			itemStack[i]->UpdatePos(x + i * ITEMBOX_BBOX_WIDTH + ITEM_X, y + ITEM_Y);
		}
	}
}

void EndGameItemStack::SetAniSet(int ani_set)
{
	animation_set = CAnimationSets::GetInstance()->Get(ani_set);

	if (!itemStack.size()) {
		itemStack.resize(3);
		for (UINT i = 0; i < 3; ++i) {
			itemStack[i] = new ItemBox();
			itemStack[i]->SetAniSet(ani_set);
		}
	}
}

void EndGameItemStack::push(int itemType)
{
	if (itemType >= 0) {
		if (!itemStack[currItem]) itemStack[currItem] = new ItemBox();
		itemStack[currItem++]->SetItem(itemType);
		currItem %= 4;
	}
}
