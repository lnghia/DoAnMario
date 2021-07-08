#pragma once
#include "GameObject.h"
#include "QBrick.h"
#include "ObjectCheatSheet.h"
#include "Board.h"

#include <stack>

#define ItemType first
#define ItemAni	second

class NoteBrick : public CGameObject
{
	float oldX = 0;
	float oldY = 0;

	DWORD start_hopUp = NULL;
	DWORD start_moveDown = NULL;
	bool hopUp = 0;
	bool moveDown = 0;

	stack<pair<int, int>> hiddenItemTypes;
public:
	NoteBrick() {
		renderPriority = 101;

		canBeStandOn = 1;

		currAni = 0;
	};
	NoteBrick(float x, float y) {
		this->x = x;
		this->y = y;
		oldY = y;

		renderPriority = 101;

		canBeStandOn = 1;

		currAni = 0;
	}
	/*NoteBrick(const int& hiddenItem, const int& hiddenItemAni, const int& backupItem = -1, const int& backupItemAni = -1) {
		this->hiddenItem = hiddenItem;
		this->hiddenItemAni = hiddenItemAni;

		this->backupItem = backupItem;
		this->backupItemAni = backupItemAni;

		renderPriority = 101;

		canBeStandOn = 1;

		currAni = 0;

		state == QBRICK_STATE_MYSTERIOUS;
	}*/

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void RenderCurrFrame();
	
	void AddHiddenItem(int hiddenItem, int hiddenItemAni) {
		hiddenItemTypes.push({ hiddenItem, hiddenItemAni });
	}

	virtual void DropHiddenItem();

	virtual void HopUpABit();

	virtual void MoveDownABit();
};

