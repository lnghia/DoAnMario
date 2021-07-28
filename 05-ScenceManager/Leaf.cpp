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

	/*if ((DWORD)GetTickCount64() - rise_start > LEAF_RISING_TIME)
	{
		rise_start = 0;
		rising = 0;
	}*/

	if (y <= yMax) {
		rise_start = 0;
		rising = 0;
	}

	if (rising)
		y -= LEAF_RISING_SPEED_VY * dt;
	else {
		state = LEAF_STATE_FALLING;
		renderPriority = 102;
		if (!topYFirstTimeUpdated) {
			topY = y;
			topYFirstTimeUpdated = 1;
		}
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
			x += dx;

			if (nx > 0) {
				y = topY + FallingForward(xOrbit++);

				if (xOrbit > 32) {
					topY = y;
					vx = -vx;
					nx = -1;
					xOrbit = 32;
				}
			}
			else {
				y = topY + FallingBackward(xOrbit--);

				if (xOrbit < 0) {
					topY = y;
					vx = -vx;
					nx = 1;
					xOrbit = 0;
				}

			}

			Grid::GetInstance()->putObjectIntoGrid(this);
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			/*x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;*/

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<CMario*>(e->obj)) {
					/*x -= min_tx * dx + nx * 0.4f;
					y -= min_ty * dy + ny * 0.4f;*/

					CMario* mario = dynamic_cast<CMario*>(e->obj);

					GotObsorbed(mario);
					LPGAMEOBJECT point = new Point(LEAF_POINT, x, y);
					grid->putObjectIntoGrid(point);

					if (mario->GetLevel() != MARIO_LEVEL_RACOON && mario->GetLevel() != MARIO_LEVEL_SMALL) {
						mario->SetStartTransforming((DWORD)GetTickCount64());
						mario->BigToRacoon();
					}
				}
				else {
					//x -= min_tx * dx + nx * 0.4f;
					//y -= min_ty * dy + ny * 0.4f;

					x += dx;

					if (nx > 0) {
						y = topY + FallingForward(xOrbit++);

						if (xOrbit > 32) {
							topY = y;
							vx = -vx;
							nx = -1;
							xOrbit = 32;
						}
					}
					else {
						y = topY + FallingBackward(xOrbit--);

						if (xOrbit < 0) {
							topY = y;
							vx = -vx;
							nx = 1;
							xOrbit = 0;
						}
					}
				}
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	/*if (x > xMax || x < xMin)
		vx = -vx;*/

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

float Leaf::FallingForward(int tmp)
{
	return -(pow((float)tmp - 24.0f, 2) / 36.0f - 16.0f);
}

float Leaf::FallingBackward(int tmp)
{
	return -(pow((float)tmp - 8.0f, 2) / 36.0f - 16.0f);
}
