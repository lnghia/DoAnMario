#include "Coin.h"
#include "ColorBrickHitBox.h"

Coin::Coin(float x, float y)
{
	this->x = containerX = x;
	this->y = y - 16;

	containerY = y;

	vx = COIN_SPEED_X;
	vy = COIN_SPEED_Y;

	renderPriority = 999;
}

void Coin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{


	/*if (!deflect_start) {
		deflect_start = GetTickCount();
	}*/

	Grid* grid = Grid::GetInstance();

	//passedDestrucPoint = (y + COIN_BBOX_HEIGHT <= containerY - 16 && !passedDestrucPoint);
	//if (vy > 0) {
	//	LPGAMEOBJECT obj = new  ColorBrickHitBox(COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT, x, y - COIN_BBOX_HEIGHT + 5);

	//	colliable_objects->push_back(obj);

	//	vector<LPCOLLISIONEVENT> coEvents;
	//	vector<LPCOLLISIONEVENT> coEventsResult;

	//	CalcPotentialCollisions(colliable_objects, coEvents);

	//	if (!coEvents.size() || y+POINT_BBOB_HEIGHT>=) {
	//		vy += COIN_GRAVITY * dt;

	//		float l, t, r, b;

	//		GetBoundingBox(l, t, r, b);

	//		RECT rect;
	//		rect.left = l;
	//		rect.right = r;
	//		rect.top = t;
	//		rect.bottom = b;

	//		CGameObject::Update(dt);

	//		x += dx;
	//		y += dy;

	//		grid->clearObjFromGrid(this);
	//		grid->putObjectIntoGrid(this);

	//		return;
	//	}

	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;

	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	if(vy>0 && ny<0)

	//	x += min_tx * dx + nx * 0.4f;
	//	y += min_ty * dy + ny * 0.4f;

	//	LPGAMEOBJECT point = new Point(COIN_POINT, x, y);

	//	//invisible = 1;
	//	//interactivable = 0;
	//	//grid->clearObjFromGrid(this);
	//	isActive = 0;
	//	grid->putObjectIntoGrid(point);
	//	grid->ObjIntoTrash(this);

	//	/*point->Update(dt);
	//	point->Render();*/

	//	//return;
	//}

	if (y + COIN_BBOX_HEIGHT >= containerY && vy > 0) {
		LPGAMEOBJECT point = new Point(COIN_POINT, x, y);

		//invisible = 1;
		//interactivable = 0;
		//grid->clearObjFromGrid(this);
		isActive = 0;
		grid->putObjectIntoGrid(point);
		grid->ObjIntoTrash(this);

		/*point->Update(dt);
		point->Render();*/

		return;
	}
	else {
		vy += COIN_GRAVITY * dt;

		float l, t, r, b;

		GetBoundingBox(l, t, r, b);

		RECT rect;
		rect.left = l;
		rect.right = r;
		rect.top = t;
		rect.bottom = b;

		CGameObject::Update(dt);

		x += dx;
		y += dy;

		grid->clearObjFromGrid(this);
		grid->putObjectIntoGrid(this);

		return;
	}
}

void Coin::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}


