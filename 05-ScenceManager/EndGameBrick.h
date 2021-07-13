#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "ChoosenEndGameItem.h"
#include "ObjectCheatSheet.h"

#define ENDGAME_BRICK_ANI_EMPTY		0
#define ENDGAME_BRICK_ANI_GATCHA	1

#define ENDGAME_BRICK_STATE_EMPTY	0
#define ENDGAME_BRICK_STATE_GATCHA	1

class EndGameBrick : public CGameObject
{

public:
	EndGameBrick();

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void RenderCurrFrame();

	int PopUpChoosenItem();
};

