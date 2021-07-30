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
#include "BrokenQuestionBrick.h"
#include "BrokenBrick.h"
#include "NoteBrick.h"
#include "EndGameBrick.h"
#include "Boomerang.h"
#include "BoomerangGuy.h"
#include "PiranhaFlower.h"
#include "WorldMapScene.h"
#include "PortalPipe.h"
#include "GreenMushroom.h"
#include "PSwitch.h"
#include "MusicBrickToHeaven.h"

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
	if (transforming && (int)((DWORD)GetTickCount64() - startTransforming) < transform_duration_time) {
		return;
	}
	if (isFlying && (int)((DWORD)GetTickCount64() - startFlying) > 200 && state != MARIO_STATE_DIE) {
		if (GetTickCount64() - startFlying > 2000) {
			SetState(MARIO_STATE_FALL);
		}
		else {
			SetState(MARIO_STATE_GLIDE);
		}
	}
	else if (isFallingTail && (DWORD)GetTickCount64() - start_falling_tail > 200 && state != MARIO_STATE_DIE) {
		isFallingTail = 0;
	}

	if (toExtraScene) {
		CGameObject::Update(dt);
		x += dx;
		y += dy;

		float ml, mt, mr, mb;

		GetBoundingBox(ml, mt, mr, mb);

		if ((vy < 0 && mb <= pipeY) || (vy > 0 && mt >= pipeY + pipeHeight)) {
			SetSpeed(0, 0);
			Board::GetInstance()->GetTime()->StopTicking();
			//Board::GetInstance()->GetTime()->SetTime(0);
			//Map::getInstance()->unLoad();
			int timeLeft = Board::GetInstance()->GetTime()->GetCurrMoment();
			DWORD start_time = Board::GetInstance()->GetTime()->GetStart();
			CGame::GetInstance()->SwitchScene(extraSceneId);
			Board::GetInstance()->GetCardStack()->RefreshItemAni();
			Board::GetInstance()->GetTime()->SetStart(start_time);
			Board::GetInstance()->GetTime()->SetTime(timeLeft);
			WorldMapScene* scene = (WorldMapScene*)CGame::GetInstance()->GetCurrentScene();
			scene->SetMarioLevel(level);

			float appearX = exitX + (exitWidth - (mr - ml)) / 2;

			if (exitDirect < 0) {
				scene->GetPlayer()->SetPosition(appearX, exitY);
				scene->GetPlayer()->gettingOutPipe = 1;
			}
			else {
				scene->GetPlayer()->SetPosition(appearX, exitY - (mb - mt));
				scene->GetPlayer()->gettingOutPipe = 1;
			}
			scene->GetPlayer()->exitDirect = this->exitDirect;
			scene->GetPlayer()->exitX = this->exitX;
			scene->GetPlayer()->exitY = this->exitY;
			scene->GetPlayer()->exitWidth = this->exitWidth;
			scene->GetPlayer()->exitHeight = this->exitHeight;
		}

		return;
	}
	else if (toHeavenScene) {
		CGameObject::Update(dt);
		x += dx;
		y += dy;

		float ml, mt, mr, mb;

		GetBoundingBox(ml, mt, mr, mb);

		if ((int)((DWORD)GetTickCount64() - start_to_heaven) >= MARIO_TO_HEAVEN_TIME) {
			SetSpeed(0, 0);
			Board::GetInstance()->GetTime()->StopTicking();
			//Board::GetInstance()->GetTime()->SetTime(0);
			//Map::getInstance()->unLoad();
			int timeLeft = Board::GetInstance()->GetTime()->GetCurrMoment();
			DWORD start_time = Board::GetInstance()->GetTime()->GetStart();
			CGame::GetInstance()->SwitchScene(heavenSceneId);
			Board::GetInstance()->GetCardStack()->RefreshItemAni();
			Board::GetInstance()->GetTime()->SetStart(start_time);
			Board::GetInstance()->GetTime()->SetTime(timeLeft);
			WorldMapScene* scene = (WorldMapScene*)CGame::GetInstance()->GetCurrentScene();
			scene->SetMarioLevel(level);
		}

		return;
	}
	else if (gettingOutPipe) {
		CGameObject::Update(dt);
		x += dx;
		y += dy;

		float ml, mt, mr, mb;

		GetBoundingBox(ml, mt, mr, mb);

		if ((vy < 0 && mb < exitY) || (vy > 0 && mt > exitY + exitHeight)) {
			gettingOutPipe = 0;
		}

		return;
	}

	oldX = x;
	oldY = y;
	//touchPortalPipe = 0;
	toExtraScene = 0;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	float _dx = dx, _dy = dy;

	Grid::GetInstance()->clearObjFromGrid(this);

	// Simple fall down
	if (!isFlying && !isFalling && !isGliding && !isFallingTail) vy += MARIO_GRAVITY * dt;

	//DebugOut(L"[DEBUG] %f - %f\n", _dy, vy);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<UINT> exceptions;

	float l1, r1, t1, b1;
	//float l2, r2, t2, b2;

	GetBoundingBox(l1, t1, r1, b1);

	coEvents.clear();

	//shared_ptr<CGameObject> tmp;

	for (UINT i = 0; i < (UINT)coObjects->size(); ++i) {
		//tmp.reset(coObjects->at(i));

		if (beingHoldedObj && beingHoldedObj == coObjects->at(i)) {
			coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), coObjects->at(i)), coObjects->end());
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
		bool standingOnPortalPipe = 0;
		bool standingOnMusicToHeaven = 0;

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
				beingBouncedUp = 0;
				longJump = 0;
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
						//goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						goomba->GetJumpedOn();
					}
					/*	LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
						Grid::GetInstance()->putObjectIntoGrid(point);
						Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);*/
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
								if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
							}
							else if (level == MARIO_LEVEL_RACOON) {
								SetStartTransforming((DWORD)GetTickCount64());
								RacoonToBig();
								StartUntouchable();
								if (isDucking) y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
							}
							else if (level == MARIO_LEVEL_FIRE) {
								SetStartTransforming((DWORD)GetTickCount64());
								FireToBig();
								StartUntouchable();
								if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
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

				float kt, kl, kr, kb;

				koopas->GetBoundingBox(kl, kt, kr, kb);

				if (e->ny < 0)
				{
					if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
					{
						//koopas->SetState(KOOPAS_STATE_IN_SHELL);
						koopas->InShell();
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
						// spin
						int tmp = (x < kl + (kr - kl) / 2) ? 1 : -1;

						koopas->GetKicked(tmp);
					}
					/*LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);*/
				}
				else if (e->nx != 0)
				{
					//x -= min_tx * dx + nx * 0.4f;

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
								if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
							}
							else if (level == MARIO_LEVEL_RACOON) {
								SetStartTransforming((DWORD)GetTickCount64());
								RacoonToBig();
								StartUntouchable();
								if (isDucking) y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
							}
							else if (level == MARIO_LEVEL_FIRE) {
								SetStartTransforming((DWORD)GetTickCount64());
								FireToBig();
								StartUntouchable();
								if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
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
			else if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				int state = koopas->GetState();

				float kt, kl, kr, kb;

				koopas->GetBoundingBox(kl, kt, kr, kb);

				if (e->ny < 0)
				{
					if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
					{
						//koopas->SetState(KOOPAS_STATE_IN_SHELL);
						//koopas->InShell();
						koopas->GetJumpedOn((nx > 0) ? -1 : 1);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
						// spin
						int tmp = (x < kl + (kr - kl) / 2) ? 1 : -1;

						koopas->GetJumpedOn(tmp);
						//koopas->GetKicked((int)tmp);
					}
					/*LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);*/
				}
				else if (e->nx != 0)
				{
					//x -= min_tx * dx + nx * 0.4f;

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
								if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
							}
							else if (level == MARIO_LEVEL_RACOON) {
								SetStartTransforming((DWORD)GetTickCount64());
								RacoonToBig();
								StartUntouchable();
								if (isDucking) y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
							}
							else if (level == MARIO_LEVEL_FIRE) {
								SetStartTransforming((DWORD)GetTickCount64());
								FireToBig();
								StartUntouchable();
								if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
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
			else if (dynamic_cast<PSwitch*>(e->obj)) {
				PSwitch* psw = dynamic_cast<PSwitch*>(e->obj);

				if (e->nx) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny < 0) {
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					psw->trigger();
				}
			}
			else if (dynamic_cast<PortalPipe*>(e->obj)) {
				if (e->nx != 0) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny != 0) {
					PortalPipe* tempObj = dynamic_cast<PortalPipe*>(e->obj);

					vy = 0;
					standingOnPortalPipe = 1;
					touchPortalPipe = 1;
					tempObj->GetPosition(pipeX, pipeY);
					pipeWidth = tempObj->GetWidth();
					pipeHeight = tempObj->GetHeight();
					extraSceneId = tempObj->sceneId;
					exitX = tempObj->exitX;
					exitY = tempObj->exitY;
					exitWidth = tempObj->exitWidth;
					exitHeight = tempObj->exitHeight;
					exitDirect = tempObj->getOutPipeDirection;
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				Map::getInstance()->unLoad();
				//Grid::GetInstance()->unload();
				SetSpeed(0, 0);
				Board::GetInstance()->GetTime()->StopTicking();
				Board::GetInstance()->GetTime()->SetTime(0);
				switchScene = 1;
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				Board::GetInstance()->GetCardStack()->RefreshItemAni();
				WorldMapScene* scene = (WorldMapScene*)CGame::GetInstance()->GetCurrentScene();
				scene->SetMarioLevel(level);
				scene->GetPlayer()->madeItToNextScene = 0;
			}
			else if (dynamic_cast<FireBall*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				if (!untouchable) {
					SetSpeed(0, 0);

					if (level == MARIO_LEVEL_BIG)
					{
						//level = MARIO_LEVEL_SMALL;
						backupLevel = MARIO_LEVEL_SMALL;
						backupState = state;
						startTransforming = (DWORD)GetTickCount64();
						turnIntoSmall();
						StartUntouchable();
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_RACOON) {
						SetStartTransforming((DWORD)GetTickCount64());
						RacoonToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_FIRE) {
						SetStartTransforming((DWORD)GetTickCount64());
						FireToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (dynamic_cast<PiranhaPlant*>(e->obj) || dynamic_cast<PiranhaFlower*>(e->obj)) {
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
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_RACOON) {
						SetStartTransforming((DWORD)GetTickCount64());
						RacoonToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_FIRE) {
						SetStartTransforming((DWORD)GetTickCount64());
						FireToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
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
			else if (dynamic_cast<BrokenQuestionBrick*>(e->obj)) {
				if (e->nx != 0) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny != 0) vy = 0;

				if (e->ny > 0) {
					BrokenQuestionBrick* qBrick = dynamic_cast<BrokenQuestionBrick*>(e->obj);

					qBrick->PopUpHiddenItem();
				}
			}
			else if (dynamic_cast<BrokenBrick*>(e->obj)) {
				if (e->nx) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny) {
					vy = 0;
				}
			}
			else if (dynamic_cast<MusicBrickToHeaven*>(e->obj)) {
				if (e->nx != 0) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny != 0) vy = 0;

				if (e->ny > 0) {
					MusicBrickToHeaven* qBrick = dynamic_cast<MusicBrickToHeaven*>(e->obj);

					qBrick->HopUpABit();
				}
				else if (e->ny < 0) {
					MusicBrickToHeaven* qBrick = dynamic_cast<MusicBrickToHeaven*>(e->obj);

					qBrick->MoveDownABit();
					vy = -MARIO_JUMP_SPEED_Y;
					state = MARIO_STATE_JUMP;
					isJumping = 1;
					isStanding = 0;
					beingBouncedUp = 1;
					start_prepare_bouncing_up = (DWORD)GetTickCount64();
					vx = (vx > 0) ? 0.05f : -0.05f;
					standingOnMusicToHeaven = 1;
					touchMusicToHeavenBrick = 1;
					heavenSceneId = qBrick->heavenSceneId;
				}
			}
			else if (dynamic_cast<NoteBrick*>(e->obj)) {
				if (e->nx != 0) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny != 0) vy = 0;

				if (e->ny > 0) {
					NoteBrick* qBrick = dynamic_cast<NoteBrick*>(e->obj);

					qBrick->HopUpABit();
				}
				else if (e->ny < 0) {
					NoteBrick* qBrick = dynamic_cast<NoteBrick*>(e->obj);

					qBrick->MoveDownABit();
					vy = -MARIO_JUMP_SPEED_Y;
					state = MARIO_STATE_JUMP;
					isJumping = 1;
					isStanding = 0;
					beingBouncedUp = 1;
					start_prepare_bouncing_up = (DWORD)GetTickCount64();
					vx = (vx > 0) ? 0.05f : -0.05f;
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
				Board::GetInstance()->GetPoint()->Add(MUSHROOM_POINT);

				if (GetLevel() != MARIO_LEVEL_BIG) {
					SetBackupLevel(MARIO_LEVEL_BIG);
					SetBackupState(GetState());
					SetStartTransforming((DWORD)GetTickCount64());
					turnIntoBig();
				}
			}
			else if (dynamic_cast<GreenMushroom*>(e->obj)) {
				x -= min_tx * dx + nx * 0.4f;
				y -= min_ty * dy + ny * 0.4f;

				GreenMushroom* mushroom = dynamic_cast<GreenMushroom*>(e->obj);
				float mX, mY;

				mushroom->GetPosition(mX, mY);
				mushroom->GotObsorbed(this);

				LPGAMEOBJECT point = new Point(GREEN_MUSHROOM_POINT, mX, mY);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(MUSHROOM_POINT);

				/*if (GetLevel() != MARIO_LEVEL_BIG) {
					SetBackupLevel(MARIO_LEVEL_BIG);
					SetBackupState(GetState());
					SetStartTransforming((DWORD)GetTickCount64());
					turnIntoBig();
				}*/
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
			else if (dynamic_cast<EndGameBrick*>(e->obj)) {
				if (e->nx != 0) {
					vx = 0;
					isRunning = 0;
					isSliding = 0;
				}
				else if (e->ny != 0) vy = 0;

				EndGameBrick* _obj = dynamic_cast<EndGameBrick*>(e->obj);

				int timeLeft = Board::GetInstance()->GetTime()->GetCurrMoment();
				int chosenCard = _obj->PopUpChoosenItem();
				Board::GetInstance()->GetCardStack()->push(chosenCard);
				Board::GetInstance()->GetPoint()->Add(timeLeft * 50);
				Board::GetInstance()->SetLatestCardType(chosenCard);

				madeItToNextScene = 1;
			}
			else if (dynamic_cast<Boomerang*>(e->obj)) {
				if (e->ny) {
					y -= min_ty * dy + ny * 0.4f;
				}

				if (untouchable == 0)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						//level = MARIO_LEVEL_SMALL;
						backupLevel = MARIO_LEVEL_SMALL;
						backupState = state;
						startTransforming = (DWORD)GetTickCount64();
						turnIntoSmall();
						StartUntouchable();
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_RACOON) {
						SetStartTransforming((DWORD)GetTickCount64());
						RacoonToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_FIRE) {
						SetStartTransforming((DWORD)GetTickCount64());
						FireToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (dynamic_cast<BoomerangGuy*>(e->obj)) {
				BoomerangGuy* guy = dynamic_cast<BoomerangGuy*>(e->obj);

				if (e->ny < 0) {
					if (guy->GetState() != BROS_STATE_DIE)
					{
						guy->SetState(BROS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					LPGAMEOBJECT point = new Point(GOOMBA_POINT, x, y);
					Grid::GetInstance()->putObjectIntoGrid(point);
					Board::GetInstance()->GetPoint()->Add(GOOMBA_POINT);
				}
				else if (untouchable == 0)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						//level = MARIO_LEVEL_SMALL;
						backupLevel = MARIO_LEVEL_SMALL;
						backupState = state;
						startTransforming = (DWORD)GetTickCount64();
						turnIntoSmall();
						StartUntouchable();
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_RACOON) {
						SetStartTransforming((DWORD)GetTickCount64());
						RacoonToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
					}
					else if (level == MARIO_LEVEL_FIRE) {
						SetStartTransforming((DWORD)GetTickCount64());
						FireToBig();
						StartUntouchable();
						if (isDucking) y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}

			touchPortalPipe = standingOnPortalPipe;
			touchMusicToHeavenBrick = standingOnMusicToHeaven;
		}
	}

	if (beingHoldedObj) {
		Grid::GetInstance()->clearObjFromGrid(beingHoldedObj);

		if (dynamic_cast<Tail*>(beingHoldedObj)) {
			beingHoldedObj->Update(dt, coObjects);
		}
		else {
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

				//if (tmp == 10) tmpX += MARIO_RACOON_TAIL_LENGTH;

				beingHoldedObj->SetPosition(tmpX, round(tmpY));
			}
			else {
				//float tmpX = x + (nx > 0) ? 10.0f : 5.0f;
				float tmpX = x + tmp;
				float tmpY = y + 10;

				beingHoldedObj->SetPosition(tmpX, tmpY);
			}
			beingHoldedObj->SetVx(vx);
		}

		Grid::GetInstance()->putObjectIntoGrid(beingHoldedObj);
	}

	Grid::GetInstance()->putObjectIntoGrid(this);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//DebugOut(L"%d\n", (y >= Map::getInstance()->getHeight() - MAP_BELOW_SPARE_SPACE && !madeItToNextScene));
	if (y >= Map::getInstance()->getHeight() - MAP_BELOW_SPARE_SPACE && !madeItToNextScene) {
		//Reset();
		
		//Grid::GetInstance()->unload();
		SetSpeed(0, 0);
		Map::getInstance()->unLoad();
		Board::GetInstance()->GetTime()->StopTicking();
		Board::GetInstance()->GetTime()->SetTime(0);
		Board::GetInstance()->GetLives()->Sub(1);
		CGame::GetInstance()->SwitchScene(4);
		Board::GetInstance()->GetCardStack()->RefreshItemAni();
	}
}

pair<int, int> CMario::UpdateInWorldMap(DWORD dt)
{
	CGameObject::Update(dt);

	x += dx;
	y += dy;
	xInWorldMap += dx;
	yInWorldMap += dy;

	float mt, mb, ml, mr;

	GetBoundingBox(ml, mt, mr, mb);

	if (vy < 0) {
		return { (int)(ml / 16), (int)(mb / 16) };
	}
	if (vy > 0) {
		return { (int)(ml / 16), (int)(mt / 16) };
	}
	if (vx < 0) {
		return { (int)(mr / 16), (int)(yInWorldMap / 16) };
	}
	if (vx > 0) {
		return { (int)(ml / 16), (int)(yInWorldMap / 16) };
	}

	return { (int)((xInWorldMap + (mr - ml) / 2) / 16), (int)((yInWorldMap + (mb - mt) / 2) / 16) };
}

void CMario::Render()
{
	//DebugOut(L"[is sliding] %d - %d - %d - %d\n", isSliding, isFlying, isFalling, isJumping);

	//DebugOut(L"[nx render] %d\n", nx);

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
		else if (level == MARIO_LEVEL_RACOON && isAttackingTail) {
			if ((DWORD)GetTickCount64() - start_attacking_tail < 180) {
				if ((DWORD)GetTickCount64() - start_attacking_tail > 50 && !moveABit) {
					x += (nx > 0) ? 6 : -6;
					moveABit = 1;
				}
				ani = (nx > 0) ? MARIO_ANI_RACOON_ATTACK_TAIL_RIGHT : MARIO_ANI_RACOON_ATTACK_TAIL_LEFT;
			}
			else {
				FinishAttackingWithTail();
				ani = filterSomeCommonAniByLevel();
			}
		}
		else if (level == MARIO_LEVEL_FIRE && isAttackingFire) {
			if ((DWORD)GetTickCount64() - start_attacking_fire < 200) {
				ani = (nx > 0) ? MARIO_ANI_FIRE_THROW_RIGHT : MARIO_ANI_FIRE_THROW_LEFT;
			}
			else {
				FinishAttackingFire();
				ani = filterSomeCommonAniByLevel();
			}
		}
		else {
			ani = filterSomeCommonAniByLevel();
		}
	}

	if (isInWorldMap) {
		if (level == MARIO_LEVEL_SMALL) {
			ani = MARIO_ANI_SMALL_WOLRDMAP;
		}
		else if (level == MARIO_LEVEL_BIG) {
			ani = MARIO_ANI_BIG_WOLRDMAP;
		}
		else if (level == MARIO_LEVEL_RACOON) {
			ani = MARIO_ANI_RACOON_WORLDMAP;
		}
		else if (level == MARIO_LEVEL_FIRE) {
			ani = MARIO_ANI_FIRE_WORLDMAP;
		}
	}
	else if (toExtraScene || gettingOutPipe) {
		if (level == MARIO_LEVEL_SMALL) {
			ani = MARIO_ANI_GET_IN_PIPE_SMALL;
		}
		else if (level == MARIO_LEVEL_BIG) {
			ani = MARIO_ANI_GET_IN_PIPE_BIG;
		}
		else if (level == MARIO_LEVEL_RACOON) {
			ani = MARIO_ANI_GET_IN_PIPE_RACOON;
		}
		else if (level == MARIO_LEVEL_FIRE) {
			ani = MARIO_ANI_GET_IN_PIPE_FIRE;
		}
	}

	currAni = ani;

	int alpha = 255;

	if (untouchable) {
		alpha = 128;
	}

	//DebugOut(L"[] %d\n", ani);

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT: {
		if (isDucking) {
			isDucking = 0;
			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE) {
				y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
			}
			else {
				y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
			}
		}

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
		if (isDucking) {
			isDucking = 0;
			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE) {
				y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
			}
			else {
				y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
			}
		}

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
		if (!isStanding && !beingBouncedUp) {
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
		if (beingBouncedUp && (int)((DWORD)GetTickCount64() - start_prepare_bouncing_up) <= 150) {
			vy = -0.35f;
			if (touchMusicToHeavenBrick) {
				toHeavenScene = 1;
				vx = 0;
				start_to_heaven = (DWORD)GetTickCount64();
			}
		}
		else if (!beingBouncedUp) {
			vy = -MARIO_JUMP_SPEED_Y;
		}

		//vy = (beingBouncedUp && (DWORD)GetTickCount64() - start_prepare_bouncing_up <= 180) ? -0.4f : -MARIO_JUMP_SPEED_Y;
		isStanding = false;
		isJumping = true;
		break;
	}
	case MARIO_STATE_IDLE: {

		if (isDucking) {
			isDucking = 0;
			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE) {
				y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
			}
			else {
				y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
			}
		}

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
		/*if (beingBouncedUp && (int)((DWORD)GetTickCount64() - start_prepare_bouncing_up) <= 150) {
			vy = -0.35f;
			if (touchMusicToHeavenBrick) {
				toHeavenScene = 1;
				vx = 0;
				start_to_heaven = (DWORD)GetTickCount64();
			}
		}
		else if (!beingBouncedUp) {
			StartFlying();
		}*/
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
	case MARIO_STATE_DUCK_LEFT:
		vx = 0;
		if (!isDucking) {
			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE) {
				y += (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
			}
			else {
				y += (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
			}
			isDucking = 1;
		}
		break;
	case MARIO_STATE_DUCK_RIGHT:
		vx = 0;
		if (!isDucking) {
			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE) {
				y += (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_DUCK_BBOX_HEIGHT);
			}
			else {
				y += (MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_DUCK_BBOX_HEIGHT);
			}
			isDucking = 1;
		}
		break;
	}


	CGameObject::SetState(state);
}

void CMario::SetLevel(int l)
{
	level = l;

	if (isInWorldMap) {
		if (l == MARIO_LEVEL_BIG) {
			y -= (MARIO_BIG_BBOX_HEIGHT_WORLDMAP - MARIO_SMALL_BBOX_HEIGHT - 3);
		}
		else if (l == MARIO_LEVEL_RACOON) {
			y -= (MARIO_RACCON_BBOX_HEIGHT_WORLDMAP - MARIO_SMALL_BBOX_HEIGHT + 3);
		}
	}
}

int CMario::GetLevel()
{
	return level;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
	{
		if (isDucking) {
			right = x + MARIO_BIG_DUCK_BBOX_WIDTH;
			bottom = y + MARIO_BIG_DUCK_BBOX_HEIGHT;
		}
		else {
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_RACOON) {
		if (nx == 1) {
			if (isDucking) left += MARIO_RACOON_DUCK_TAIL_LENGTH;
			else left += MARIO_RACOON_TAIL_LENGTH;
		}
		if (isDucking) {
			right = left + MARIO_RACOON_DUCK_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_DUCK_BBOX_HEIGHT;
		}
		else {
			right = left + MARIO_RACOON_BBOX_WIDTH;
			bottom = y + MARIO_RACOON_BBOX_HEIGHT;
		}
	}

	if (isInWorldMap) {
		/*if (level == MARIO_LEVEL_BIG)
		{
			right = xInWorldMap + MARIO_BIG_BBOX_WIDTH_WORLDMAP;
			bottom = yInWorldMap + MARIO_BIG_BBOX_HEIGHT_WORLDMAP;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			right = xInWorldMap + MARIO_SMALL_BBOX_WIDTH_WORLDMAP;
			bottom = yInWorldMap + MARIO_SMALL_BBOX_HEIGHT_WORLMAP;
		}
		else if (level == MARIO_LEVEL_RACOON) {
			right = xInWorldMap + MARIO_RACCON_BBOX_WIDTH_WORLDMAP;
			bottom = yInWorldMap + MARIO_RACCON_BBOX_HEIGHT_WORLDMAP;
		}*/
		left = xInWorldMap;
		top = yInWorldMap;
		right = xInWorldMap + MARIO_SMALL_BBOX_WIDTH_WORLDMAP;
		bottom = yInWorldMap + MARIO_SMALL_BBOX_HEIGHT_WORLMAP;
	}
}

void CMario::ToBig()
{
	y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
}

void CMario::ToSmall() {
	y += (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
}

void CMario::ToFire()
{
	y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
}

void CMario::ToRacoon()
{
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE) {
		y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
	}
	else y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
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
	else if (isDucking) {
		ani = (state == MARIO_STATE_DUCK_LEFT) ? animationsByLevel[level][MARIO_ANI_DUCK_LEFT] : animationsByLevel[level][MARIO_ANI_DUCK_RIGHT];
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

bool CMario::GetMadeItToNextScene()
{
	return madeItToNextScene;
}

void CMario::SetMadeItToNextScene(bool val) {
	madeItToNextScene = val;
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

void CMario::SetIsAttackingTail(bool val)
{
	isAttackingTail = val;
}

bool CMario::GetIsAttackingTail()
{
	return isAttackingTail && currAni == MARIO_ANI_RACOON_ATTACK_TAIL_LEFT || currAni == MARIO_ANI_RACOON_ATTACK_TAIL_RIGHT;
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

bool CMario::GetIsInWorldMap()
{
	return isInWorldMap;
}

void CMario::SetIsInWorldMap(bool val) {
	isInWorldMap = val;
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

void CMario::BigToFire()
{
	transforming = MARIO_FIRE_TRANSFORMING;
	transform_duration_time = MARIO_TRANSFORM_SIZE_TIME;
	level = MARIO_LEVEL_FIRE;
	//y -= abs(MARIO_RACOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
	/*if (nx > 0) {
		x -= MARIO_RACOON_TAIL_LENGTH;
	}*/
}

void CMario::FireToBig()
{
	transforming = MARIO_FIRE_TRANSFORMING;
	transform_duration_time = MARIO_TRANSFORM_SIZE_TIME;
	level = MARIO_LEVEL_BIG;
}

void CMario::SetSwitchScene(bool val)
{
	switchScene = val;
}

bool CMario::GetSwitchScene()
{
	return switchScene;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	//SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	Board::GetInstance()->GetTime()->SetTime(0);
	Map::getInstance()->unLoad();
	//Grid::GetInstance()->unload();
	CGame::GetInstance()->SwitchScene(4);
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

void CMario::RenderBigToFireTransforming()
{
	int ani = (nx > 0) ? MARIO_ANI_FIRE_TURN_RIGHT : MARIO_ANI_FIRE_TURN_LEFT;

	animation_set->at(ani)->Render(x, y);
}

void CMario::FinishBigToFireTransforming()
{
	if (transforming && (DWORD)GetTickCount64() - startTransforming >= MARIO_TRANSFORM_SIZE_TIME) {
		transforming = 0;
	}
}

