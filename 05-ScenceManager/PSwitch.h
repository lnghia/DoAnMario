#pragma once
#include "GameObject.h"
#include "BrokenBrick.h"

class PSwitch : public CGameObject
{
	vector<BrokenBrick*> affectedBricks;
private:
	PSwitch() {};
	
};

