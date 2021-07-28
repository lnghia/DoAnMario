#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(int camW, int camH)
{
}

void GameOverMenu::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + GAMEOVER_MENU_BBOX_WIDTH;
	b = t + GAMEOVER_MENU_BBOX_HEIGHT;
}

void GameOverMenu::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void GameOverMenu::Render()
{
	animation_set->at(option)->Render(x, y);
}

void GameOverMenu::UpdatePos(float x, float y)
{
	this->x = x;
	this->y = y;
}
