#pragma once
#include "GameObject.h"
#include "Tail.h"
#include "Grid.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_WALKING_MAX_SPEED	0.1f

#define MARIO_SKID_ACCELERATION	0.005f
#define MARIO_WALKINGSLOWINGDOWN_ACCELERATION	0.003f
#define MARIO_RUNNINGSLOWINGDOWN_ACCELERATION	0.004f

#define MARIO_RUNNING_SPEED		0.19f

#define MARIO_RACOON_GLIDE_VY		0.035f
#define MARIO_RACOON_FALL_VY		0.05f
#define MARIO_RACOON_FLY_VY			 -0.06f

#define MARIO_JUMP_SLIGHTLY		0.25f
#define MARIO_JUMP_ACCELERATION	0.01f

#define MARIO_WALKING_ACCELERATION	0.002f

//0.1f
#define MARIO_JUMP_SPEED_Y		0.3f
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
#define MARIO_STATE_FALL			900
#define MARIO_STATE_GLIDE			1000
#define MARIO_STATE_FALL_TAIL		1100
#define MARIO_STATE_JUMP_LONG		1200
#define MARIO_STATE_KICK_RIGHT		1300
#define MARIO_STATE_KICK_LEFT		1400
#define MARIO_STATE_LIFT_RIGHT		1500
#define MARIO_STATE_LIFT_LEFT		1600

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

#define MARIO_ANI_RACOON_FLY_RIGHT		33
#define	MARIO_ANI_RACOON_FLY_LEFT		34

#define MARIO_ANI_RACOON_SLIDE_RIGHT	35
#define MARIO_ANI_RACOON_SLIDE_LEFT		36

#define MARIO_ANI_SMALL_SLIDE_RIGHT		37
#define MARIO_ANI_SMALL_SLIDE_LEFT		38

#define MARIO_ANI_BIG_SLIDE_RIGHT		39
#define MARIO_ANI_BIG_SLIDE_LEFT		40

#define MARIO_ANI_RACOON_GLIDE_RIGHT	41
#define MARIO_ANI_RACOON_GLIDE_LEFT		42

#define MARIO_ANI_RACOON_FALL_RIGHT		43
#define MARIO_ANI_RACOON_FALL_LEFT		44

#define MARIO_ANI_RACOON_FALL_TAIL_RIGHT	45
#define MARIO_ANI_RACOON_FALL_TAIL_LEFT		46

#define MARIO_ANI_BIG_TO_RACOON			47

#define MARIO_ANI_BIG_SKID_RIGHT	48
#define MARIO_ANI_BIG_SKID_LEFT		49

#define MARIO_ANI_SMALL_SKID_RIGHT	50
#define MARIO_ANI_SMALL_SKID_LEFT	51

#define MARIO_ANI_RACOON_SKID_RIGHT	52
#define MARIO_ANI_RACOON_SKID_LEFT	53

#define MARIO_ANI_BIG_KICK_RIGHT	54
#define MARIO_ANI_BIG_KICK_LEFT		55

#define MARIO_ANI_SMALL_KICK_RIGHT	56
#define MARIO_ANI_SMALL_KICK_LEFT	57

#define MARIO_ANI_RACOON_KICK_RIGHT	58
#define MARIO_ANI_RACOON_KICK_LEFT	59

#define MARIO_ANI_BIG_LIFT_THINGS_RIGHT		60
#define MARIO_ANI_BIG_LIFT_THINGS_LEFT		61

#define MARIO_ANI_SMALL_LIFT_THINGS_RIGHT	62
#define MARIO_ANI_SMALL_LIFT_THINGS_LEFT	63

#define MARIO_ANI_RACOON_LIFT_THINGS_RIGHT	64
#define MARIO_ANI_RACOON_LIFT_THINGS_LEFT	65

#define MARIO_ANI_BIG_LIFT_THINGS_JUMP_RIGHT	66
#define MARIO_ANI_BIG_LIFT_THINGS_JUMP_LEFT		67

#define MARIO_ANI_SMALL_LIFT_THINGS_JUMP_RIGHT	68
#define MARIO_ANI_SMALL_LIFT_THINGS_JUMP_LEFT	69

#define MARIO_ANI_RACOON_LIFT_THINGS_JUMP_RIGHT	70
#define MARIO_ANI_RACOON_LIFT_THINGS_JUMP_LEFT	71

#define MARIO_ANI_BIG_IDLE_LIFT_RIGHT	72
#define MARIO_ANI_BIG_IDLE_LIFT_LEFT	73

#define MARIO_ANI_SMALL_IDLE_LIFT_RIGHT	74
#define MARIO_ANI_SMALL_IDLE_LIFT_LEFT	75

#define MARIO_ANI_RACOON_IDLE_LIFT_RIGHT	76
#define MARIO_ANI_RACOON_IDLE_LIFT_LEFT		77

#define MARIO_ANI_RACOON_ATTACK_TAIL_RIGHT	78
#define MARIO_ANI_RACOON_ATTACK_TAIL_LEFT	79

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
#define MARIO_ANI_SLIDE_RIGHT	10
#define	MARIO_ANI_SLIDE_LEFT	11
#define	MARIO_ANI_SKID_RIGHT	12
#define MARIO_ANI_SKID_LEFT		13
//#define MARIO_ANI_KICK_RIGHT	14
//#define MARIO_ANI_KICK_LEFT		15
#define MARIO_ANI_LIFT_RIGHT	16
#define MARIO_ANI_LIFT_LEFT		17
#define MARIO_ANI_LIFT_JUMP_RIGHT	18
#define MARIO_ANI_LIFT_JUMP_LEFT	19
#define MARIO_ANI_LIFT_IDLE_RIGHT	20
#define MARIO_ANI_LIFT_IDLE_LEFT	21
#define MARIO_ANI_KICK_RIGHT	22
#define MARIO_ANI_KICK_LEFT		23


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

#define MARIO_UNTOUCHABLE_TIME 3000
#define MARIO_TRANSFORM_SIZE_TIME 800
#define MARIO_TRANSFORM_RACOON_TIME 450
#define MARIO_KICKING_TIME	200
#define MARIO_ATTACKING_TAIL_TIME	200

#define MARIO_SIZE_TRANSFORMING	1
#define MARIO_RACOOON_TRANSFORMING	2

#define MARIO_RACOON_TAIL_LENGTH	7


class CMario : public CGameObject
{
	//vector<unsigned short int> alphaValsForUntouchable = { 255, 128 };

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
	DWORD startFlying;
	DWORD start_kicking;
	DWORD start_attacking_tail;
	DWORD start_falling_tail;
	DWORD start_prepare_bouncing_up;

	int transforming;

	int backupLevel = 1;
	int backupState;

	float accelerationX;

	bool isRunning = 0;
	bool isSliding = 0;
	bool isFlying = 0;
	bool isFalling = 0;
	bool isGliding = 0;
	bool isJumping = 0;
	bool isFallingTail = 0;
	bool canHold = 0;
	bool isKicking = 0;
	bool isAttackingTail = 0;
	bool moveABit = 0;
	bool beingBouncedUp = 0;

	LPGAMEOBJECT beingHoldedObj = NULL;

	vector<vector<short int>> animationsByLevel = {
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
			MARIO_ANI_SMALL_RUN_LEFT,
			MARIO_ANI_SMALL_SLIDE_RIGHT,
			MARIO_ANI_SMALL_SLIDE_LEFT,
			MARIO_ANI_SMALL_SKID_RIGHT,
			MARIO_ANI_SMALL_SKID_LEFT,
			MARIO_ANI_SMALL_KICK_RIGHT,
			MARIO_ANI_SMALL_KICK_LEFT,
			MARIO_ANI_SMALL_LIFT_THINGS_RIGHT,
			MARIO_ANI_SMALL_LIFT_THINGS_LEFT,
			MARIO_ANI_SMALL_LIFT_THINGS_JUMP_RIGHT,
			MARIO_ANI_SMALL_LIFT_THINGS_JUMP_LEFT,
			MARIO_ANI_SMALL_IDLE_LIFT_RIGHT,
			MARIO_ANI_SMALL_IDLE_LIFT_LEFT,
			MARIO_ANI_SMALL_KICK_RIGHT,
			MARIO_ANI_SMALL_KICK_LEFT
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
			MARIO_ANI_BIG_RUN_LEFT,
			MARIO_ANI_BIG_SLIDE_RIGHT,
			MARIO_ANI_BIG_SLIDE_LEFT,
			MARIO_ANI_BIG_SKID_RIGHT,
			MARIO_ANI_BIG_SKID_LEFT,
			MARIO_ANI_BIG_KICK_RIGHT,
			MARIO_ANI_BIG_KICK_LEFT,
			MARIO_ANI_BIG_LIFT_THINGS_RIGHT,
			MARIO_ANI_BIG_LIFT_THINGS_LEFT,
			MARIO_ANI_BIG_LIFT_THINGS_JUMP_RIGHT,
			MARIO_ANI_BIG_LIFT_THINGS_JUMP_LEFT,
			MARIO_ANI_BIG_IDLE_LIFT_RIGHT,
			MARIO_ANI_BIG_IDLE_LIFT_LEFT,
			MARIO_ANI_BIG_KICK_RIGHT,
			MARIO_ANI_BIG_KICK_LEFT
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
			MARIO_ANI_RACOON_RUN_LEFT,
			MARIO_ANI_RACOON_SLIDE_RIGHT,
			MARIO_ANI_RACOON_SLIDE_LEFT,
			MARIO_ANI_RACOON_SKID_RIGHT,
			MARIO_ANI_RACOON_SKID_LEFT,
			MARIO_ANI_RACOON_KICK_RIGHT,
			MARIO_ANI_RACOON_KICK_LEFT,
			MARIO_ANI_RACOON_LIFT_THINGS_RIGHT,
			MARIO_ANI_RACOON_LIFT_THINGS_LEFT,
			MARIO_ANI_RACOON_LIFT_THINGS_JUMP_RIGHT,
			MARIO_ANI_RACOON_LIFT_THINGS_JUMP_LEFT,
			MARIO_ANI_RACOON_IDLE_LIFT_RIGHT,
			MARIO_ANI_RACOON_IDLE_LIFT_LEFT,
			MARIO_ANI_RACOON_KICK_RIGHT,
			MARIO_ANI_RACOON_KICK_LEFT
		}
	};

public:
	bool flyUp = 0;
	bool hasJustKicked = 0;
	bool tailAttacked = 0;

	int transform_duration_time = 0;

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel();
	void StartUntouchable() { untouchable = 1; untouchable_start = (DWORD)GetTickCount64(); }

	void StartFlying() { startFlying = (DWORD)GetTickCount64(); isFlying = 1; isFalling = 0; isStanding = 0; isGliding = 0; vy = MARIO_RACOON_FLY_VY; };
	void StartGliding() {
		isGliding = 1; isFalling = 0; isStanding = 0;
		vy = MARIO_RACOON_GLIDE_VY;
	};
	void StartFalling() {
		isFlying = 0; isGliding = 0; isFalling = 1;
		vy = MARIO_RACOON_FALL_VY;
	};

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

	void SetIsFlying(bool val);
	bool GetIsFlying();

	void SetIsFalling(bool val);
	bool GetIsFalling();

	void SetIsGliding(bool val);
	bool GetIsGliding();

	void SetIsFallingTail(bool val);
	bool GetIsFallingTail();

	void SetTransforming(int val);
	int GetTransforming();

	void SetCanHold(const bool& val);
	bool GetCanHold();

	void SetIsKicking(bool val);
	bool GetIsKicking();

	void SetIsAttackingTail(bool val);
	bool GetIsAttackingTail();

	void SetBeingHoldedObj(LPGAMEOBJECT obj);
	LPGAMEOBJECT GetBeingHoldedObj();

	void SetStartTransforming(DWORD startTransforming);
	DWORD GetStartTransforming();

	float GetOldX();
	float GetOldY();

	int GetNx();

	float GetVx();
	float GetVy();

	void turnIntoSmall();

	void turnIntoBig();

	void BigToRacoon();
	void RacoonToBig();

	void ToBig();
	void ToSmall();

	void ToRacoon();

	void Reset();

	void RenderSizeTransforming();
	void RenderBigToRacoonTransforming();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void finishSizeTransforming();
	void finishRacoonTransforming();

	int filterSomeCommonAniByLevel();

	void StartTransforming() {
		if (transforming == MARIO_SIZE_TRANSFORMING) {
			RenderSizeTransforming();
		}
		else if (transforming == MARIO_RACOOON_TRANSFORMING) {
			RenderBigToRacoonTransforming();
		}
	}

	void FinishTransforming() {
		if (transforming == MARIO_SIZE_TRANSFORMING) {
			finishSizeTransforming();
		}
		else if (transforming == MARIO_RACOOON_TRANSFORMING) {
			finishRacoonTransforming();
		}
	}

	void StartKicking() {
		isKicking = 1;
		start_kicking = (DWORD)GetTickCount64();
	}

	void FinishKicking() {
		isKicking = 0;
	}

	void StartAttackingWithTail() {
		isAttackingTail = 1;
		start_attacking_tail = (DWORD)GetTickCount64();

		/*float maxRange = (nx > 0) ? x + 25.0f : -10.0f;

		beingHoldedObj = new Tail(nx, maxRange);*/

		//Grid::GetInstance()->putObjectIntoGrid(beingHoldedObj);
	}

	void FinishAttackingWithTail() {
		moveABit = 0;
		x += (nx > 0) ? -6 : 6;
		isAttackingTail = 0;

		//Grid::GetInstance()->clearObjFromGrid(beingHoldedObj);
		//delete beingHoldedObj;
		//beingHoldedObj = NULL;

		/*Grid::GetInstance()->ObjIntoTrash(beingHoldedObj);
		beingHoldedObj = NULL;*/
	}

	void StartFallingTail() {
		if (beingBouncedUp && (DWORD)GetTickCount64() - start_prepare_bouncing_up <= 150) {
			state = MARIO_STATE_JUMP;
			vy = -0.35f;
			return;
		}

		SetState(MARIO_STATE_FALL_TAIL);
		start_falling_tail = (DWORD)GetTickCount64();
	}
};