#include "Fragment.h"
#include "Game.h"

Fragment::Fragment(int side)
{
	//SetState(FIREBALL_STATE_SPIN);
	ny = -1;
	if (side == 0)							// fragment right tall
	{
		nx = 1;
		vx = FRAGMENT_SPIN_SPEED_VX * nx;
		vy = FRAGMENT_SPIN_SPEED_VY * ny;
	}
	else if (side == 1)						// fragment right short
	{
		nx = 1;
		vx = FRAGMENT_SPIN_SPEED_VX * nx;
		vy = FRAGMENT_SPIN_SPEED_VY1 * ny;

	}
	else if (side == 2)						// fragment left tall
	{
		nx = -1;
		vx = FRAGMENT_SPIN_SPEED_VX * nx;
		vy = FRAGMENT_SPIN_SPEED_VY * ny;
	}
	else if (side == 3)						// fragment left short
	{
		nx = -1;
		vx = FRAGMENT_SPIN_SPEED_VX * nx;
		vy = FRAGMENT_SPIN_SPEED_VY1 * ny;
	}

	interactivable = 0;
	renderPriority = 99;
	currAni = 1;
}

void Fragment::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + FRAGMENT_WIDTH_BBOX;
	b = t + FRAGMENT_HEIGHT_BBOX;
}

void Fragment::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > Map::getInstance()->getHeight()) {
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);
	}

	CGame* game = CGame::GetInstance();
	float camx = game->GetCamX();
	float camy = game->GetCamY();
	float scrw = float(game->GetScreenWidth());
	float scrh = float(game->GetScreenHeight());

	CGameObject::Update(dt);

	vy += FRAGMENT_GRAVITY * dt;

	x += dx;
	y += dy;
}

void Fragment::Render()
{
	animation_set->at(1)->Render(round(x), round(y));
}