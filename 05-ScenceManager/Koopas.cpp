#include "Koopas.h"
#include "BangEffect.h"
#include "BoomerangGuy.h"
#include "Board.h"
#include "BrokenBrick.h"


CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	interactivable = 1;
	renderPriority = 101;
}

CKoopas::CKoopas(short int nx, int level)
{
	interactivable = 1;
	renderPriority = 101;

	this->nx = (int)nx;
	initNx = (int)nx;

	if (nx > 0) {
		SetState(KOOPAS_STATE_WALKING_RIGHT);
		vx = KOOPAS_WALKING_SPEED;
	}
	else {
		SetState(KOOPAS_STATE_WALKING_LEFT);
		vx = -KOOPAS_WALKING_SPEED;
	}

	this->level = level;
	//vx = (nx > 0) ? KOOPAS_WALKING_SPEED : -KOOPAS_WALKING_SPEED;
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_OUT_A_SHELL)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	else */if (state == KOOPAS_STATE_IN_SHELL && (DWORD)GetTickCount64() - in_shell > 5000) {
		OutShell();
	}
	else if (outShell) {
		if ((DWORD)GetTickCount64() - out_shell_start < 700) {
			vx *= -1;
		}
		else {
			FinishOutShell();
		}
	}

	if (state == KOOPAS_STATE_SPIN && (DWORD)GetTickCount64() - just_got_kicked > KOOPAS_GET_KICKED_HARMLESS_TIME) {
		harmless = 0;
	}

	CGame* game = CGame::GetInstance();

	Grid* grid = Grid::GetInstance();

	grid->clearObjFromGrid(this);

	vy += KOOPAS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

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

		for (UINT i = 0; i < (UINT)coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			float bx, by;

			e->obj->GetPosition(bx, by);

			isStanding = (e->ny < 0) ? e->obj->GetCanBeStandOn() : false;

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QBrick*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj) || dynamic_cast<Ground*>(e->obj)) {

				if (e->nx) {
					float l, t, r, b;

					e->obj->GetBoundingBox(l, t, r, b);

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
				}

				if (ny) {
					if (isStanding && level != KOOPAS_LEVEL_GREEN_WALKING) {
						vy = -KOOPAS_JUMP_SPEED;
					}
					else {
						vy = 0;
					}

					if (state == KOOPAS_STATE_IN_SHELL) {
						vx = 0;
					}
				}
			}
			else if (dynamic_cast<ColorBrickHitBox*>(e->obj)) {
				if (e->ny < 0) {
					if (isStanding && level != KOOPAS_LEVEL_GREEN_WALKING) {
						vy = -KOOPAS_JUMP_SPEED;
					}
					else {
						vy = 0;
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

				float tmpX, tmpY;

				guy->GetPosition(tmpX, tmpY);

				if (state == KOOPAS_STATE_SPIN && (vx > 0 && guy->GetDirection() < 0) || (vx < 0 && guy->GetDirection() > 0)) {
					float _vx, _vy;

					e->obj->GetSpeed(_vx, _vy);
					e->obj->SetSpeed(_vx, -BROS_JUMP_SPEED);

					x -= min_tx * dx + nx * 0.4f;
				}
				else if (state == KOOPAS_STATE_SPIN) {
					// die
					guy->SetState(BROS_STATE_DIE);

					BangEffect* bangEffect = new BangEffect();
					bangEffect->SetPosition(tmpX, tmpY);
					Grid::GetInstance()->putObjectIntoGrid(bangEffect);

					LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* k = dynamic_cast<CKoopas*>(e->obj);

				float tmpX, tmpY;

				k->GetPosition(tmpX, tmpY);

				if (e->ny) {
					y -= min_ty * dy + ny * 0.4f;
				}

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
						if (vx < 0) {
							x += 0.05f;
						}
						else {
							x -= 0.05f;
						}

						vx *= -1;
						nx *= -1;
						state = (vx > 0) ? KOOPAS_STATE_WALKING_RIGHT : KOOPAS_STATE_WALKING_LEFT;
					}
				}
			}
		}
	}

	if (isActive) grid->putObjectIntoGrid(this);

	for (UINT i = 0; i < (UINT)coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;

	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_IN_SHELL || beingHolded) {
		ani = (upward) ? KOOPAS_ANI_SHELL_UPWARD_GREEN : KOOPAS_ANI_SHELL_IDLE;
	}
	else if (state == KOOPAS_STATE_SPIN) {
		ani = (upward) ? KOOPAS_ANI_SPIN_UPWARD_GREEN : KOOPAS_ANI_SHELL_SPIN;
	}
	else if (state == KOOPAS_STATE_WALKING_RIGHT) {
		DebugOut(L"%d\n", isStanding);
		ani = (level == KOOPAS_LEVEL_GREEN_WALKING) ? KOOPAS_ANI_WALKING_RIGHT : ((isStanding) ? KOOPAS_ANI_WALKING_RIGHT_GREEN_FLYING : KOOPAS_ANI_FLYING_RIGHT_GREEN_FLYING);
	}
	else if (state == KOOPAS_STATE_WALKING_LEFT) {
		ani = (level == KOOPAS_LEVEL_GREEN_WALKING) ? KOOPAS_ANI_WALKING_LEFT : ((isStanding) ? KOOPAS_ANI_WALKING_LEFT_GREEN_FLYING : KOOPAS_ANI_FLYING_LEFT_GREEN_FLYING);
	}
	else if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_SHELL_UPWARD_GREEN;
	}

	if (outShell) {
		ani = (upward) ? KOOPAS_ANI_OUT_A_SHELL_UPWARD_GREEN : KOOPAS_ANI_OUT_A_SHELL;
	}

	currAni = ani;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
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
	case KOOPAS_STATE_WALKING:
		if (nx > 0) {
			vx = KOOPAS_WALKING_SPEED;
		}
		else {
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
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

void CKoopas::Spawn() {
	Grid::GetInstance()->clearObjFromGrid(this);
	isActive = 1;
	invisible = 0;
	harmless = 0;
	x = initX;
	y = initY;
	SetState(KOOPAS_STATE_WALKING_LEFT);
	Grid::GetInstance()->putObjectIntoGrid(this);
}

void CKoopas::GetHit(bool byTail, int nx)
{
	if (byTail) {
		InShell();
		upward = 1;
		level = KOOPAS_LEVEL_GREEN_WALKING;
		vy = -KOOPAS_DIE_DEFLECT_SPEED;
		vx = nx * KOOPAS_GET_HIT_VX;
	}
	else {
		SetState(KOOPAS_STATE_DIE);
	}

	BangEffect* bangEffect = new BangEffect();
	bangEffect->SetPosition(x, y);
	Grid::GetInstance()->putObjectIntoGrid(bangEffect);
}

void CKoopas::GetHit(int nx)
{
	SetState(KOOPAS_STATE_DIE);
}

void CKoopas::GetKicked(int nx)
{
	beingHolded = 0;
	harmless = 1;
	state = KOOPAS_STATE_SPIN;
	vx = (nx > 0) ? KOOPAS_SPIN_SPEED : -KOOPAS_SPIN_SPEED;
	this->nx = nx;
	just_got_kicked = (DWORD)GetTickCount64();
}

bool CKoopas::GetBeingHolded()
{
	return beingHolded;
}

void CKoopas::Die()
{
	SetState(KOOPAS_STATE_DIE);
}

void CKoopas::IsHolded()
{
	vx = 0;
	vy = 0;
	beingHolded = 1;
	harmless = 1;
}

bool CKoopas::GetHarmless()
{
	return harmless;
}

void CKoopas::SetHarmless(bool val)
{
	harmless = val;
}

void CKoopas::SetDirection(int nx)
{
	this->nx = nx;
	vx = (nx > 0) ? KOOPAS_WALKING_SPEED : -KOOPAS_WALKING_SPEED;
}

void CKoopas::InShell()
{
	state = KOOPAS_STATE_IN_SHELL;
	y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE);
	//y -= 100;
	backupVx = vx;
	vx = 0;
	vy = 0;
	in_shell = (DWORD)GetTickCount64();
	harmless = 1;
}

void CKoopas::OutShell()
{
	out_shell_start = (DWORD)GetTickCount64();
	//backupVx = vx;
	outShell = 1;
	vx = 0.05f;
	vy = 0;
	state = KOOPAS_STATE_OUT_A_SHELL;
	/*if (initNx > 0) {
		SetState(KOOPAS_STATE_WALKING_RIGHT);
	}
	else {
		SetState(KOOPAS_STATE_WALKING_LEFT);
	}*/
}

void CKoopas::FinishOutShell() {
	outShell = 0;
	harmless = 0;
	upward = 0;
	y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE);
	vx = backupVx;
	vy = 0;
	if (vx > 0) {
		SetState(KOOPAS_STATE_WALKING_RIGHT);
	}
	else {
		SetState(KOOPAS_STATE_WALKING_LEFT);
	}
}

void CKoopas::SetPos(const float& x, const float& y)
{
	this->x = x;
	this->y = y;
}

void CKoopas::GetJumpedOn(int nx)
{
	if (level > KOOPAS_LEVEL_GREEN_WALKING) {
		LooseWings();
	}
	else if (state == KOOPAS_STATE_IN_SHELL) {
		GetKicked(nx);
	}
	else if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT) {
		InShell();
	}

	LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
	Grid::GetInstance()->putObjectIntoGrid(point);
	Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
}

void CKoopas::LooseWings()
{
	level = KOOPAS_LEVEL_GREEN_WALKING;
	vy = 0;
	state = (nx < 0) ? KOOPAS_STATE_WALKING_LEFT : KOOPAS_STATE_WALKING_RIGHT;
}
