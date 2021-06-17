#include "SpeedBar.h"
#include "Utils.h"

SpeedBar::SpeedBar(float x, float y, CMario* player)
{
	this->x = x;
	this->y = y;
	this->player = player;
}

void SpeedBar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + SPEEDBAR_BBOX_WIDTH;
	b = t + SPEEDBAR_BBOX_HEIGHT;
}

void SpeedBar::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	step = (player->GetIsRunning() && player->GetIsStanding()) ? 1 : -1;

	if ((DWORD)GetTickCount64() - start_running >= 100) {
		level += step;
		start_running = (DWORD)GetTickCount64();
	}

	if (level >= (short int)animation_set->size()) {
		level = (short int)animation_set->size() - 1;
	}
	else if (level < 0) {
		level = 0;
	}
	DebugOut(L"[DEBUG] %d %d\n", level, step);
}

void SpeedBar::Render()
{
	animation_set->at(level)->Render(x, y);
}

void SpeedBar::UpdatePos(float x, float y)
{
	this->x = x;
	this->y = y;
}

void SpeedBar::SetAniSet(int ani_set)
{
	animation_set = CAnimationSets::GetInstance()->Get(ani_set);
}

void SpeedBar::SetPlayer(CMario* player)
{
	this->player = player;
}
