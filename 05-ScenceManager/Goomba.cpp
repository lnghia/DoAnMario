#include "Goomba.h"
#include "Map.h"
#include "Grid.h"
#include "Brick.h"
#include "ColorBrickHitBox.h"
#include "QBrick.h"
#include "PipeHitBox.h"
#include "Ground.h"
#include "FireBall.h"
#include "PiranhaPlant.h"
#include "RedKoopas.h"

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
	interactivable = 1;
	renderPriority = 101;

}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (y > Map::getInstance()->getHeight()) {
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);
	}

	//deactivateThisIfUnderGround();

	if (state == GOOMBA_STATE_DIE) {
		if ((DWORD)GetTickCount64() - start_die < GOOMBA_BODY_EXIST_TIME) {
			return;
		}
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);
	}
	else if (state == GOOMBA_STATE_GET_HIT) {
		coObjects->clear();
	}

	CGame* game = CGame::GetInstance();

	Grid* grid = Grid::GetInstance();

	grid->clearObjFromGrid(this);

	vy += GOOMBA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CGameObject::Update(dt);

	float _dx = dx;
	float _dy = dy;

	for (UINT i = 0; i < coObjects->size(); ++i) {
		LPGAMEOBJECT tmp = coObjects->at(i);

		if (dynamic_cast<FireBall*>(coObjects->at(i)) || dynamic_cast<PiranhaPlant*>(coObjects->at(i)) || dynamic_cast<CBrick*>(coObjects->at(i))) {
			coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), tmp), coObjects->end());
		}
	}

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0) {
		x += dx;
		y += dy;

		Grid::GetInstance()->putObjectIntoGrid(this);
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		return;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			float bx, by;

			e->obj->GetPosition(bx, by);
			
			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QBrick*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj) || dynamic_cast<Ground*>(e->obj) || dynamic_cast<CGoomba*>(e->obj)) {

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
			/*else if (dynamic_cast<RedKoopas*>(e->obj)) {
				RedKoopas* koopas = dynamic_cast<RedKoopas*>(e->obj);

				if (koopas->GetBeingHolded()) {
					SetState(GOOMBA_STATE_DIE);
				}
				else {
					if (e->nx) {
						vx *= -1;
					}
					else if (e->ny) {
						y -= min_ty * dy + ny * 0.4f;
					}
				}
			}*/
			//else if (dynamic_cast<ColorBrickHitBox*>(e->obj)) {
			//	x -= min_tx * dx + nx * 0.4f;
			//	y -= min_ty * dy + ny * 0.4f;
			//	x += _dx;

			//	if (e->ny < 0) {
			//		vy = 0;
			//	}
			//	else {
			//		x += _dx;
			//		y += _dy;
			//	}
			//}
		}
	}

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	
	if (isActive) grid->putObjectIntoGrid(this);

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_GET_HIT) {
		ani = GOOMBA_ANI_GET_HIT;
	}

	currAni = ani;

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1);
			interactivable = 0;
			start_die = (DWORD)GetTickCount64();
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}

void CGoomba::GetHit()
{
	state = GOOMBA_STATE_GET_HIT;
	vx = 0;
	vy = -GOOMBA_DIE_DEFLECT_SPEED;
	interactivable = 0;
}
