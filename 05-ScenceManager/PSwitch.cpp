#include "PSwitch.h"

void PSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;

	if (state == PSWITCH_STATE_TRIGGER) {
		t = y + (PSWITCH_BBOX_HEIGHT - PSWITCH_BBOX_TRIGGER_HEIGHT);
		r = l + PSWITCH_BBOX_TRIGGER_WIDTH;
		b = t + PSWITCH_BBOX_TRIGGER_HEIGHT;
	}
	else {
		r = l + PSWITCH_BBOX_WIDTH;
		b = t + PSWITCH_BBOX_HEIGHT;
	}
}

void PSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (state == PSWITCH_STATE_APPEAR && (int)((DWORD)GetTickCount64() - start_appear < PSWITCH_APPEAR_TIME)) {
		return;
	}
	else {
		interactivable = 1;
		if (state == PSWITCH_STATE_APPEAR) {
			state = PSWITCH_STATE_NOT_TRIGGER;
		}
	}

	if (state == PSWITCH_STATE_TRIGGER) {
		if ((int)((DWORD)GetTickCount64() - start_trigger > PSWITCH_TRIGGER_TIME)) {
			for (auto& item : affectedBricks) {
				if (item->GetIsActive()) {
					item->SetInteractivable(1);
					item->SetInvisible(0);
				}
			}
			for (auto& item : coins) {
				if (item->GetIsActive()) {
					item->SetInteractivable(0);
					item->SetInvisible(0);
					item->isActive = 0;
				}
			}
		}
	}
}

void PSwitch::Render()
{
	int ani = 0;

	if (state == PSWITCH_STATE_APPEAR) {
		ani = PSWITCH_ANI_APPEAR;
	}
	else if (state == PSWITCH_STATE_NOT_TRIGGER) {
		ani = PSWITCH_ANI_NOT_TRIGGERD;
	}
	else {
		ani = PSWITCH_ANI_TRIGGERED;
	}

	currAni = ani;

	animation_set->at(ani)->Render(x, y);
}

void PSwitch::trigger()
{
	for (auto& item : affectedBricks) {
		if (item->GetIsActive()) {
			item->SetInteractivable(0);
			item->SetInvisible(1);

			FloatingCoin* coin = new FloatingCoin(item->x, item->y);
			coin->SetAnimationSet(CAnimationSets::GetInstance()->Get(COIN_ANI_SET));
			coin->brokenBrickId = item->id;
			coins.push_back(coin);
			Grid::GetInstance()->putObjectIntoGrid(coin);
		}
	}
	state = PSWITCH_STATE_TRIGGER;
	start_trigger = (DWORD)GetTickCount64();
}

void PSwitch::appear()
{
	state = PSWITCH_STATE_APPEAR;
	start_appear = (DWORD)GetTickCount64();
	Grid::GetInstance()->putObjectIntoGrid(this);
}
