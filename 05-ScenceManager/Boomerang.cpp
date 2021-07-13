#include "Boomerang.h"
#include "Utils.h"
#include "Grid.h"

Boomerang::Boomerang(float y, int nx)
{
	originY = y;
	ny = 1;
	this->nx = nx;
	vx = BOOMERANG_SPIN_SPEED_VX * nx;
	vy = BOOMERANG_SPIN_SPEED_VY * ny;

	interactivable = 0;
}

void Boomerang::UpdateY(float val)
{
	originY = y;
	ny = 1;
	nx = 1;
	vx = BOOMERANG_SPIN_SPEED_VX * nx;
	vy = BOOMERANG_SPIN_SPEED_VY * ny;
}

void Boomerang::SetIsHolded(bool val)
{
	isHolded = val;
}

bool Boomerang::GetIsHolded()
{
	return isHolded;
}

void Boomerang::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BOOMERANG_BBOX_WIDTH;
	b = y + BOOMERANG_BBOX_HEIGHT;
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isHolded) {
		return;
	}

	CGame* game = CGame::GetInstance();
	float camx;
	float camy;
	float scrw = float(game->GetScreenWidth());
	float scrh = float(game->GetScreenHeight());
	camx = game->GetCamX();
	camy = game->GetCamY();

	// delete boomerang if out map
	if (x < camx || x > camx + scrw || y < camy || y > camy + scrh) {
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);
		
		return;
	}

	CGameObject::Update(dt);

	if (this->y < originY - 30.0f)					// reach max height then fall
	{
		vx = 0;
		vy += BOOMERANG_GRAVITY_Y * dt;
	}
	if (this->y > originY + 15.0f && vy > 0)		//	reach min height then back to boomerangbros
	{
		nx *= -1;
		vy = 0;
		vx += BOOMERANG_GRAVITY_X * nx * dt;
	}

	x += dx;
	y += dy;

}

void Boomerang::Render()
{
	int ani = (nx > 0);
	animation_set->at(ani)->Render(round(x), round(y));
}

