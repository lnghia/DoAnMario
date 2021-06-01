#include "QBrick.h"
#include "ObjectCheatSheet.h"
#include "Grid.h"

void QBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void QBrick::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}

void QBrick::PopUpHiddenItem()
{
	if (state != QBRICK_STATE_MYSTERIOUS) {
		return;
	}

	unsigned short int ani = state;
	Grid* grid = Grid::GetInstance();
	LPGAMEOBJECT obj = NULL;

	switch (hiddenItem)
	{
	case OBJECT_TYPE_COIN: {
		obj = new Coin(x, y);
		obj->SetInteractivable(0);
		obj->SetCanBeStandOn(0);

		break;
	}
	case OBJECT_TYPE_MUSHROOM: {
		obj = new Mushroom(x, y);

		break;
	}
	}

	if (hiddenItem == OBJECT_TYPE_COIN) {

	}

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	if (obj) {

		obj->SetAnimationSet(animation_sets->Get(hiddenItemAni));
		grid->putObjectIntoGrid(obj);
	}

	state = QBRICK_STATE_NO_MORE_MYSTERIOUS;
	// add the object into the grid (in case that object is a mushroom or something other than coin)
}

void QBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void QBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
