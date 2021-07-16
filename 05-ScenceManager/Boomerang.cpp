#include "Boomerang.h"
#include "Utils.h"
#include "Grid.h"
#include "Animations.h"
#include "BoomerangGuy.h"

Boomerang::Boomerang(float y, int nx)
{
	originY = y;
	ny = 1;
	this->nx = nx;
	vx = BOOMERANG_SPIN_SPEED_VX * nx;
	vy = BOOMERANG_SPIN_SPEED_VY * ny;

	interactivable = 0;
	renderPriority = 101;

	start_flying = (DWORD)GetTickCount64();
}

void Boomerang::UpdateY(float val)
{
	originY = y;
	ny = 1;
	/*nx = 1;
	vx = BOOMERANG_SPIN_SPEED_VX * nx;
	vy = BOOMERANG_SPIN_SPEED_VY * ny;*/
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

	float _dx = dx;
	float _dy = dy;

	Grid::GetInstance()->clearObjFromGrid(this);

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

		// todo: this is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (int i = 0; i < (int)coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<BoomerangGuy*>(e->obj) && (DWORD)GetTickCount64() - start_flying > BOOMERANG_BEINGHOLDED_TIME)
			{
				BoomerangGuy* guy = dynamic_cast<BoomerangGuy*>(e->obj);

				isActive = 0;
				invisible = 1;
				Grid::GetInstance()->ObjIntoTrash(this);
				//guy->SetState(BROS_STATE_WALKING);
				guy->load_start = (DWORD)GetTickCount64();
			}
			else {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				x += _dx;
				y += _dy;
			}
		}
	}

	for (UINT i = 0; i < (int)coEvents.size(); i++) delete coEvents[i];

	Grid::GetInstance()->putObjectIntoGrid(this);
}

void Boomerang::Render()
{
	int ani = (nx > 0);

	if (isHolded) {
		ani = (nx > 0) ? BOOMERANG_ANI_BEING_HOLDED_RIGHT : BOOMERANG_ANI_BEING_HOLDED_LEFT;
	}

	CAnimationSets::GetInstance()->Get(BOOMERANG_ANI_SET)->at(ani)->Render(x, y);
	//animation_set->at(ani)->Render(round(x), round(y));
}

