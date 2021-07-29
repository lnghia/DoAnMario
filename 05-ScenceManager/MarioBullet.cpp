#include "MarioBullet.h"
#include "Mario.h"
#include "Ground.h"
#include "Koopas.h"
#include "BangEffect.h"
#include "BoomerangGuy.h"
#include "Board.h"
#include "BrokenBrick.h"
#include "RedKoopas.h"
#include "PiranhaPlant.h"
#include "PiranhaFlower.h"


MarioBullet::MarioBullet(int nx, int ny)
{
	vx = MARIOFIREBALL_VX * nx;
	vy = MARIOFIREBALL_VY * ny;

	isActive = 1;
	interactivable = 1;

	renderPriority = 9999;
}

void MarioBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	float camx;
	float camy;
	float scrw = float(game->GetScreenWidth());
	float scrh = float(game->GetScreenHeight());

	camx = game->GetCamX();
	camy = game->GetCamY();

	if (x > camx + scrw || x + PLANTFIREBALL_BBOX_WIDTH < camx ||
		y>camy + scrh || y + PLANTFIREBALL_BBOX_HEIGHT < camy || (hitTarget && (int)((DWORD)GetTickCount64() - start_hit_target) > MARIOFIREBALL_HIT_TARGET_TIME)) {
		isActive = 0;
		Grid::GetInstance()->ObjIntoTrash(this);
		return;
	}

	if (hitTarget) {
		return;
	}

	Grid::GetInstance()->clearObjFromGrid(this);

	vy += MARIOFIREBALL_GRAVITY * dt;

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

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QBrick*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj) || dynamic_cast<Ground*>(e->obj) || dynamic_cast<BrokenBrick*>(e->obj) || dynamic_cast<QBrick*>(e->obj)) {
				if (e->ny) {
					vy *= -1;
				}
				if (e->nx) {
					vx = 0;
					vy = 0;
					hitTarget = 1;
					start_hit_target = (DWORD)GetTickCount64();
					interactivable = 0;
				}
			}
			else if (dynamic_cast<ColorBrickHitBox*>(e->obj)) {
				if (e->ny < 0) {
					vy *= -1;
				}
				else {
					x -= min_tx * dx + nx * 0.4f;
					y -= min_ty * dy + ny * 0.4f;

					x += _dx;
					y += _dy;
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) {
				CGoomba* _obj = dynamic_cast<CGoomba*>(e->obj);

				vx = 0;
				vy = 0;
				hitTarget = 1;
				start_hit_target = (DWORD)GetTickCount64();
				interactivable = 0;

				_obj->GetHit((int)nx);
				LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
			}
			else if (dynamic_cast<BoomerangGuy*>(e->obj)) {
				BoomerangGuy* _obj = dynamic_cast<BoomerangGuy*>(e->obj);

				vx = 0;
				vy = 0;
				hitTarget = 1;
				start_hit_target = (DWORD)GetTickCount64();
				interactivable = 0;

				_obj->SetState(BROS_STATE_DIE);
				LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
			}
			else if (dynamic_cast<RedKoopas*>(e->obj)) {
				RedKoopas* _obj = dynamic_cast<RedKoopas*>(e->obj);

				vx = 0;
				vy = 0;
				hitTarget = 1;
				start_hit_target = (DWORD)GetTickCount64();
				interactivable = 0;

				_obj->SetState(KOOPAS_STATE_DIE);
				LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* _obj = dynamic_cast<CKoopas*>(e->obj);

				vx = 0;
				vy = 0;
				hitTarget = 1;
				start_hit_target = (DWORD)GetTickCount64();
				interactivable = 0;

				_obj->SetState(KOOPAS_STATE_DIE);

				LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
			}
			else if (dynamic_cast<PiranhaFlower*>(e->obj)) {
				e->obj->SetState(PIRANHAFLOWER_STATE_DIE);

				vx = 0;
				vy = 0;
				hitTarget = 1;
				start_hit_target = (DWORD)GetTickCount64();
				interactivable = 0;

				LPGAMEOBJECT point = new Point(PIRANHAPLANT_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(PIRANHAPLANT_POINT);
			}
			else if (dynamic_cast<PiranhaPlant*>(e->obj)) {
				e->obj->SetState(PIRANHAPLANT_STATE_DIE);

				vx = 0;
				vy = 0;
				hitTarget = 1;
				start_hit_target = (DWORD)GetTickCount64();
				interactivable = 0;

				LPGAMEOBJECT point = new Point(PIRANHAPLANT_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(PIRANHAPLANT_POINT);
			}
		}
	}

	Grid::GetInstance()->putObjectIntoGrid(this);
}

void MarioBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PLANTFIREBALL_BBOX_WIDTH;
	b = y + PLANTFIREBALL_BBOX_HEIGHT;
}

void MarioBullet::Render()
{
	currAni = (vx <= 0);

	if (hitTarget) {
		currAni = MARIOFIREBALL_ANI_HIT_TARGET;
	}

	CAnimationSets::GetInstance()->Get(FIREBALL_ANI_SET)->at(currAni)->Render(x, y);
}

void MarioBullet::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}
