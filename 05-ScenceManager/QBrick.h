#pragma once
#include "Brick.h"
#include "Coin.h"
#include "GameObject.h"

#define QBRICK_ANI_MYSTERIOUS 0
#define QBRICK_ANI_NO_MORE_MYSTERIOUS 1

#define QBRICK_STATE_MYSTERIOUS 0
#define QBRICK_STATE_NO_MORE_MYSTERIOUS 1

class QBrick : public CGameObject
{
	int hiddenItem;
	int hiddenItemAni;
public:
	QBrick(){}
	QBrick(int hiddenItem, int hiddenItemAni){
		this->hiddenItem = hiddenItem;
		this->hiddenItemAni = hiddenItemAni;

		canBeStandOn = 1;
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void PopUpHiddenItem();
};

