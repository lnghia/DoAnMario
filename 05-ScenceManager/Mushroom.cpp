#include "Mushroom.h"
#include "PlayScence.h"
#include "Map.h"
#include "FloatingCoin.h"


void Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	r = x + MUSHROOM_BBOX_WIDTH;
	t = y;
	b = y + MUSHROOM_BBOX_HEIGHT;
}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (y > Map::getInstance()->getHeight()) {
		interactivable = 0;
		isActive = 0;
		Grid::GetInstance()->clearObjFromGrid(this);
	}

	//deactivateThisIfUnderGround();

	CGame* game = CGame::GetInstance();

	Grid* grid = Grid::GetInstance();

	if (state == MUSHROOM_STATE_APPEAR) {
		CGameObject::Update(dt);

		if (y + dy > containerY - 1 - MUSHROOM_BBOX_HEIGHT) {
			x += dx;
			y += dy;

			return;
		}
		else {
			state = MUSHROOM_STATE_RUN;
			y = containerY - 1 - MUSHROOM_BBOX_HEIGHT;
			
			if (dynamic_cast<CPlayScene*>(game->GetCurrentScene())) {
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(game->GetCurrentScene());

				float pX, pY;

				playScene->GetPlayer()->GetPosition(pX, pY);

				vx = (pX < x) ? MUSHROOM_MOVING_RIGHT : MUSHROOM_MOVING_LEFT;
				vy = 0;
			}

			grid->putObjectIntoGrid(this);
		}
	}

	grid->clearObjFromGrid(this);

	vy += MUSHROOM_GRAVITY * dt;

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

			if (dynamic_cast<CMario*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				CMario* mario = dynamic_cast<CMario*>(e->obj);

				GotObsorbed(mario);
				LPGAMEOBJECT point = new Point(MUSHROOM_POINT, x, y);
				grid->putObjectIntoGrid(point);

				if (mario->GetLevel() != MARIO_LEVEL_BIG) {
					mario->SetBackupLevel(MARIO_LEVEL_BIG);
					mario->SetBackupState(mario->GetState());
					mario->SetStartTransforming((DWORD)GetTickCount64());
					mario->turnIntoBig();
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QBrick*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj) || dynamic_cast<Ground*>(e->obj)) {

				if (e->nx) {
					float l, t, r, b;

					e->obj->GetBoundingBox(l, t, r, b);

					if ((y >= t && y <= b) || (y + MUSHROOM_BBOX_HEIGHT >= t && y + MUSHROOM_BBOX_HEIGHT <= b)) {
						if (e->nx < 0) {
							vx = MUSHROOM_MOVING_LEFT;
						}
						else if (e->nx > 0) {
							vx = MUSHROOM_MOVING_RIGHT;
						}
					}
				}

				if (ny) vy = 0;
			}
			else if (dynamic_cast<ColorBrickHitBox*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;
				x += _dx;

				if (e->ny < 0) {
					vy = 0;
				}
				else {
					x += _dx;
					y += _dy;
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;

				x += _dx;
				//y += _dy;
			}
			else if (dynamic_cast<FloatingCoin*>(e->obj)) {
				if (e->nx) {
					x -= min_tx * dx + nx * 0.4f;
				}
				else if (e->ny) {
					y -= min_ty * dy + ny * 0.4f;
				}

				x += _dx;
				y += _dy;
			}
		}
	}

	if(isActive) grid->putObjectIntoGrid(this);

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mushroom::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}


