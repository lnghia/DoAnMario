#include "NoteBrick.h"

void NoteBrick::Render()
{
	currAni = 0;
	animation_set->at(0)->Render(x, y);
}

void NoteBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = (hopUp) ? oldY : y;
	r = x + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void NoteBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (hopUp && (int)((DWORD)GetTickCount64() - start_hopUp) >= NOTEBRICK_HOP_SINK_TIME) {
		y = oldY;
		hopUp = 0;
		//interactivable = 1;
	}
	else if (moveDown && (int)((DWORD)GetTickCount64() - start_moveDown) >= NOTEBRICK_HOP_SINK_TIME) {
		y = oldY;
		moveDown = 0;
	}
	else if (moveHorizontally && (int)((DWORD)GetTickCount64() - start_moveHorizontally) >= NOTEBRICK_HOP_SINK_TIME) {
		x = oldX;
		moveHorizontally = 0;
		moveDirect = 0;
	}
}

void NoteBrick::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}

void NoteBrick::DropHiddenItem()
{
	if (hiddenItemTypes.empty()) {
		return;
	}

	if (!hiddenItemTypes.empty()) {
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

		MoveDownABit();
	}
}

void NoteBrick::HopUpABit()
{
	if (!hopUp) {
		oldY = y;
		//interactivable = 0;
		hopUp = 1;
		start_hopUp = (DWORD)GetTickCount64();
		y -= 3.0f;
	}
}

void NoteBrick::MoveHorizontallyABit(int nx)
{
	if (!moveHorizontally) {
		oldX = x;
		moveHorizontally = 1;
		moveDirect = nx;
		start_moveHorizontally = (DWORD)GetTickCount64();
		x += (nx > 0) ? 3.0f : -3.0f;
	}
}

void NoteBrick::MoveDownABit()
{
	if (!moveDown) {
		oldY = y;
		//interactivable = 0;
		moveDown = 1;
		start_moveDown = (DWORD)GetTickCount64();
		y += 3.0f;
	}
}
