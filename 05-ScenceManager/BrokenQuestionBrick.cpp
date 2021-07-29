#include "BrokenQuestionBrick.h"

void BrokenQuestionBrick::Render()
{
	int ani = (state == QBRICK_STATE_MYSTERIOUS) ? QBRICK_ANI_MYSTERIOUS_IN_BROKEN_BRICK : QBRICK_ANI_NO_MORE_MYSTERIOUS;

	currAni = ani;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void BrokenQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = this->oldY;
	r = x + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void BrokenQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (hopUp && (DWORD)GetTickCount64() - start_hopUp >= 150) {
		y = oldY;
		hopUp = 0;
		//interactivable = 1;
	}
}

void BrokenQuestionBrick::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}

void BrokenQuestionBrick::PopUpHiddenItem()
{
	if (state != QBRICK_STATE_MYSTERIOUS) {
		return;
	}

	if (psw) {
		psw->appear();
	}
	else if (!hiddenItemTypes.empty()) {
		int item = hiddenItemTypes.top().ItemType;
		int ani = hiddenItemTypes.top().ItemAni;

		hiddenItemTypes.pop();

		Grid* grid = Grid::GetInstance();
		LPGAMEOBJECT obj = NULL;
		CGame* game = CGame::GetInstance();

		switch (item)
		{
		case OBJECT_TYPE_COIN: {
			obj = new Coin(x, y);
			obj->SetInteractivable(0);
			obj->SetCanBeStandOn(0);
			Board::GetInstance()->GetPoint()->Add(COIN_POINT);
			Board::GetInstance()->GetMoney()->Add(COIN_MONEY);

			break;
		}
		}

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();

		if (obj) {
			obj->SetAnimationSet(animation_sets->Get(ani));

			grid->putObjectIntoGrid(obj);
		}

		HopUpABit();
	}
}

void BrokenQuestionBrick::HopUpABit()
{
	if (state == QBRICK_STATE_MYSTERIOUS) {
		if (!hopUp) {
			oldY = y;
			//interactivable = 0;
			hopUp = 1;
			start_hopUp = (DWORD)GetTickCount64();
			y -= 3.0f;
		}
		//vx = -0.5f;
		state = (hiddenItemTypes.empty()) ? QBRICK_STATE_NO_MORE_MYSTERIOUS : QBRICK_STATE_MYSTERIOUS;
	}
}
