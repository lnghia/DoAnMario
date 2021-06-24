#include "Koopas.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	interactivable = 1;
	renderPriority = 101;
}

CKoopas::CKoopas(short int nx)
{
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

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_OUT_A_SHELL)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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

	if (state == KOOPAS_STATE_DIE) {
		if ((DWORD)GetTickCount64() - spawn_delay < KOOPAS_DIE_TIME) {
			return;
		}
		Spawn();
	}
	else if (state == KOOPAS_STATE_IN_SHELL && (DWORD)GetTickCount64() - in_shell > 600) {
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
		ani = KOOPAS_ANI_SHELL_IDLE;
	}
	else if (state == KOOPAS_STATE_SPIN) {
		ani = KOOPAS_ANI_SHELL_SPIN;
	}
	else if (state == KOOPAS_STATE_WALKING_RIGHT) {
		ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	else if (state == KOOPAS_STATE_WALKING_LEFT) {
		ani = KOOPAS_ANI_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}

	if (outShell) {
		ani = KOOPAS_ANI_OUT_A_SHELL;
	}

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
		vy = KOOPAS_DIE_DEFLECT_SPEED;
		spawn_delay = (DWORD)GetTickCount64();
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

void CKoopas::GetHit(bool byTail)
{
	if (byTail) {
		SetState(KOOPAS_STATE_DIE);
	}
	else {
		SetState(KOOPAS_STATE_IN_SHELL);
	}
}

void CKoopas::GetKicked(const int& nx)
{
	beingHolded = 0;
	harmless = 0;
	state = KOOPAS_STATE_SPIN;
	vx = (nx > 0) ? KOOPAS_SPIN_SPEED : -KOOPAS_SPIN_SPEED;
	this->nx = nx;
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
