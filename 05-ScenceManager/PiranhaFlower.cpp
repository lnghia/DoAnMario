#include "PiranhaFlower.h"

PiranhaFlower::PiranhaFlower(float pipeX, float pipeY, float pipeWidth, float pipeHeight, LPGAMEOBJECT player, int level)
{
	this->player = player;

	this->pipeX = pipeX;
	this->pipeY = pipeY;
	this->pipeWidth = pipeWidth;
	this->pipeHeight = pipeHeight;

	SetState(PIRANHAPLAN_STATE_AWAKE);
	climax = pipeY - ((level == PIRANHAPLANT_LEVEL_BIG) ? PIRANHAPLANT_BBOX_HEIGHT : PIRANHAPLANT_BBOX_HEIGHT_SMALL);

	y = pipeY;

	this->level = level;

	vx = 0;
	vy = -0.065f;
	isSleeping = 1;
	startResting = (DWORD)GetTickCount64();
	startAttacking = 0;

	currAni = 0;
}

void PiranhaFlower::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void PiranhaFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float camX, camY;
	int screenWidth, screenHeight;
	CGame* gameInstance = CGame::GetInstance();

	camX = gameInstance->GetCamX();
	camY = gameInstance->GetCamY();
	screenWidth = gameInstance->GetScreenWidth();
	screenHeight = gameInstance->GetScreenHeight();

	/*if (y <= climax) {
		vy = 0.25f;
	}
	else if (y >= pipeY) {
		vy = -0.25f;
	}*/

	/*if (y >= pipeY) {
		vy = -0.25f;
	}*/

	if (x + PIRANHAPLANT_BBOX_WIDTH < camX || x > camX + screenWidth ||
		y + PIRANHAPLANT_BBOX_HEIGHT < camY || y > camY + screenHeight) {
		return;
	}

	float playerLeft, playerTop, playerRight, playerBottom;

	player->GetBoundingBox(playerLeft, playerTop, playerRight, playerBottom);

	if ((abs(playerRight - pipeX) <= 1 || abs(playerLeft - pipeX - pipeWidth) <= 1) && y >= pipeY) {
		return;
	}
	if (level == PIRANHAPLANT_LEVEL_SMALL && y >= pipeY && playerLeft >= pipeX && playerRight <= pipeY + pipeWidth) {
		return;
	}

	CGameObject::Update(dt);

	x += dx;
	y += dy;

	if (this->y <= climax && !startAttacking) {
		y = climax;
		vy = 0;
		startAttacking = (DWORD)GetTickCount64();
		startResting = 0;
		return;
	}
	else if (this->y > pipeY && !startResting) {
		vy = 0;
		startResting = (DWORD)GetTickCount64();
		startAttacking = 0;
		isSleeping = 1;
	}

	if (startAttacking && (DWORD)GetTickCount64() - startAttacking >= PIRANHAPLANT_AWAKE_TIME) {
		vy = PIRANHAPLANT_SLEEP_SPEED;
		startAttacking = 0;
	}
	else if (startResting && (DWORD)GetTickCount64() - startResting >= PIRANHAPLANT_REST_TIME) {
		vy = PIRANHAPLANT_AWAKE_SPEED;
		startResting = 0;
		fired = 0;
		isSleeping = 0;
	}

	//CGameObject::Update(dt);
	//DebugOut(L"[Piranha Plant] %f %f", x, y);
}

void PiranhaFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PIRANHAPLANT_BBOX_WIDTH;
	b = y + ((level == PIRANHAPLANT_LEVEL_BIG) ? PIRANHAPLANT_BBOX_HEIGHT : PIRANHAPLANT_BBOX_HEIGHT_SMALL);
}
