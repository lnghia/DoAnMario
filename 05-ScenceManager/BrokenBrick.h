#pragma once
#include "GameObject.h"
#include "Fragment.h"
#include "FloatingCoin.h"

#define BROKENBRICK_ANI_BRICK	90013
//#define BROKENBRICK_ANI_COIN	1
#define BROKENBRICK_ANI_FRAGMENT	1

#define BROKENBRICK_STATE_BRICK 0 
#define BROKENBRICK_STATE_COIN	1

class BrokenBrick : public CGameObject
{
	int hiddenItemType;
	bool isBroken = 0;
	vector<LPGAMEOBJECT> fragments;
	LPGAMEOBJECT temp = NULL;
public:
	int trigger = 0;
	int attack = 0;

	BrokenBrick();
	BrokenBrick(int hiddenItemType);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetDisappeared();
	void GetBroken();
	//vector<CGameObject*> Broken();
	//CGameObject* Attack();
	virtual void SetState(int state);

	void SetHiddenItemType(int val);
};
