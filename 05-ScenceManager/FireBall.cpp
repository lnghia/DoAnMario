#include "FireBall.h"

FireBall::FireBall(int nx, int ny)
{
	vx = PLANTFIREBALL_VX * nx;
	vy = PLANTFIREBALL_VY * ny;

	isActive = 1;
}

void FireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	float camx;
	float camy;
	float scrw = float(game->GetScreenWidth());
	float scrh = float(game->GetScreenHeight());

	camx = game->GetCamX();
	camy = game->GetCamY();

	if (x > camx + scrw || x + PLANTFIREBALL_BBOX_WIDTH < camx ||
		y>camy + scrh || y + PLANTFIREBALL_BBOX_HEIGHT < camy) {
		isActive = 0;
		return;
	}

	CGameObject::Update(dt);

	x += dx;
	y += dy;
}

void FireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PLANTFIREBALL_BBOX_WIDTH;
	b = y + PLANTFIREBALL_BBOX_HEIGHT;
}

void FireBall::Render()
{
	// ani = 1: fireball moves left
	// ani = 2: fireball moves right
	int ani = (vx <= 0);

	animation_set->at(ani)->Render(round(x), round(y));

	RenderBoundingBox();
}
