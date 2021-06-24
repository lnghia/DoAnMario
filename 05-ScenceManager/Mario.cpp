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
#include "Ground.h"
#include "Wood.h"
#include "FloatingCoin.h"
#include "RedKoopas.h"
#include "Koopas.h"

#include "Map.h"
#include "Board.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	transforming = 0;
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

	if (transforming && GetTickCount64() - startTransforming < transform_duration_time) {
		return;
	}
	if (isFlying && GetTickCount64() - startFlying > 200 && state != MARIO_STATE_DIE) {
		if (GetTickCount64() - startFlying > 2000) {
			SetState(MARIO_STATE_FALL);
		}
		else {
			SetState(MARIO_STATE_GLIDE);
		}
	}

	oldX = x;
	oldY = y;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	float _dx = dx, _dy = dy;

	Grid::GetInstance()->clearObjFromGrid(this);

	// Simple fall down
	if (!isFlying && !isFalling && !isGliding) vy += MARIO_GRAVITY * dt;

	//DebugOut(L"[DEBUG] %f - %f\n", _dy, vy);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<UINT> exceptions;

	float l1, r1, t1, b1;
	//float l2, r2, t2, b2;

	GetBoundingBox(l1, t1, r1, b1);

	coEvents.clear();

	for (UINT i = 0; i < (UINT)coObjects->size(); ++i) {
		LPGAMEOBJECT tmp = coObjects->at(i);

		if (beingHoldedObj && tmp == beingHoldedObj) {
			coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), tmp), coObjects->end());
		}
	}

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		float temp = vy;

		//
		// Collision logic with other objects
		//
		//float px, py;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			isStanding = (e->ny < 0) ? e->obj->GetCanBeStandOn() : false;
			if (isStanding) {
				isJumping = 0;
				isFlying = 0;
				isFalling = 0;
				isGliding = 0;
				isFallingTail = 0;
			}
			//isJumping = isFlying = isFalling = !isStanding;

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
					LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
				}
				else if (e->nx != 0)
				{
					x -= min_tx * dx + nx * 0.4f;

					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level == MARIO_LEVEL_BIG)
							{
								//level = MARIO_LEVEL_SMALL;
								backupLevel = MARIO_LEVEL_SMALL;
								backupState = state;
								startTransforming = (DWORD)GetTickCount64();
								turnIntoSmall();
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_RACOON) {
								SetStartTransforming((DWORD)GetTickCount64());
								RacoonToBig();
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<RedKoopas*>(e->obj)) {
				RedKoopas* koopas = dynamic_cast<RedKoopas*>(e->obj);
				int state = koopas->GetState();

				if (e->ny < 0)
				{
					if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
					{
						//koopas->SetState(KOOPAS_STATE_IN_SHELL);
						koopas->InShell();
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if(koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
						// spin
						koopas->GetKicked((int)nx);
					}
					/*LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);*/
				}
				else if (e->nx != 0)
				{
					x -= min_tx * dx + nx * 0.4f;

					if (/*isRunning && isStanding*/ canHold && koopas->GetHarmless()) {
						beingHoldedObj = koopas;
						koopas->IsHolded();
						//Grid::GetInstance()->clearObjFromGrid(koopas);

						/*if (level == MARIO_LEVEL_SMALL) {
							float tmpX = x + (vx > 0) ? 10 : 5;
							float tmpY = y - 1;

							koopas->SetPosition(tmpX, tmpY);
						}
						else if(level != MARIO_LEVEL_RACOON) {
							float tmpX = x + (vx > 0) ? 10 : 5;
							float tmpY = y + 8;

							koopas->SetPosition(tmpX, tmpY);
						}
						else {
							float tmpX = x + (vx > 0) ? 10 : 5;
							float tmpY = y + 10;

							koopas->SetPosition(tmpX, tmpY);
						}

						Grid::GetInstance()->putObjectIntoGrid(koopas);*/
					}
					else if (untouchable == 0)
					{
						if (hasJustKicked) {
							float _l, _t, _r, _b;
							float tmpX, tmpY;

							hasJustKicked = 0;
							GetBoundingBox(_l, _t, _r, _b);
							koopas->GetPosition(tmpX, tmpY);
							if (nx > 0) {
								koopas->SetPosition(_l - 1 - 16, tmpY);
							}
							else {
								koopas->SetPosition(_r + 3, tmpY);
							}
						}
						else if (!koopas->GetHarmless())
						{
							if (level == MARIO_LEVEL_BIG)
							{
								//level = MARIO_LEVEL_SMALL;
								backupLevel = MARIO_LEVEL_SMALL;
								backupState = state;
								startTransforming = (DWORD)GetTickCount64();
								turnIntoSmall();
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_RACOON) {
								SetStartTransforming((DWORD)GetTickCount64());
								RacoonToBig();
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
						else {
							//kick
							int tmp = (vx > 0) ? 1 : -1;
							koopas->GetKicked(tmp);
							StartKicking();
						}
					}
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<FireBall*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				if (!untouchable) {
					SetSpeed(0, 0);

					if (level == MARIO_LEVEL_BIG)
					{
						//mario->SetLevel(MARIO_LEVEL_SMALL);
						SetBackupLevel(MARIO_LEVEL_SMALL);
						SetBackupState(GetState());
						SetStartTransforming((DWORD)GetTickCount64());
						turnIntoSmall();
						StartUntouchable();
					}
					else if (GetLevel() == MARIO_LEVEL_RACOON) {
						SetStartTransforming((DWORD)GetTickCount64());
						RacoonToBig();
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (dynamic_cast<PiranhaPlant*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				if (!untouchable)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						//level = MARIO_LEVEL_SMALL;
						backupLevel = MARIO_LEVEL_SMALL;
						backupState = state;
						startTransforming = (DWORD)GetTickCount64();
						turnIntoSmall();
						StartUntouchable();
					}
					else if (level == MARIO_LEVEL_RACOON) {
						startTransforming = (DWORD)GetTickCount64();
						RacoonToBig();
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
				else {
					x += _dx;
					y += _dy;
				}
			}
			else if (dynamic_cast<Ground*>(e->obj) || dynamic_cast<PipeHitBox*>(e->obj) || dynamic_cast<Wood*>(e->obj)) {
				_dx = _dy = 0;

				if (e->nx != 0) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny) {
					vy = 0;
				}
			}
			else if (dynamic_cast<QBrick*>(e->obj)) {
				if (e->nx != 0) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny != 0) vy = 0;

				if (e->ny > 0) {
					QBrick* qBrick = dynamic_cast<QBrick*>(e->obj);

					qBrick->PopUpHiddenItem();
				}
			}
			else if (dynamic_cast<ColorBrickHitBox*>(e->obj)) {

				if (e->ny < 0) {
					vy = 0;
				}
				else {
					x -= min_tx * dx + nx * 0.4f;
					y -= min_ty * dy + ny * 0.4f;

					x += _dx;
					y += _dy;
				}
			}
			else if (dynamic_cast<Mushroom*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				Mushroom* mushroom = dynamic_cast<Mushroom*>(e->obj);
				float mX, mY;

				mushroom->GetPosition(mX, mY);
				mushroom->GotObsorbed(this);

				LPGAMEOBJECT point = new Point(MUSHROOM_POINT, mX, mY);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(LEAF_POINT);

				if (GetLevel() != MARIO_LEVEL_BIG) {
					SetBackupLevel(MARIO_LEVEL_BIG);
					SetBackupState(GetState());
					SetStartTransforming((DWORD)GetTickCount64());
					turnIntoBig();
				}
			}
			else if (dynamic_cast<FloatingCoin*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				dynamic_cast<FloatingCoin*>(e->obj)->GetObsorbed();
				Board::GetInstance()->GetPoint()->Add(COIN_POINT);
				Board::GetInstance()->GetMoney()->Add(COIN_MONEY);
			}
			else if (dynamic_cast<Leaf*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				Leaf* leaf = dynamic_cast<Leaf*>(e->obj);

				leaf->GotObsorbed(this);
				LPGAMEOBJECT point = new Point(LEAF_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(LEAF_POINT);

				if (level != MARIO_LEVEL_RACOON && level != MARIO_LEVEL_SMALL) {
					SetStartTransforming((DWORD)GetTickCount64());
					BigToRacoon();
				}
			}
		}
	}

	if (beingHoldedObj) {
		Grid::GetInstance()->clearObjFromGrid(beingHoldedObj);

		int tmp = (nx > 0) ? 10 : -16 + 5;

		if (level == MARIO_LEVEL_SMALL) {
			//float tmpX = x + (nx > 0) ? 10 : -16 + 5;
			//float tmpX = x - 16 + 5;
			float tmpX = x + tmp;
			float tmpY = y - 1;

			beingHoldedObj->SetPosition(tmpX, tmpY);
		}
		else if (level != MARIO_LEVEL_RACOON) {
			//float tmpX = x + (nx > 0) ? 10.0f : 5.0f;
			float tmpX = x + tmp;
			float tmpY = y + 8;

			beingHoldedObj->SetPosition(tmpX, round(tmpY));
		}
		else {
			//float tmpX = x + (nx > 0) ? 10.0f : 5.0f;
			float tmpX = x + tmp;
			float tmpY = y + 10;

			beingHoldedObj->SetPosition(tmpX, tmpY);
		}
		beingHoldedObj->SetVx(vx);

		Grid::GetInstance()->putObjectIntoGrid(beingHoldedObj);
	}

	Grid::GetInstance()->putObjectIntoGrid(this);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	//DebugOut(L"[is sliding] %d - %d - %d - %d\n", isSliding, isFlying, isFalling, isJumping);

	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else {
		if (isFalling) {
			ani = (nx > 0) ? MARIO_ANI_RACOON_FALL_RIGHT : MARIO_ANI_RACOON_FALL_LEFT;
		}
		else if (isFlying) {
			if (isGliding) {
				ani = (nx > 0) ? MARIO_ANI_RACOON_GLIDE_RIGHT : MARIO_ANI_RACOON_GLIDE_LEFT;
			}
			else {
				ani = (nx > 0) ? MARIO_ANI_RACOON_FLY_RIGHT : MARIO_ANI_RACOON_FLY_LEFT;
			}
		}
		else if (isFallingTail) {
			ani = (nx > 0) ? MARIO_ANI_RACOON_FALL_TAIL_RIGHT : MARIO_ANI_RACOON_FALL_TAIL_LEFT;
		}
		else {
			ani = filterSomeCommonAniByLevel();
		}
	}


	int alpha = 255;

	if (untouchable) {
		alpha = 128;
	}

	//DebugOut(L"[] %d\n", ani);

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT: {
		if (!isRunning) {
			//vx = MARIO_WALKING_SPEED;
			if (vx > MARIO_WALKING_SPEED) {
				vx -= MARIO_RUNNINGSLOWINGDOWN_ACCELERATION;
			}
			else if (vx >= 0 && vx <= MARIO_WALKING_SPEED) {
				vx = MARIO_WALKING_SPEED;
			}
			else if (vx < 0) {
				vx += MARIO_SKID_ACCELERATION;
			}
		}
		else
		{
			if (vx < MARIO_WALKING_SPEED) {
				if (vx < 0 && isStanding) {
					vx += MARIO_SKID_ACCELERATION;
				}
				else {
					vx = MARIO_WALKING_SPEED;
				}
			}
			if (vx < MARIO_RUNNING_SPEED && isStanding)
				vx += MARIO_WALKING_ACCELERATION;
		}
		nx = 1;
		break;
	}
	case MARIO_STATE_WALKING_LEFT: {
		if (!isRunning) {
			if (vx < -MARIO_WALKING_SPEED) {
				vx += MARIO_RUNNINGSLOWINGDOWN_ACCELERATION;
			}
			else if (vx <= 0 && vx >= -MARIO_WALKING_SPEED) {
				vx = -MARIO_WALKING_SPEED;
			}
			else if (vx > 0) {
				vx -= MARIO_SKID_ACCELERATION;
			}
		}
		//vx = -MARIO_WALKING_SPEED;
		//vx = (vx > 0) ? vx - MARIO_SKID_ACCELERATION : -MARIO_WALKING_SPEED;
		else
		{
			if (vx > -MARIO_WALKING_SPEED) {
				if (vx > 0 && isStanding) {
					vx -= MARIO_SKID_ACCELERATION;
				}
				else {
					vx = -MARIO_WALKING_SPEED;
				}
			}
			if (vx > -MARIO_RUNNING_SPEED && isStanding)
				vx -= MARIO_WALKING_ACCELERATION;
		}
		nx = -1;
		break;
	}
	case MARIO_STATE_JUMP: {
		if (!isStanding) {
			return;
		}

		/*if (abs(vy) < MARIO_JUMP_SLIGHTLY) {
			vy = -MARIO_JUMP_SLIGHTLY;
		}
		else if (abs(vy) < MARIO_JUMP_SPEED_Y) {
			vy -= MARIO_JUMP_ACCELERATION;
		}
		else {
			vy = -MARIO_JUMP_SPEED_Y;
		}*/
		vy = -MARIO_JUMP_SPEED_Y;
		isStanding = false;
		isJumping = true;
		break;
	}
	case MARIO_STATE_IDLE: {

		if (nx > 0) {
			if (vx > MARIO_WALKING_SPEED) {
				vx -= MARIO_RUNNINGSLOWINGDOWN_ACCELERATION;
			}
			else {
				vx -= MARIO_WALKING_SPEED;
			}
		}
		else if (nx < 0) {
			if (vx < -MARIO_WALKING_SPEED) {
				vx += MARIO_RUNNINGSLOWINGDOWN_ACCELERATION;
			}
			else {
				vx += MARIO_WALKING_SPEED;
			}
		}

		vx = ((nx > 0 && vx < 0) || (nx < 0 && vx>0)) ? 0 : vx;

		break;
	}
	case MARIO_STATE_DIE: {
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		break;
	}
	case MARIO_STATE_FLY:
		StartFlying();
		break;
	case MARIO_STATE_FALL:
		StartFalling();
		break;
	case MARIO_STATE_GLIDE:
		StartGliding();
		break;
	case MARIO_STATE_FALL_TAIL:
		vy = MARIO_RACOON_GLIDE_VY;
		isFallingTail = 1;
		break;
	}


	CGameObject::SetState(state);
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
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_RACOON) {
		if (nx == 1) {
			left += MARIO_RACOON_TAIL_LENGTH;
		}

		right = left + MARIO_RACOON_BBOX_WIDTH;
		bottom = y + MARIO_RACOON_BBOX_HEIGHT;
	}
}

void CMario::ToBig()
{
	y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
}

void CMario::ToSmall() {
	y += (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
}

void CMario::ToRacoon()
{
	y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
}

void CMario::finishSizeTransforming()
{
	if (transforming && (DWORD)GetTickCount64() - startTransforming >= MARIO_TRANSFORM_SIZE_TIME) {
		//player->SetState(player->Get)
		SetState(backupState);

		if (backupLevel == MARIO_LEVEL_BIG && level == MARIO_LEVEL_SMALL) {
			ToBig();
		}
		else if (backupLevel == MARIO_LEVEL_SMALL && level == MARIO_LEVEL_BIG) {
			ToSmall();
		}

		level = backupLevel;

		transforming = 0;
		//player->SetStartTransforming((DWORD)0);
	}
}

void CMario::finishRacoonTransforming()
{
	if (transforming && (DWORD)GetTickCount64() - startTransforming >= MARIO_TRANSFORM_RACOON_TIME) {
		transforming = 0;
	}
}

int CMario::filterSomeCommonAniByLevel()
{
	int ani;

	//DebugOut(L"[IS JUMPING] %d\n", isJumping);

	if (isKicking && (DWORD)GetTickCount64() - start_kicking < MARIO_KICKING_TIME) {
		ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_KICK_RIGHT] : animationsByLevel[level][MARIO_ANI_KICK_LEFT];
	}
	else if (isJumping) {
		if (beingHoldedObj) {
			ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_LIFT_JUMP_RIGHT] : animationsByLevel[level][MARIO_ANI_LIFT_JUMP_LEFT];
		}
		else {
			ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_JUMP_RIGHT] : animationsByLevel[level][MARIO_ANI_JUMP_LEFT];
		}
	}
	else if (!vx && state == MARIO_STATE_IDLE) {
		if (beingHoldedObj) {
			ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_LIFT_IDLE_RIGHT] : animationsByLevel[level][MARIO_ANI_LIFT_IDLE_LEFT];
		}
		else {
			ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_IDLE_RIGHT] : animationsByLevel[level][MARIO_ANI_IDLE_LEFT];
		}
	}
	else if (!vx && state != MARIO_STATE_IDLE) {
		/*if (nx > 0) {
			if (vx < 0) {
				ani = animationsByLevel[level][MARIO_ANI_SKID_RIGHT];
			}
			else {
				ani = animationsByLevel[level][MARIO_ANI_WALKING_RIGHT];
			}
		}
		else {
			if (vx > 0) {
				ani = animationsByLevel[level][MARIO_ANI_SKID_LEFT];
			}
			else {
				ani = animationsByLevel[level][MARIO_ANI_WALKING_LEFT];
			}
		}*/
		if (beingHoldedObj) {
			ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_LIFT_RIGHT] : animationsByLevel[level][MARIO_ANI_LIFT_LEFT];
		}
		else {
			ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_WALKING_RIGHT] : animationsByLevel[level][MARIO_ANI_WALKING_LEFT];
		}
	}
	else if (vx > 0) {
		if (isJumping && !isFlying) {
			if (beingHoldedObj) {
				ani = animationsByLevel[level][MARIO_ANI_LIFT_JUMP_RIGHT];
			}
			else {
				ani = animationsByLevel[level][MARIO_ANI_JUMP_RIGHT];
			}
		}
		else if (isRunning) {
			if (isSliding) {
				if (beingHoldedObj) {
					ani = (nx < 0) ? animationsByLevel[level][MARIO_ANI_LIFT_LEFT] : animationsByLevel[level][MARIO_ANI_LIFT_RIGHT];
				}
				else {
					ani = (nx < 0) ? animationsByLevel[level][MARIO_ANI_SKID_LEFT] : animationsByLevel[level][MARIO_ANI_SLIDE_RIGHT];
				}
			}
			else {
				if (beingHoldedObj) {
					ani = (nx < 0) ? animationsByLevel[level][MARIO_ANI_LIFT_LEFT] : animationsByLevel[level][MARIO_ANI_LIFT_RIGHT];
				}
				else {
					ani = (nx < 0) ? animationsByLevel[level][MARIO_ANI_SKID_LEFT] : animationsByLevel[level][MARIO_ANI_RUN_RIGHT];
				}
			}
		}
		else {
			if (beingHoldedObj) {
				ani = (nx < 0) ? animationsByLevel[level][MARIO_ANI_LIFT_LEFT] : animationsByLevel[level][MARIO_ANI_LIFT_RIGHT];
			}
			else {
				ani = (nx < 0) ? animationsByLevel[level][MARIO_ANI_SKID_LEFT] : animationsByLevel[level][MARIO_ANI_WALKING_RIGHT];
			}
		}
	}
	else {
		if (isJumping && !isFlying) {
			if (beingHoldedObj) {
				ani = animationsByLevel[level][MARIO_ANI_LIFT_JUMP_LEFT];
			}
			else {
				ani = animationsByLevel[level][MARIO_ANI_JUMP_LEFT];
			}
		}
		else if (isRunning) {
			if (isSliding) {
				if (beingHoldedObj) {
					ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_LIFT_RIGHT] : animationsByLevel[level][MARIO_ANI_LIFT_LEFT];
				}
				else {
					ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_SKID_RIGHT] : animationsByLevel[level][MARIO_ANI_SLIDE_LEFT];
				}
			}
			else {
				if (beingHoldedObj) {
					ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_LIFT_RIGHT] : animationsByLevel[level][MARIO_ANI_LIFT_LEFT];
				}
				else {
					ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_SKID_RIGHT] : animationsByLevel[level][MARIO_ANI_RUN_LEFT];
				}
			}
		}
		else {
			if (beingHoldedObj) {
				ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_LIFT_RIGHT] : animationsByLevel[level][MARIO_ANI_LIFT_LEFT];
			}
			else {
				ani = (nx > 0) ? animationsByLevel[level][MARIO_ANI_SKID_RIGHT] : animationsByLevel[level][MARIO_ANI_WALKING_LEFT];
			}
		}
	}

	return ani;
}

DWORD CMario::GetUntouchableStart()
{
	return untouchable_start;
}

bool CMario::GetUntouchable()
{
	return untouchable;
}

int CMario::GetBackupLevel()
{
	return backupLevel;
}

void CMario::SetBackupLevel(int val) {
	backupLevel = val;
}

int CMario::GetBackupState()
{
	return backupState;
}

void CMario::SetBackupState(int state) {
	backupState = state;
}

bool CMario::GetIsRunning()
{
	return isRunning;
}

void CMario::SetIsRunning(bool val) {
	isRunning = val;
}

bool CMario::GetIsSliding() {
	return isSliding;
}

void CMario::SetIsSliding(bool val)
{
	isSliding = val;
}

void CMario::SetIsStanding(bool val)
{
	isStanding = val;
}

bool CMario::GetIsStanding() {
	return isStanding;
}

void CMario::SetIsJumping(bool val)
{
	isJumping = val;
}

bool CMario::GetIsJumping() {
	return isJumping;
}

void CMario::SetIsFlying(bool val)
{
	isFlying = val;
}

bool CMario::GetIsFlying() {
	return isFlying;
}

void CMario::SetIsFalling(bool val)
{
	isFalling = val;
}

bool CMario::GetIsFalling() {
	return isFalling;
}

void CMario::SetIsGliding(bool val)
{
	isGliding = val;
}

bool CMario::GetIsGliding() {
	return isGliding;
}

void CMario::SetIsFallingTail(bool val)
{
	isFallingTail = val;
}

bool CMario::GetIsFallingTail() {
	return isFallingTail;
}

void CMario::SetTransforming(int val)
{
	transforming = val;
}

int CMario::GetTransforming() {
	return transforming;
}

void CMario::SetCanHold(const bool& val)
{
	canHold = val;
}

bool CMario::GetCanHold()
{
	return canHold;
}

void CMario::SetIsKicking(bool val)
{
	isKicking = val;
}

bool CMario::GetIsKicking()
{
	return isKicking;
}

void CMario::SetBeingHoldedObj(LPGAMEOBJECT obj)
{
	beingHoldedObj = obj;
}

LPGAMEOBJECT CMario::GetBeingHoldedObj()
{
	return beingHoldedObj;
}

void CMario::SetStartTransforming(DWORD startTransforming)
{
	this->startTransforming = startTransforming;
}

DWORD CMario::GetStartTransforming() {
	return startTransforming;
}

float CMario::GetOldX()
{
	return oldX;
}

float CMario::GetOldY() {
	return oldY;
}

int CMario::GetNx()
{
	return nx;
}

float CMario::GetVx()
{
	return vx;
}

float CMario::GetVy() {
	return vy;
}

void CMario::turnIntoSmall()
{
	transforming = 1;
	transform_duration_time = MARIO_TRANSFORM_SIZE_TIME;
	level = MARIO_LEVEL_SMALL;
	y = (y + (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
	SetState(MARIO_STATE_BIG_TO_SMALL);
}

void CMario::turnIntoBig() {
	/*if (level == MARIO_LEVEL_SMALL) {

	}*//*
	if (level == MARIO_LEVEL_BIG) {
		return;
	}*/
	transforming = 1;
	transform_duration_time = MARIO_TRANSFORM_SIZE_TIME;
	level = MARIO_LEVEL_BIG;
	y = (y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
	SetState(MARIO_STATE_BIG_TO_SMALL);
}

void CMario::BigToRacoon() {
	transforming = MARIO_RACOOON_TRANSFORMING;
	transform_duration_time = MARIO_TRANSFORM_RACOON_TIME;
	level = MARIO_LEVEL_RACOON;
	y -= abs(MARIO_RACOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
	if (nx > 0) {
		x -= MARIO_RACOON_TAIL_LENGTH;
	}
}

void CMario::RacoonToBig() {
	transforming = MARIO_RACOOON_TRANSFORMING;
	transform_duration_time = MARIO_TRANSFORM_RACOON_TIME;
	level = MARIO_LEVEL_BIG;
	y += abs(MARIO_RACOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
	if (nx > 0) {
		x += MARIO_RACOON_TAIL_LENGTH;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	Board::GetInstance()->GetTime()->Reset();
}

void CMario::RenderSizeTransforming()
{
	int ani;

	//ani = (nx > 0) ? MARIO_ANI_BIG_TO_SMALL_RIGHT : MARIO_ANI_BIG_TO_SMALL_LEFT;

	if (level == MARIO_LEVEL_BIG) {
		turnIntoSmall();
		//level = MARIO_LEVEL_SMALL;
		ani = (nx > 0) ? MARIO_ANI_SMALL_IDLE_RIGHT : MARIO_ANI_SMALL_IDLE_LEFT;
	}
	else {
		turnIntoBig();
		//level = MARIO_LEVEL_BIG;
		ani = (nx > 0) ? MARIO_ANI_BIG_IDLE_RIGHT : MARIO_ANI_BIG_IDLE_LEFT;
	}

	animation_set->at(ani)->Render(x, y);
}

void CMario::RenderBigToRacoonTransforming() {
	animation_set->at(MARIO_ANI_BIG_TO_RACOON)->Render(x, y);
}

