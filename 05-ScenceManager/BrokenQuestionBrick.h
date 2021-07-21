#pragma once
#include "QBrick.h"
#include "ObjectCheatSheet.h"
#include "Board.h"
#include <stack>

#define ItemType first
#define ItemAni	second

#define QBRICK_ANI_MYSTERIOUS_IN_BROKEN_BRICK	2

class BrokenQuestionBrick : public QBrick
{
	int hiddenItem = 0;
	int hiddenItemAni = 0;

	int backupItem = 0;
	int backupItemAni = 0;

	float oldX = 0;
	float oldY = 0;

	DWORD start_hopUp = NULL;
	bool hopUp = 0;

	stack<pair<int, int>> hiddenItemTypes;
public:
	BrokenQuestionBrick() {
		renderPriority = 101;

		canBeStandOn = 1;

		currAni = 0;
	};
	BrokenQuestionBrick(float x, float y) {
		this->x = x;
		this->y = y;
		oldY = y;

		renderPriority = 101;

		canBeStandOn = 1;

		currAni = 0;
	}
	BrokenQuestionBrick(const int& hiddenItem, const int& hiddenItemAni, const int& backupItem = -1, const int& backupItemAni = -1) {
		this->hiddenItem = hiddenItem;
		this->hiddenItemAni = hiddenItemAni;

		this->backupItem = backupItem;
		this->backupItemAni = backupItemAni;

		renderPriority = 101;

		canBeStandOn = 1;

		currAni = 0;

		state = QBRICK_STATE_MYSTERIOUS;
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void RenderCurrFrame();
	
	void AddHiddenItem(int hiddenItem, int hiddenItemAni) {
		hiddenItemTypes.push({ hiddenItem, hiddenItemAni });
	}

	virtual void PopUpHiddenItem();

	virtual void HopUpABit();
};

