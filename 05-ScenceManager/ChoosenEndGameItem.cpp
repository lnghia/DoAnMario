#include "ChoosenEndGameItem.h"

void ChoosenEndGameItem::Render()
{
	int ani = CHOSEN_ITEM_ANI_STAR;

	if (state == CHOSEN_ITEM_STATE_FLOWER) {
		ani = CHOSEN_ITEM_ANI_FLOWER;
	}
	else if(state == CHOSEN_ITEM_STATE_MUSHROOM) {
		ani = CHOSEN_ITEM_ANI_MUSHROOM;
	}

	animation_set->at(ani)->Render(x, y);
}

void ChoosenEndGameItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = t + 16;
}

void ChoosenEndGameItem::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();
	float camx;
	float camy;
	float scrh = float(game->GetScreenHeight());
	float scrw = float(game->GetScreenWidth());
	camx = game->GetCamX();
	camy = game->GetCamY();

	if (y < camy || y > camy + scrh || x > camx + scrw || x + BRICK_BBOX_WIDTH / 2 < camx) {
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);
		//Grid::GetInstance()->ObjIntoTrash(this);

		return;
	}

	Grid::GetInstance()->clearObjFromGrid(this);

	CGameObject::Update(dt);

	y += dy;

	Grid::GetInstance()->putObjectIntoGrid(this);
}

void ChoosenEndGameItem::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}

ChoosenEndGameItem::ChoosenEndGameItem()
{
	vy = CHOSEN_ITEM_VY;
	interactivable = 0;
	//invisible = 0;
	renderPriority = 101;

	animation_set = CAnimationSets::GetInstance()->Get(CHOSEN_ITEM_ANI_SET);
}
