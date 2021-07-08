#pragma once
#include "Brick.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "GameObject.h"

#define QBRICK_ANI_MYSTERIOUS 0
#define QBRICK_ANI_NO_MORE_MYSTERIOUS 1

#define QBRICK_STATE_MYSTERIOUS 0
#define QBRICK_STATE_NO_MORE_MYSTERIOUS 1

class QBrick : public CGameObject
{
	int hiddenItem = 0;
	int hiddenItemAni = 0;

	int backupItem = 0;
	int backupItemAni = 0;

	float oldX = 0;
	float oldY = 0;

	DWORD start_hopUp = NULL;
	bool hopUp = 0;
public:
	QBrick(){}
	QBrick(const int& hiddenItem, const int& hiddenItemAni, const int& backupItem = -1, const int& backupItemAni = -1){
		this->hiddenItem = hiddenItem;
		this->hiddenItemAni = hiddenItemAni;

		this->backupItem = backupItem;
		this->backupItemAni = backupItemAni;

		renderPriority = 101;

		canBeStandOn = 1;

		currAni = 0;

		state == QBRICK_STATE_MYSTERIOUS;
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void RenderCurrFrame();

	virtual void PopUpHiddenItem();

	virtual void HopUpABit();
};

