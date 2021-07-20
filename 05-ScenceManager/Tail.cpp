#include "Tail.h"
#include "Game.h"
#include "Map.h"
#include "Grid.h"
#include "RedKoopas.h"
#include "Goomba.h"
#include "Board.h"
#include "BangEffect.h"

Tail::Tail(int nx, float maxX)
{
	invisible = 0;
	interactivable = 1;
	isActive = 1;
	vx = TAIL_SPEED * nx;
	this->nx = nx;
	this->maxX = maxX;

	animation_set = NULL;
}

void Tail::RenderCurrFrame()
{
}

void Tail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TAIL_BBOX_WIDTH;
	b = y + TAIL_BBOX_HEIGHT;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > Map::getInstance()->getHeight() || (x >= maxX && nx > 0) || (x <= maxX && nx < 0)) {
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);
		Grid::GetInstance()->ObjIntoTrash(this);
	}

	CGame* game = CGame::GetInstance();

	Grid::GetInstance()->clearObjFromGrid(this);

	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += dx;
		y += dy;

		for (int i = 0; i < int(coEventsResult.size()); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e->obj))						// object is goomba
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_GET_HIT) {
					float gX, gY;

					goomba->GetHit(nx * -1);
					goomba->GetPosition(gX, gY);

					BangEffect* bangEffect = new BangEffect();
					bangEffect->SetPosition(gX, gY);
					Grid::GetInstance()->putObjectIntoGrid(bangEffect);

					LPGAMEOBJECT point = new Point(MUSHROOM_POINT, gX, gY);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
				}
			}
			//else if (dynamic_cast<CPlant*>(e->obj))					// obj is plant
			//{
			//	CPlant* plant = dynamic_cast<CPlant*>(e->obj);
			//	plant->SetState(PLANT_STATE_DIE);
			//	game->AddScore(100);
			//}
			//else if (dynamic_cast<CPiranhaPlant*>(e->obj))			 // obj is PiranhaPlant
			//{
			//	CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
			//	plant->SetState(PIRANHAPLANT_STATE_DIE);
			//	game->AddScore(100);

			//}
			//else if (dynamic_cast<CFlyGoomba*>(e->obj))				// object is Flygoomba
			//{
			//	CFlyGoomba* goomba = dynamic_cast<CFlyGoomba*>(e->obj);
			//	goomba->SetState(FLYGOOMBA_STATE_DIE_DEFLECT);
			//	goomba->vx = 0.05f * this->nx;
			//	game->AddScore(100);
			//}
			//else if (dynamic_cast<RedKoopas*>(e->obj))				// object is koopas
			//{
			//	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
			//	koopas->SetState(KOOPAS_STATE_DIE_DEFLECT);
			//	game->AddScore(100);
			//}
			//else if (dynamic_cast<CMushRoom*>(e->obj))				// object is koopas
			//{
			//	CMushRoom* mushroom = dynamic_cast<CMushRoom*>(e->obj);
			//	if (mushroom->GetState() == MUSHROOM_STATE_MOVING)
			//		mushroom->SetState(MUSHROOM_STATE_FLY);
			//}
			//else if (dynamic_cast<CFlyKoopas*>(e->obj))				// object is Flykoopas
			//{
			//	CFlyKoopas* koopas = dynamic_cast<CFlyKoopas*>(e->obj);
			//	koopas->SetState(FLYKOOPAS_STATE_DIE_DEFLECT);
			//	game->AddScore(100);
			//}
			//else if (dynamic_cast<CQBrick*>(e->obj))				// object is Question Brick
			//{
			//	CQBrick* brick = dynamic_cast<CQBrick*>(e->obj);
			//	if (brick->GetState() != BRICK_STATE_EMP)
			//	{
			//		brick->trigger = 1;
			//		brick->StartRinging();
			//		brick->stack--;
			//		if (brick->stack == 0)
			//			brick->SetState(BRICK_STATE_EMP);
			//	}
			//}
			//else if (dynamic_cast<CBrokenBrick*>(e->obj))				// object is Broken Brick
			//{
			//	CBrokenBrick* brick = dynamic_cast<CBrokenBrick*>(e->obj);
			//	if (brick->GetState() == BROKENBRICK_STATE_BRICK)
			//	{
			//		brick->trigger = 1;
			//		brick->isFinish = 1;
			//	}
			//}
			//else if (dynamic_cast<CBoomerangBros*>(e->obj))				// object is Boomerang bros
			//{
			//	isFinish = 1;
			//	CBoomerangBros* bros = dynamic_cast<CBoomerangBros*>(e->obj);
			//	bros->SetState(BROS_STATE_DIE);
			//	game->AddScore(1000);
			//}
		}
	}

	Grid::GetInstance()->putObjectIntoGrid(this);

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Tail::Render()
{
	//animation_set->at(0)->Render(round(x), round(y));
	RenderBoundingBox();
}