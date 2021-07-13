#include "BoomerangGuy.h"
#include "Utils.h"
#include "Grid.h"
#include "Mario.h"

#define OBJECT_TYPE_BOOMERANG 26

BoomerangGuy::BoomerangGuy(float max, float min)
{
	xMax = round(max);
	xMin = round(min);
	SetState(BROS_STATE_WALKING);
	nx = -1;
	LoadBoomerang();
}

void BoomerangGuy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + BROS_BOOMERANG_BBOX_WIDTH - BROS_BBOX_WIDTH;
	top = y + BROS_BOOMERANG_BBOX_HEIGHT - BROS_BBOX_HEIGHT;
	right = left + BROS_BBOX_WIDTH;
	bottom = top + BROS_BBOX_HEIGHT;
}

void BoomerangGuy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGame* game = CGame::GetInstance();
	float camx;
	float camy;
	float scrh = float(game->GetScreenHeight());
	float scrw = float(game->GetScreenWidth());
	camx = game->GetCamX();
	camy = game->GetCamY();

	if (y < camy || y > camy + scrh || x > camx + scrw || x + BROS_BBOX_WIDTH / 2 < camx) {
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);

		return;
	}

	//if (state != BROS_STATE_DIE)
	//	if ((DWORD)GetTickCount64() - load_start > BROS_LOADBOOMERANG_TIME)		//load boomerang falling time
	//	{
	//		load_start = 0;
	//		Load = 0;
	//		LoadBoomerang();
	//		SetState(BROS_STATE_WALKING_NO_BOOMERANG);
	//	}

	if (state != BROS_STATE_DIE) {
		if ((DWORD)GetTickCount64() - load_start > 1000) {
			//ThrowBoomerang();
			TakeOutBoomerang();
			ThrowBoomerang();
			load_start = (DWORD)GetTickCount64();
		}
	}

	for (UINT i = 0; i < (UINT)coObjects->size(); ++i) {
		if (dynamic_cast<CMario*>(coObjects->at(i))) {
			float mx, my;

			coObjects->at(i)->GetPosition(mx, my);

			if (mx < x) {
				FaceLeft();
			}
			else {
				FaceRight();
			}
		}
	}

	CGameObject::Update(dt);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	vy += BROS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != BROS_STATE_DIE)
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
		//y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		for (int i = 0; i < (int)coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Boomerang*>(e->obj)) 
			{
				Boomerang* boomerang = dynamic_cast<Boomerang*>(e->obj);
				Grid::GetInstance()->ObjIntoTrash(boomerang);
				SetState(BROS_STATE_WALKING);
				load_start = (DWORD)GetTickCount64();
			}
			/*else if (abs(nx) > 0.0001f)
				vx = -vx;*/
		}
	}

	for (UINT i = 0; i < (int)coEvents.size(); i++) delete coEvents[i];

	if (state != BROS_STATE_DIE)
	{
		if (vx < 0 && x < xMax)
		{
			x = xMax; vx = -vx;
		}
		else if (vx > 0 && x > xMin)
		{
			x = xMin; vx = -vx;
		}
	}

}



void BoomerangGuy::Render()
{
	int ani = (nx > 0) ? BROS_ANI_WALKING_RIGHT : BROS_ANI_WALKING_LEFT;

	if (holdingBoomerang && (DWORD)GetTickCount64() - load_start < 500) {
		ani = (nx > 0) ? BROS_ANI_THREW_RIGHT : BROS_ANI_THREW_LEFT;
	}
	else if(state == BROS_STATE_DIE) {
		ani = BROS_ANI_DIE;
	}

	currAni = ani;

	if (boomerang) {
		ani = (nx > 0) ? BROS_ANI_THREW_RIGHT : BROS_ANI_THREW_LEFT;
		boomerang->Render();
	}

	animation_set->at(ani)->Render(round(x), round(y));
}

void BoomerangGuy::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BROS_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case BROS_STATE_WALKING:
		vx = BROS_WALKING_SPEED;
		break;
	}

}

void BoomerangGuy::FaceRight()
{
	nx = 1;
	vx = BROS_WALKING_SPEED;
}

void BoomerangGuy::FaceLeft()
{
	nx = -1;
	vx = -BROS_WALKING_SPEED;
}

void BoomerangGuy::TakeOutBoomerang()		// create fireball function
{
	int ani_set_id = BOOMERANG_ANI_SET_ID;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	Boomerang* _obj = NULL;
	_obj = new Boomerang(this->y - 8.0f, nx);

	if (nx > 0) {
		_obj->SetPosition(this->x, this->y - 8.0f);
	}
	else {
		_obj->SetPosition(this->x + BROS_BBOX_WIDTH, this->y - 8.0f);
	}
	
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	_obj->SetAnimationSet(ani_set);

	//boomerang = _obj;

	Grid::GetInstance()->putObjectIntoGrid(_obj);

	boomerang = _obj;

	start_hold_boomerang = (DWORD)GetTickCount64();
}

void BoomerangGuy::ThrowBoomerang()
{
	float bx, by;

	boomerang->GetPosition(bx, by);

	boomerang->UpdateY(by - 1);

	boomerang->SetInteractivable(1);
	boomerang->SetIsHolded(0);

	boomerang = NULL;
}
