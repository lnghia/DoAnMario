#pragma once

#include "GameObject.h"

#include "Grid.h"
#include "Map.h"
#include "PiranhaPlant.h"
#include "FireBall.h"
#include "Brick.h"
#include "QBrick.h"
#include "Goomba.h"
#include "Ground.h"


#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING 100
//#define KOOPAS_STATE_DIE 200

#define KOOPAS_ANI_WALKING_LEFT 3
#define KOOPAS_ANI_WALKING_RIGHT 2
#define KOOPAS_ANI_SHELL_IDLE	0
#define KOOPAS_ANI_DIE 4
#define KOOPAS_ANI_SHELL_SPIN	1
#define KOOPAS_ANI_OUT_A_SHELL	5

#define KOOPAS_DIE_TIME	1000

#define KOOPAS_GRAVITY	0.002f

#define KOOPAS_STATE_DIE	0
#define KOOPAS_STATE_WALKING_RIGHT	1
#define	KOOPAS_STATE_WALKING_LEFT	2
//#define KOOPAS_STATE_WALKING	5
#define	KOOPAS_STATE_IN_SHELL	3
#define KOOPAS_STATE_OUT_A_SHELL	6
#define	KOOPAS_STATE_SPIN	4

#define KOOPAS_DIE_DEFLECT_SPEED	0.3f
#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_SPIN_SPEED	0.22f

class CKoopas : public CGameObject
{
protected:
	DWORD spawn_delay = NULL;
	DWORD in_shell = NULL;
	DWORD out_shell_start = NULL;

	float initX;
	float initY;
	short int initNx;

	bool harmless = 0;
	bool outShell = 0;
	bool beingHolded = 0;

	float backupVx = 0;
public:
	CKoopas(); 
	CKoopas(short int nx);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void Spawn();
	void GetHit(bool byTail = 0);
	void GetKicked(const int& nx);
	bool GetBeingHolded();
	void Die();
	void IsHolded();

	bool GetHarmless();
	void SetHarmless(bool val);
	void SetDirection(int nx);

	void InShell();
	void OutShell();
	void FinishOutShell();

	void SetPos(const float& x, const float& y);
};
