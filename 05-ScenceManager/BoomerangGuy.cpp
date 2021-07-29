#include "BoomerangGuy.h"
#include "Utils.h"
#include "Grid.h"
#include "Mario.h"
#include "Ground.h"
#include "Wood.h"
#include "PipeHitBox.h"
#include "Brick.h"
#include "QBrick.h"
//#include "Koopas.h"
//#include "RedKoopas.h"

#define OBJECT_TYPE_BOOMERANG 26

BoomerangGuy::BoomerangGuy(float max, float min, CMario* player)
{
	xMax = round(max);
	xMin = round(min);
	SetState(BROS_STATE_WALKING);
	nx = -1;
	LoadBoomerang();

	this->player = player;

	interactivable = 1; invisible = 0; renderPriority = 110;
}

void BoomerangGuy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
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

	if (player) {
		float px, py;

		player->GetPosition(px, py);

		if (px < x) {
			FaceLeft();
		}
		else {
			FaceRight();
		}
	}

	if (state != BROS_STATE_DIE) {
		if (boomerang && (DWORD)GetTickCount64() - start_hold_boomerang >= BROS_THROWBOOMERANGDELAY_TIME) {
			ThrowBoomerang();
		}
		else if ((DWORD)GetTickCount64() - load_start > BROS_LOADBOOMERANG_TIME) {
			//ThrowBoomerang();
			TakeOutBoomerang();
			load_start = (DWORD)GetTickCount64();
		}
	}

	for (UINT i = 0; i < (UINT)coObjects->size(); ++i) {
		if (dynamic_cast<Boomerang*>(coObjects->at(i))) {
			coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), coObjects->at(i)), coObjects->end());

			//float mx, my;

			///*coObjects->at(i)->GetPosition(mx, my);

			//if (mx < x) {
			//	FaceLeft();
			//}
			//else {
			//	FaceRight();
			//}*/

			//if (coObjects->at(i)->GetState() == KOOPAS_STATE_IN_SHELL) {
			//	vy = -BROS_JUMP_SPEED;
			//}
		}
	}

	CGameObject::Update(dt);

	Grid::GetInstance()->clearObjFromGrid(this);

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
		y += min_ty * dy + ny * 0.4f;

		for (int i = 0; i < (int)coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			/*if (dynamic_cast<Boomerang*>(e->obj)) 
			{
				Boomerang* boomerang = dynamic_cast<Boomerang*>(e->obj);
				boomerang->SetInvisible(1);
				boomerang->SetInteractivable(0);
				Grid::GetInstance()->ObjIntoTrash(boomerang);
				//SetState(BROS_STATE_WALKING);
				load_start = (DWORD)GetTickCount64();
			}
			else*/ if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QBrick*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj) || dynamic_cast<Ground*>(e->obj)) {

				if (e->nx) {
					float l, t, r, b;

					e->obj->GetBoundingBox(l, t, r, b);

					if (e->nx) {
						vx *= -1;
					}
				}

				if (ny) {
					vy = 0;
				}
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

	Grid::GetInstance()->putObjectIntoGrid(this);
}



void BoomerangGuy::Render()
{
	int ani = (nx > 0) ? BROS_ANI_WALKING_RIGHT_WITH_BOOMERANG : BROS_ANI_WALKING_LEFT_WITH_BOOMERANG;

	/*if (holdingBoomerang && (DWORD)GetTickCount64() - load_start < 500) {
		ani = (nx > 0) ? BROS_ANI_THREW_RIGHT : BROS_ANI_THREW_LEFT;
	}
	else */if(state == BROS_STATE_DIE) {
		ani = BROS_ANI_DIE;
	}
	else if (!boomerang) {
		ani = (nx > 0) ? BROS_ANI_THREW_RIGHT : BROS_ANI_THREW_LEFT;
		//boomerang->Render();
	}

	currAni = ani;

	if (boomerang) boomerang->Render();
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void BoomerangGuy::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BROS_STATE_DIE:
		vx = 0;
		vy = -BROS_DIE_DEFLECT_SPEED;
		break;
	case BROS_STATE_WALKING:
		vx = BROS_WALKING_SPEED;
		break;
	}

}

void BoomerangGuy::FaceRight()
{
	nx = 1;
	//vx = BROS_WALKING_SPEED;
}

void BoomerangGuy::FaceLeft()
{
	nx = -1;
	//vx = -BROS_WALKING_SPEED;
}

float BoomerangGuy::GetDirection()
{
	return (float)nx;
}

CMario* BoomerangGuy::GetPlayer()
{
	return player;
}

void BoomerangGuy::SetPlayer(CMario* player) {
	this->player = player;
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

	boomerang->UpdateY(by - 1.0f);

	boomerang->SetInteractivable(1);
	boomerang->SetIsHolded(0);

	boomerang = NULL;
}
