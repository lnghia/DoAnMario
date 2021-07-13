#include "EndGameBrick.h"

EndGameBrick::EndGameBrick()
{
	isActive = 1;
	interactivable = 1;
	state = ENDGAME_BRICK_STATE_GATCHA;
}

void EndGameBrick::Render()
{
	int ani = (state == ENDGAME_BRICK_STATE_EMPTY) ? ENDGAME_BRICK_ANI_EMPTY : ENDGAME_BRICK_ANI_GATCHA;

	currAni = ani;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void EndGameBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void EndGameBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void EndGameBrick::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
	//RenderBoundingBox();
}

int EndGameBrick::PopUpChoosenItem()
{
	if (state != ENDGAME_BRICK_STATE_GATCHA) {
		return -1;
	}

	int currFrame = animation_set->at(ENDGAME_BRICK_ANI_GATCHA)->GetCurrFrame();

	LPGAMEOBJECT _obj = new ChoosenEndGameItem();

	if (currFrame == 0 || currFrame == -1) {
		_obj->SetState(CHOSEN_ITEM_STATE_STAR);
	}
	else if (currFrame == 1) {
		_obj->SetState(CHOSEN_ITEM_STATE_FLOWER);
	}
	else {
		_obj->SetState(CHOSEN_ITEM_STATE_MUSHROOM);
	}

	_obj->SetPosition(x, y);

	//state = ENDGAME_BRICK_STATE_EMPTY;

	Grid::GetInstance()->putObjectIntoGrid(_obj);

	return (currFrame == 0 || currFrame == -1) ? OBJECT_TYPE_STAR_CARD : (currFrame == 1) ? OBJECT_TYPE_FLOWER_CARD : OBJECT_TYPE_MUSHROOM_CARD;
}
