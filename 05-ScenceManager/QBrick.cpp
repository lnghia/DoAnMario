#include "QBrick.h"
#include "ObjectCheatSheet.h"
#include "Grid.h"
#include "Board.h"

void QBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (hopUp && GetTickCount() - start_hopUp >= 150) {
		y = oldY;
	}
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
		Board::GetInstance()->GetPoint()->Add(COIN_POINT);
		Board::GetInstance()->GetMoney()->Add(COIN_MONEY);

		break;
	}
	case OBJECT_TYPE_MUSHROOM: {
		obj = new Mushroom(x, y);

		break;
	}
	case OBJECT_TYPE_LEAF:
		obj = new Leaf(x, y);

		break;
	}

	if (hiddenItem == OBJECT_TYPE_COIN) {

	}

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	if (obj) {

		obj->SetAnimationSet(animation_sets->Get(hiddenItemAni));
		grid->putObjectIntoGrid(obj);
	}

	//state = QBRICK_STATE_NO_MORE_MYSTERIOUS;

	HopUpABit();
	// add the object into the grid (in case that object is a mushroom or something other than coin)
}

void QBrick::HopUpABit()
{
	if (state == QBRICK_STATE_MYSTERIOUS) {
		oldY = y;
		//vx = -0.5f;
		y -= 1.5f;
		state = QBRICK_STATE_NO_MORE_MYSTERIOUS;
		start_hopUp = GetTickCount();
		hopUp = 1;
	}
}

void QBrick::Render()
{
	int ani = (state == QBRICK_STATE_MYSTERIOUS) ? QBRICK_ANI_MYSTERIOUS : QBRICK_ANI_NO_MORE_MYSTERIOUS;

	currAni = ani;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void QBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
