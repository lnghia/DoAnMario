#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "FireBall.h"
#include "PiranhaPlant.h"
#include "Brick.h"
#include "PipeHitBox.h"
#include "ColorBrickHitBox.h"
#include "QBrick.h"

#include "Map.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > Map::getInstance()->getHeight()) {
		Reset();
	}

	// Calculate dx, dy 
	CGameObject::Update(dt);

	float _dx = dx, _dy = dy;

	Grid::GetInstance()->clearObjFromGrid(this);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		/*Grid::GetInstance()->putObjectIntoGrid(this);
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		return;*/
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;*/

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		float temp = vy;
		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			isStanding = (e->ny < 0) ? e->obj->GetCanBeStandOn() : false;

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								//level = MARIO_LEVEL_SMALL;
								turnIntoSmall();
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<PiranhaPlant*>(e->obj) || dynamic_cast<FireBall*>(e->obj)) {
				if (dynamic_cast<FireBall*>(e->obj)) {
					int tmp = 1;
				}
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						//level = MARIO_LEVEL_SMALL;
						turnIntoSmall();
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj)) {
				

				_dx = _dy = 0;

				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;

				/*Grid::GetInstance()->putObjectIntoGrid(this);
				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

				return;*/
			}
			else if (dynamic_cast<QBrick*>(e->obj)) {
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;

				if (e->ny > 0) {
					QBrick* qBrick = dynamic_cast<QBrick*>(e->obj);

					 qBrick->PopUpHiddenItem();
				}
			}
			else if (dynamic_cast<ColorBrickHitBox*>(e->obj)) {
				if (e->ny < 0) {
					//x -= min_tx * dx + nx * 0.4f;
					//y -= min_ty * dy + ny * 0.4f;


					//y += vy * e->t;
					if (ny) vy = 0;
				}
				else {
					x += _dx;
					y += _dy;
				}
			}
		}
	}

	/*x += _dx;
	y += _dy;*/

	Grid::GetInstance()->putObjectIntoGrid(this);

	/*if (state != MARIO_STATE_DIE) {
		Grid::GetInstance()->putObjectIntoGrid(this);
	}*/

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else {
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
	}


	int alpha = 255;

	if (untouchable) {
		alpha = 128;
		//animation_set->at(ani)->Render(x, y, alpha);
		//while (GetTickCount() - untouchable_start < 200) {
		//	//alpha = (alpha == 255) ? 128 : 255;
		//	/*animation_set->at(ani)->Render(x, y, alpha);*/
		//}
		/*if (GetTickCount() - untouchable_start < 1500 && ani != MARIO_ANI_BIG_TO_SMALL_RIGHT)
			if (state == MARIO_STATE_BIG_TO_SMALL) {
				ani = (nx > 0) ? MARIO_ANI_BIG_TO_SMALL_RIGHT : MARIO_ANI_BIG_TO_SMALL_LEFT;
			}*/
		//ani = (nx > 0) ? MARIO_ANI_SMALL_IDLE_RIGHT : MARIO_ANI_SMALL_IDLE_LEFT;
		/*if (GetTickCount() - untouchable_start < 1500) {
			if (state == MARIO_STATE_BIG_TO_SMALL) {
				ani = (nx > 0) ? MARIO_ANI_BIG_TO_SMALL_RIGHT : MARIO_ANI_BIG_TO_SMALL_LEFT;
				int _y = (aniBigToSmallIndex == -1 || 0) ? y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) : y;
				animation_set->at(ani)->_Render(x, _y, aniBigToSmallIndex, alpha);

				return;
			}
		}*/
	}
	/*else {
		aniBigToSmallIndex = -1;
	}*/

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (!isStanding) {
			return;
		}

		vy = -MARIO_JUMP_SPEED_Y;
		isStanding = false;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

int CMario::GetLevel()
{
	return level;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

DWORD CMario::GetUntouchableStart()
{
	return untouchable_start;
}

bool CMario::GetUntouchable()
{
	return untouchable;
}

void CMario::SetIsStanding(bool val)
{
	isStanding = val;
}

bool CMario::GetIsStanding() {
	return isStanding;
}

void CMario::turnIntoSmall()
{
	level = MARIO_LEVEL_SMALL;
	y += (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
	//SetState(MARIO_STATE_BIG_TO_SMALL);
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

