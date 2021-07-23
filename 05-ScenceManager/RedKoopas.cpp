#include "RedKoopas.h"
#include "Utils.h"
#include "Board.h"
#include "Mario.h"
#include "BrokenBrick.h"
#include "BoomerangGuy.h"
#include "BangEffect.h"

RedKoopas::RedKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	interactivable = 1;
	renderPriority = 101;
}

RedKoopas::RedKoopas(short int nx)
{
	SetState(KOOPAS_STATE_WALKING);
	interactivable = 1;
	renderPriority = 101;

	this->nx = nx;
	initNx = nx;

	if (nx > 0) {
		SetState(KOOPAS_STATE_WALKING_RIGHT);
		vx = KOOPAS_WALKING_SPEED;
	}
	else {
		SetState(KOOPAS_STATE_WALKING_LEFT);
		vx = -KOOPAS_WALKING_SPEED;
	}

	//vx = (nx > 0) ? KOOPAS_WALKING_SPEED : -KOOPAS_WALKING_SPEED;
}

RedKoopas::RedKoopas(short int nx, const int& x, const int& y, const int& w, const int& h)
{
	SetState(KOOPAS_STATE_WALKING);
	interactivable = 1;
	renderPriority = 101;
	canBeStandOn = 1;

	this->nx = nx;
	initNx = nx;

	if (nx > 0) {
		SetState(KOOPAS_STATE_WALKING_RIGHT);
		vx = KOOPAS_WALKING_SPEED;
	}
	else {
		SetState(KOOPAS_STATE_WALKING_LEFT);
		vx = -KOOPAS_WALKING_SPEED;
	}

	//vx = (nx > 0) ? KOOPAS_WALKING_SPEED : -KOOPAS_WALKING_SPEED;

	beneathSurfaceX = x;
	beneathSurfaceY = y;
	beneathSurfaceW = w;
	beneathSurfaceH = h;
}

void RedKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_OUT_A_SHELL)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void RedKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > Map::getInstance()->getHeight()) {
		/*interactivable = 0;
		isActive = 0;*/
		Grid::GetInstance()->clearObjFromGrid(this);
		x = initX;
		y = initY;
		Grid::GetInstance()->putObjectIntoGrid(this);
		invisible = 1;
		harmless = 1;
		return;
	}
	if (beingHolded) {
		return;
	}

	/*if (state == KOOPAS_STATE_DIE) {
		if ((DWORD)GetTickCount64() - spawn_delay < KOOPAS_DIE_TIME) {
			return;
		}
		Spawn();
	}
	else */if (state == KOOPAS_STATE_IN_SHELL) {
		if ((DWORD)GetTickCount64() - in_shell > 5000) {
			OutShell();
		}
	}
	else if (outShell) {
		if ((DWORD)GetTickCount64() - out_shell_start < 1000) {
			vx *= -1;
		}
		else {
			FinishOutShell();
		}
	}

	//DebugOut(L"%d %d\n", state, coEventsResult.size());
	if (state == 3) {
		int tmp = 1;
	}



	//unique_ptr<CGame> game = CGame::GetInstance();

	CGame* game = CGame::GetInstance();

	Grid* grid = Grid::GetInstance();

	grid->clearObjFromGrid(this);

	vy += KOOPAS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT || state == KOOPAS_STATE_WALKING) {
		float l, r, t, b;

		GetBoundingBox(l, t, r, b);

		if (l <= beneathSurfaceX - (r - l) / 2 || r >= beneathSurfaceX + beneathSurfaceW + (r - l) / 2) {
			//vx *= -1;
			int tmp = 1;
			if (nx > 0) {
				SetState(KOOPAS_STATE_WALKING_LEFT);
				x = beneathSurfaceX + beneathSurfaceW - (r - l) / 2;
				//l = beneathSurfaceX - (r - l) / 2;
			}
			else {
				SetState(KOOPAS_STATE_WALKING_RIGHT);
				//r = beneathSurfaceX + beneathSurfaceW + (r - l) / 2;
				x = beneathSurfaceX - (r - l) / 2;
			}
		}
	}

	CGameObject::Update(dt);

	float _dx = dx;
	float _dy = dy;

	for (UINT i = 0; i < coObjects->size(); ++i) {
		LPGAMEOBJECT tmp = coObjects->at(i);

		if (dynamic_cast<FireBall*>(coObjects->at(i)) || dynamic_cast<PiranhaPlant*>(coObjects->at(i)) || dynamic_cast<CBrick*>(coObjects->at(i)) || dynamic_cast<CMario*>(coObjects->at(i))) {
			coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), tmp), coObjects->end());
		}
	}

	CalcPotentialCollisions(coObjects, coEvents);

	if (!coEvents.size()) {
		x += dx;
		y += dy;

		Grid::GetInstance()->putObjectIntoGrid(this);
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		//float sT, sB, sL, sR;
		//float l, t, r, b;

		/*beneathSurface->GetBoundingBox(sL, sT, sR, sB);
		GetBoundingBox(l, t, r, b);

		if (l <= sL || r >= sR) {
			vx *= -1;
		}*/

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

		for (UINT i = 0; i < (UINT)coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			isStanding = (e->ny < 0) ? e->obj->GetCanBeStandOn() : false;

			float bx, by;

			e->obj->GetPosition(bx, by);

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj) || dynamic_cast<Ground*>(e->obj)) {

				if (e->nx) {
					float l, t, r, b;

					e->obj->GetBoundingBox(l, t, r, b);

					if (e->nx) {
						if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT) {
							if (vx > 0) {
								SetState(KOOPAS_STATE_WALKING_LEFT);
							}
							else {
								SetState(KOOPAS_STATE_WALKING_RIGHT);
							}
						}
						else if (state == KOOPAS_STATE_SPIN) {
							vx *= -1;
						}

						//vx *= -1;
					}
				}

				if (ny) {
					vy = 0;
					if (state == KOOPAS_STATE_IN_SHELL) {
						vx = 0;
					}
				}
			}
			else if (dynamic_cast<ColorBrickHitBox*>(e->obj)) {
				if (e->ny < 0) {
					vy = 0;
					if (state == KOOPAS_STATE_IN_SHELL) {
						vx = 0;
					}
				}
				else {
					x -= min_tx * dx + nx * 0.4f;
					y -= min_ty * dy + ny * 0.4f;

					x += _dx;
					y += _dy;
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) {
				if (e->nx) {
					float l, t, r, b;

					e->obj->GetBoundingBox(l, t, r, b);

					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

					if (beingHolded || state == KOOPAS_STATE_SPIN && goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_GET_HIT) {
						float gX, gY;

						goomba->GetHitByShell();
						goomba->GetPosition(gX, gY);
						goomba->SetVy(-GOOMBA_DIE_GET_HIT_BY_SHELL_DEFLECT_SPEED);

						BangEffect* bangEffect = new BangEffect();
						bangEffect->SetPosition(gX, gY);
						Grid::GetInstance()->putObjectIntoGrid(bangEffect);

						LPGAMEOBJECT point = new Point(MUSHROOM_POINT, gX, gY);
						Grid::GetInstance()->putObjectIntoGrid(point);
						Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
					}
					else {
						if (e->nx) {
							if (vx > 0) {
								SetState(KOOPAS_STATE_WALKING_LEFT);
							}
							else {
								SetState(KOOPAS_STATE_WALKING_RIGHT);
							}
							//vx *= -1;
						}
					}
				}
				else if (e->ny) {
					float l, t, r, b;

					e->obj->GetBoundingBox(l, t, r, b);

					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

					if (beingHolded || state == KOOPAS_STATE_SPIN && goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_GET_HIT) {
						float gX, gY;

						goomba->GetHitByShell();
						goomba->GetPosition(gX, gY);

						BangEffect* bangEffect = new BangEffect();
						bangEffect->SetPosition(gX, gY);
						Grid::GetInstance()->putObjectIntoGrid(bangEffect);

						LPGAMEOBJECT point = new Point(MUSHROOM_POINT, gX, gY);
						Grid::GetInstance()->putObjectIntoGrid(point);
						Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
					}
				}
			}
			else if (dynamic_cast<QBrick*>(e->obj)) {
				if (e->nx) {
					if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT) {
						if (vx > 0) {
							SetState(KOOPAS_STATE_WALKING_LEFT);
						}
						else {
							SetState(KOOPAS_STATE_WALKING_RIGHT);
						}
					}
					else if (state == KOOPAS_STATE_SPIN) {
						vx *= -1;
						dynamic_cast<QBrick*>(e->obj)->PopUpHiddenItem();
					}
				}
			}
			else if (dynamic_cast<BrokenBrick*>(e->obj)) {
				BrokenBrick* brokenBrick = dynamic_cast<BrokenBrick*>(e->obj);

				if (e->nx) {
					float l, t, r, b;

					e->obj->GetBoundingBox(l, t, r, b);

					if (e->nx) {
						if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT) {
							if (vx > 0) {
								SetState(KOOPAS_STATE_WALKING_LEFT);
							}
							else {
								SetState(KOOPAS_STATE_WALKING_RIGHT);
							}
						}
						else if (state == KOOPAS_STATE_SPIN) {
							vx *= -1;
							brokenBrick->GetBroken();
						}

						//vx *= -1;
					}
				}

				if (ny) {
					vy = 0;
				}
			}
			else if (dynamic_cast<BoomerangGuy*>(e->obj)) {
				BoomerangGuy* guy = dynamic_cast<BoomerangGuy*>(e->obj);

				if (state == KOOPAS_STATE_SPIN && (vx > 0 && guy->GetDirection() < 0) || (vx < 0 && guy->GetDirection() > 0)) {
					float _vx, _vy;

					e->obj->GetSpeed(_vx, _vy);
					e->obj->SetSpeed(_vx, -BROS_JUMP_SPEED);

					x -= min_tx * dx + nx * 0.4f;
				}
				else if (state == KOOPAS_STATE_SPIN) {
					// die
					guy->SetState(BROS_STATE_DIE);
					LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* k = dynamic_cast<CKoopas*>(e->obj);

				float tmpX, tmpY;

				k->GetPosition(tmpX, tmpY);

				if (k->GetState() != KOOPAS_STATE_DIE) {
					if (state == KOOPAS_STATE_SPIN && k->GetState() == KOOPAS_STATE_SPIN) {
						SetState(KOOPAS_STATE_DIE);
						k->SetState(KOOPAS_STATE_DIE);

						BangEffect* bangEffect = new BangEffect();
						bangEffect->SetPosition(tmpX, tmpY);
						Grid::GetInstance()->putObjectIntoGrid(bangEffect);
					}
					else if (state == KOOPAS_STATE_SPIN) {
						x -= min_tx * dx + nx * 0.4f;
						y -= min_ty * dy + ny * 0.4f;

						k->SetState(KOOPAS_STATE_DIE);

						BangEffect* bangEffect = new BangEffect();
						bangEffect->SetPosition(tmpX, tmpY);
						Grid::GetInstance()->putObjectIntoGrid(bangEffect);

						LPGAMEOBJECT point = new Point(MUSHROOM_POINT, tmpX, tmpY);
						Grid::GetInstance()->putObjectIntoGrid(point);
						Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
					}
					else if (k->GetHarmless() && k->GetState() != KOOPAS_STATE_WALKING_LEFT && k->GetState() != KOOPAS_STATE_WALKING_RIGHT) {
						SetState(KOOPAS_STATE_DIE);

						BangEffect* bangEffect = new BangEffect();
						bangEffect->SetPosition(tmpX, tmpY);
						Grid::GetInstance()->putObjectIntoGrid(bangEffect);

						LPGAMEOBJECT point = new Point(MUSHROOM_POINT, tmpX, tmpY);
						Grid::GetInstance()->putObjectIntoGrid(point);
						Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
					}
					else if (state != KOOPAS_STATE_IN_SHELL) {
						vx *= -1;
						nx *= -1;
						state = (vx > 0) ? KOOPAS_STATE_WALKING_RIGHT : KOOPAS_STATE_WALKING_LEFT;
					}
				}
			}
		}

		/*float sT, sB, sL, sR;
		float l, t, r, b;

		beneathSurface->GetBoundingBox(sL, sT, sR, sB);
		GetBoundingBox(l, t, r, b);

		if (l <= sL || r >= sR) {
			vx *= -1;
		}*/
	}

	// turn around if about to fall
	/*if () {
		x += (vx > 0) ? -2.0f : 2.0f;
		y -= 0.8f;
		vx *= -1;
	}*/

	//DebugOut(L"%f\n", vy);

	if (isActive) grid->putObjectIntoGrid(this);

	for (UINT i = 0; i < (UINT)coEvents.size(); i++) delete coEvents[i];
}

void RedKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_IN_SHELL) {
		ani = (upward) ? REDKOOPAS_ANI_IN_SHELL_UPWARD : KOOPAS_ANI_SHELL_IDLE;
	}
	else if (state == KOOPAS_STATE_SPIN) {
		ani = (upward) ? REDKOOPAS_ANI_SPIN_UPWARD : KOOPAS_ANI_SHELL_SPIN;
	}
	else if (state == KOOPAS_STATE_WALKING_RIGHT) {
		ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	else if (state == KOOPAS_STATE_WALKING_LEFT) {
		ani = KOOPAS_ANI_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_DIE) {
		ani = REDKOOPAS_ANI_IN_SHELL_UPWARD;
	}

	if (outShell) {
		ani = (upward) ? REDKOOPAS_ANI_IN_SHELL_UPWARD : REDKOOPAS_ANI_OUT_A_SHELL;
	}

	currAni = ani;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void RedKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = -KOOPAS_DIE_DEFLECT_SPEED;
		spawn_delay = (DWORD)GetTickCount64();
		interactivable = 0;
		break;
		/*case KOOPAS_STATE_WALKING:
			if (nx > 0) {
				vx = KOOPAS_WALKING_SPEED;
			}
			else {
				vx = -KOOPAS_WALKING_SPEED;
			}
			break;*/
	case KOOPAS_STATE_WALKING_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		nx = -1;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		nx = 1;
		break;
	case KOOPAS_STATE_IN_SHELL:
		in_shell = (DWORD)GetTickCount64();
		break;
	case KOOPAS_STATE_SPIN:
		vx = KOOPAS_WALKING_SPEED;
		break;
	}

}