#include "BrokenBrick.h"
#include "Brick.h"
#include "ObjectCheatSheet.h"

BrokenBrick::BrokenBrick()
{
	SetState(BROKENBRICK_STATE_BRICK);
}

BrokenBrick::BrokenBrick(int hiddenItemType)
{
	SetState(BROKENBRICK_STATE_BRICK);
	this->hiddenItemType = hiddenItemType;
	renderPriority = 10000;
}

void BrokenBrick::Render()
{
	if (fragments.size()) {
		for (auto& fragment : fragments) {
			fragment->Render();
		}

		return;
	}
	else {
		int ani = BROKENBRICK_ANI_BRICK;
		animation_set->at(ani)->Render(x, y);
	}
	
	//RenderBoundingBox();
}

void BrokenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isBroken) {
		return;
	}

	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void BrokenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (fragments.size()) {

		for (auto& fragment : fragments) {
			if (!fragment->isActive) {
				continue;
			}
			
			fragment->Update(dt);
		}
	}
	else {
		if (isBroken) {
			interactivable = 0;
			isActive = 0;
			Grid::GetInstance()->clearObjFromGrid(this);
		}
	}
}

void BrokenBrick::GetDisappeared()
{
	if (hiddenItemType == OBJECT_TYPE_COIN) {
		temp = new FloatingCoin(x, y);
	}

	Grid::GetInstance()->putObjectIntoGrid(temp);
}

void BrokenBrick::GetBroken()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	for (int i = 0; i < 4; ++i) {
		temp = new Fragment(i);
		temp->SetAnimationSet(animation_set);
		temp->SetPosition(x, y);
		//fragments.push_back(temp);
		Grid::GetInstance()->putObjectIntoGrid(temp);
	}

	isBroken = 1;
	Grid::GetInstance()->clearObjFromGrid(this);
	isActive = 0;
	invisible = 1;

	/*if (hiddenItemType == OBJECT_TYPE_COIN) {

	}*/
}

void BrokenBrick::SetState(int state)
{
	CGameObject::SetState(state);
}

void BrokenBrick::SetHiddenItemType(int val)
{
	hiddenItemType = val;
}

//vector<CGameObject*>BrokenBrick::Broken()
//{
//	int ani_set_id = 106;
//	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
//	vector<CGameObject*> VCGameObject;
//	for (int i = 0; i < 4; i++)
//	{
//		/*CGameObject* obj = NULL;
//		obj = new CFragment(i);
//		obj->SetPosition(this->x, this->y);
//		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//		obj->SetAnimationSet(ani_set);
//		VCGameObject.push_back(obj);*/
//	}
//	return VCGameObject;
//}

//CGameObject* BrokenBrick::Attack()		// create attack function
//{
//	int ani_set_id = MARIO_TAIL_ANI_SET_ID;
//	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
//	CGameObject* obj = NULL;
//	float maxRange = this->y - 1.0f;
//	obj = new CBrickAttack(maxRange);
//	obj->type = OBJECT_TYPE_BRICKATTACK;
//	obj->SetPosition(this->x, this->y + BRICK_BBOX_HEIGHT);
//	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//	obj->SetAnimationSet(ani_set);
//	return obj;
//}