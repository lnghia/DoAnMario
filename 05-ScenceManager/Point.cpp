#include "Point.h"

Point::Point(unsigned short int point, float x, float y)
{
	interactivable = 0;
	canBeStandOn = 0;
	invisible = 0;

	renderPriority = 999;

	this->point = point;

	vx = 0;
	vy = POINT_SPEED_Y;

	this->x = x;
	this->y = y;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	animation_set = animation_sets->Get(POINT_ANI_SET);

	switch (point) {
		case 100: {
			this->point = POINT_ANI_100;

			break;
		}
		case 200: {
			this->point = POINT_ANI_200;

			break;
		}
		/*case 400: {
			this->point = POINT_ANI_400;

			break;
		}
		case 800: {
			this->point = POINT_ANI_800;

			break;
		}
		case 1000: {
			this->point = POINT_ANI_1000;

			break;
		}
		case 2000: {
			this->point = POINT_ANI_2000;

			break;
		}
		case 4000: {
			this->point = POINT_ANI_4000;

			break;
		}
		case 8000: {
			this->point = POINT_ANI_8000;

			break;
		}
		default:
			break;*/
	}
	currAni = this->point;
}

void Point::Render()
{
	animation_set->at(point)->Render(x, y);
}

void Point::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (!fly_start) {
		fly_start = GetTickCount();
	}

	if (GetTickCount() - fly_start < POINT_FLY_TIME) {
		vy += POINT_GRAVITY * dt;

		CGameObject::Update(dt);

		x += dx;
		y += dy;

		return;
	}
	
	isActive = 0;
	Grid::GetInstance()->ObjIntoTrash(this);
}

void Point::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + POINT_BBOX_WIDTH;
	b = y + POINT_BBOB_HEIGHT;
}
