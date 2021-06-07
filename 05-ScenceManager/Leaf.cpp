#include "Leaf.h"
#include "Utils.h"
#include "Map.h"
#include "Grid.h"

void Leaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LEAF_BBOX_WIDTH;
	b = y + LEAF_BBOX_HEIGHT;
}

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	//deactivateThisIfUnderGround();

	Grid* grid = Grid::GetInstance();

	if (y > Map::getInstance()->getHeight()) {
		interactivable = 0;
		isActive = 0;
		grid->clearObjFromGrid(this);
	}

	grid->clearObjFromGrid(this);

	if (GetTickCount() - rise_start > LEAF_RISING_TIME)
	{
		rise_start = 0;
		rising = 0;
	}

	if (rising)
		y -= LEAF_RISING_SPEED_VY;
	else {
		state = LEAF_STATE_FALLING;
		renderPriority = 102;
	}

	if (state == LEAF_STATE_FALLING)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CGameObject::Update(dt);

		float _dx = dx;
		float _dy = dy;

		CalcPotentialCollisions(colliable_objects, coEvents);

		if (coEvents.size() == 0)
		{
			x += _dx;
			y += _dy;

			Grid::GetInstance()->putObjectIntoGrid(this);
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

				if (dynamic_cast<CMario*>(e->obj)) {
					x -= min_tx * dx + nx * 0.4f;
					y -= min_ty * dy + ny * 0.4f;

					CMario* mario = dynamic_cast<CMario*>(e->obj);

					GotObsorbed(mario);
					LPGAMEOBJECT point = new Point(LEAF_POINT, x, y);
					grid->putObjectIntoGrid(point);

					if (mario->GetLevel() != MARIO_LEVEL_RACOON && mario->GetLevel() != MARIO_LEVEL_SMALL) {
						mario->SetStartTransforming(GetTickCount());
						mario->BigToRacoon();
					}
				}
				else {
					/*x -= min_tx * dx + nx * 0.4f;
					y -= min_ty * dy + ny * 0.4f;*/

					x += _dx;
					y += _dy;
				}
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	if (x > xMax || x < xMin)
		vx = -vx;

	if (isActive) grid->putObjectIntoGrid(this);
}

void Leaf::Render()
{
	int ani = LEAF_FALL_RIGHT;
	if (vx >= 0) ani = LEAF_FALL_LEFT;
	animation_set->at(ani)->Render(round(x), round(y));
}


void Leaf::SetState(int state)
{
	CGameObject::SetState(state);
}