#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_WALKING_MAX_SPEED	0.1f

#define MARIO_RUNNING_SPEED		0.19f

#define MARIO_RACOON_FALL_VY		0.035f
#define MARIO_RACOON_FLY_VY			 -0.06f

#define MARIO_WALKING_ACCELERATION	0.01f

//0.1f
#define MARIO_JUMP_SPEED_Y		0.33f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0008f
#define MARIO_DIE_DEFLECT_SPEED	 0.3f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_SMALL_TO_BIG	500
#define MARIO_STATE_BIG_TO_SMALL	600
#define MARIO_STATE_FLY				700
#define MARIO_STATE_SLIDE			800

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_BIG_TO_SMALL_RIGHT	9
#define MARIO_ANI_BIG_TO_SMALL_LEFT		10
#define MARIO_ANI_SMALL_TO_BIG_RIGHT	11
#define MARIO_ANI_SMALL_TO_BIG_LEFT		12

#define MARIO_ANI_RACOON_IDLE_RIGHT		13
#define MARIO_ANI_RACOON_IDLE_LEFT		14

#define MARIO_ANI_RACOON_WALK_RIGHT		15
#define	MARIO_ANI_RACOON_WALK_LEFT		16

#define	MARIO_ANI_RACOON_JUMP_LEFT		17
#define	MARIO_ANI_RACOON_JUMP_RIGHT		18

#define MARIO_ANI_RACOON_DUCK_RIGHT		19
#define MARIO_ANI_RACOON_DUCK_LEFT		20

#define MARIO_ANI_BIG_RUN_RIGHT			21
#define MARIO_ANI_BIG_RUN_LEFT			22

#define MARIO_ANI_RACOON_RUN_RIGHT			23
#define MARIO_ANI_RACOON_RUN_LEFT			24

#define	MARIO_ANI_SMALL_JUMP_RIGHT		25
#define	MARIO_ANI_SMALL_JUMP_LEFT		26

#define	MARIO_ANI_SMALL_RUN_RIGHT		27
#define	MARIO_ANI_SMALL_RUN_LEFT		28

#define	MARIO_ANI_BIG_JUMP_RIGHT		29
#define	MARIO_ANI_BIG_JUMP_LEFT			30

#define MARIO_ANI_BIG_DUCK_RIGHT		31
#define	MARIO_ANI_BIG_DUCK_LEFT			32


// Indexes are defined for filtering some common animations by level
#define MARIO_ANI_IDLE_RIGHT	0
#define MARIO_ANI_IDLE_LEFT		1
#define MARIO_ANI_WALKING_RIGHT	2
#define MARIO_ANI_WALKING_LEFT	3
#define MARIO_ANI_JUMP_RIGHT	4
#define	MARIO_ANI_JUMP_LEFT		5
#define	MARIO_ANI_DUCK_RIGHT	6
#define	MARIO_ANI_DUCK_LEFT		7
#define	MARIO_ANI_RUN_RIGHT		8
#define	MARIO_ANI_RUN_LEFT		9


#define MARIO_ANI_DIE				8

#define	MARIO_LEVEL_SMALL	0
#define	MARIO_LEVEL_BIG		1
#define MARIO_LEVEL_RACOON	2

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_RACOON_BBOX_HEIGHT 28
#define MARIO_RACOON_BBOX_WIDTH	 15

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	vector<unsigned short int> alphaValsForUntouchable = { 255, 128 };

	int level;
	bool untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	float oldX;
	float oldY;

	bool isStanding;

	int aniBigToSmallIndex = -1;

	DWORD startTransforming = (DWORD)0;

	bool transforming;

	int backupLevel = 1;
	int backupState;

	float accelerationX;

	bool isRunning = 0;
	bool isSliding = 0;
	bool isFlying = 0;
	bool isJumping = 0;

	vector<vector<int>> animationsByLevel = {
		{
			MARIO_ANI_SMALL_IDLE_RIGHT,
			MARIO_ANI_SMALL_IDLE_LEFT,
			MARIO_ANI_SMALL_WALKING_RIGHT,
			MARIO_ANI_SMALL_WALKING_LEFT,
			MARIO_ANI_SMALL_JUMP_RIGHT,
			MARIO_ANI_SMALL_JUMP_LEFT,
			MARIO_ANI_SMALL_IDLE_RIGHT,
			MARIO_ANI_SMALL_IDLE_LEFT,
			MARIO_ANI_SMALL_RUN_RIGHT,
			MARIO_ANI_SMALL_RUN_LEFT
		},
		{
			MARIO_ANI_BIG_IDLE_RIGHT,
			MARIO_ANI_BIG_IDLE_LEFT,
			MARIO_ANI_BIG_WALKING_RIGHT,
			MARIO_ANI_BIG_WALKING_LEFT,
			MARIO_ANI_BIG_JUMP_RIGHT,
			MARIO_ANI_BIG_JUMP_LEFT,
			MARIO_ANI_BIG_DUCK_RIGHT,
			MARIO_ANI_BIG_DUCK_LEFT,
			MARIO_ANI_BIG_RUN_RIGHT,
			MARIO_ANI_BIG_RUN_LEFT
		},
		{
			MARIO_ANI_RACOON_IDLE_RIGHT,
			MARIO_ANI_RACOON_IDLE_LEFT,
			MARIO_ANI_RACOON_WALK_RIGHT,
			MARIO_ANI_RACOON_WALK_LEFT,
			MARIO_ANI_RACOON_JUMP_RIGHT,
			MARIO_ANI_RACOON_JUMP_LEFT,
			MARIO_ANI_RACOON_DUCK_RIGHT,
			MARIO_ANI_RACOON_DUCK_LEFT,
			MARIO_ANI_RACOON_RUN_RIGHT,
			MARIO_ANI_RACOON_RUN_LEFT
		}
	};

public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	DWORD GetUntouchableStart();

	bool GetUntouchable();

	int GetBackupLevel();
	void SetBackupLevel(int val);

	int GetBackupState();
	void SetBackupState(int state);

	bool GetIsRunning();
	void SetIsRunning(bool val);

	bool GetIsSliding();
	void SetIsSliding(bool val);

	void SetIsStanding(bool val);
	bool GetIsStanding();

	void SetIsJumping(bool val);
	bool GetIsJumping();

	void SetTransforming(bool val);
	bool GetTransforming();

	void SetStartTransforming(DWORD startTransforming);
	DWORD GetStartTransforming();

	float GetOldX();
	float GetOldY();

	float GetVx();
	float GetVy();

	void turnIntoSmall();

	void turnIntoBig();

	void Reset();

	void RenderSizeTransforming();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void ToBig();
	void ToSmall();

	void ToRacoon();

	void finishSizeTransforming();

	int filterSomeCommonAniByLevel();
};