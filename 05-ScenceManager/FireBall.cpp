#include "FireBall.h"
#include "Grid.h"
#include "Mario.h"

FireBall::FireBall(int nx, int ny)
{
	vx = PLANTFIREBALL_VX * nx;
	vy = PLANTFIREBALL_VY * ny;

	isActive = 1;
	interactivable = 1;

	renderPriority = 9999;
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
		//Grid::GetInstance()->ObjIntoTrash(this);
		return;
	}

	Grid::GetInstance()->clearObjFromGrid(this);

	CGameObject::Update(dt);

	float _dx = dx, _dy = dy;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		Grid::GetInstance()->putObjectIntoGrid(this);

		return;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;*/

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CMario*>(e->obj)) {
				CMario* mario = dynamic_cast<CMario*>(e->obj);

				if (!mario->GetUntouchable()) {
					_dx = dx;
					_dy = dy;

					mario->SetSpeed(0, 0);

					if (mario->GetLevel() > MARIO_LEVEL_SMALL)
					{
						//mario->SetLevel(MARIO_LEVEL_SMALL);
						mario->SetBackupLevel(MARIO_LEVEL_SMALL);
						mario->SetBackupState(mario->GetState());
						mario->SetStartTransforming(GetTickCount());
						mario->turnIntoSmall();
						mario->StartUntouchable();
					}
					else
						mario->SetState(MARIO_STATE_DIE);
				}
			}
		}
	}

	x += _dx;
	y += _dy;

	Grid::GetInstance()->putObjectIntoGrid(this);
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
	currAni = (vx <= 0);

	animation_set->at(currAni)->Render(round(x), round(y));

	//RenderBoundingBox();
}

void FireBall::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}
