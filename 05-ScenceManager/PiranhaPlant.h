#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Animations.h"
#include "FireBall.h"
#include "Grid.h"

#define PIRANHAPLANT_BBOX_WIDTH  16
#define PIRANHAPLANT_BBOX_HEIGHT 32
#define PIRANHAPLANT_BBOX_HEIGHT_SMALL	24

#define PIRANHAPLANT_ANI_NUM	8

#define PIRANHAPLANT_LEVEL_BIG		0
#define PIRANHAPLANT_LEVEL_SMALL	1

#define PIRANHAPLANT_ANI_BOTLEFT_STILL		0
#define PIRANHAPLANT_ANI_BOTLEFT			1
#define PIRANHAPLANT_ANI_TOPLEFT_STILL		2
#define PIRANHAPLANT_ANI_TOPLEFT			3
#define PIRANHAPLANT_ANI_BOTRIGHT_STILL		4
#define PIRANHAPLANT_ANI_BOTRIGHT			5
#define PIRANHAPLANT_ANI_TOPRIGHT_STILL		6
#define PIRANHAPLANT_ANI_TOPRIGHT			7
#define PIRANHAPLANT_ANI_SLEEP				8

#define PIRANHAPLANT_AWAKE_TIME 2000
#define PIRANHAPLANT_REST_TIME 850
#define LOCKING_TARGET_TIME 1000

#define PIRANHAPLANT_STATE_NORMAL			0
#define PIRANHAPLANT_STATE_SLEEP			1
#define PIRANHAPLAN_STATE_AWAKE             2

#define PIRANHAPLANT_AWAKE_SPEED -0.065f
#define PIRANHAPLANT_SLEEP_SPEED 0.065f

#define FLAME_ANI_SET_ID 610


class PiranhaPlant : public CGameObject
{
protected:
	LPGAMEOBJECT player;

	float pipeX, pipeY;
	float pipeWidth, pipeHeight;

	int climax;

	bool isSleeping = 1;
	bool fired = 0;

	DWORD startAttacking;
	DWORD startResting;

	unsigned int dirX, dirY;

	int level = 0;

public:
	PiranhaPlant() {};
	PiranhaPlant(float pipeX, float pipeY, float pipeWidth, float pipeHeight, LPGAMEOBJECT player, int level = 0);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	void Awake() { vy = PIRANHAPLANT_AWAKE_SPEED; }
	void Sleep() { vy = PIRANHAPLANT_SLEEP_SPEED; }
	void throwFlame();
};

