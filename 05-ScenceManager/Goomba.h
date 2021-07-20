#pragma once
#include "GameObject.h"
#include "Board.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_DIE_DEFLECT_SPEED					0.3f
#define GOOMBA_DIE_GET_HIT_BY_SHELL_DEFLECT_SPEED	0.1f
#define GOOMBA_JUMP_SPEED							0.22f
#define GOOMBA_GET_HIT_BY_TAIL_VX					0.022f

#define GOOMBA_BBOX_WIDTH					16
#define GOOMBA_BBOX_HEIGHT					15
#define GOOMBA_BBOX_HEIGHT_DIE				9
#define GOOMBA_FLYING_BBOX_WIDTH			20
#define GOOMBA_FLYING_BBOX_HEIGHT			24
#define GOOMBA_FLYING_STANDING_BBOX_HEIGHT	18
#define GOOMBA_WING_TO_TOE_DIS				2
#define GOOMBA_WING_TO_HEAD_DIS_JUMPING		8
#define GOOMBA_WING_TO_HEAD_DIS_STANDING	3

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_GET_HIT	300

#define GOOMBA_ANI_WALKING		0
#define GOOMBA_ANI_DIE			1
#define GOOMBA_ANI_GET_HIT		2
#define GOOMBA_ANI_FLY_WALKING	3
#define GOOMBA_ANI_FLY_JUMPING	4

#define GOOMBA_GRAVITY	0.0008f

#define GOOMBA_POINT	100

#define GOOMBA_BODY_EXIST_TIME	80

#define GOOMBA_LEVEL_WALK	0
#define GOOMBA_LEVEL_FLY	1

#define GOOMBA_WALKING_TIME	1000

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	int level;
	bool isStanding = 0;
	bool aboutToJump = 0;

	DWORD start_die;
	DWORD start_walking;
public: 	
	CGoomba();
	CGoomba(int level = 0);
	virtual void SetState(int state);

	void GetHit(int nx);
	int GetLevel();
	void SetLevel(int level);
	void GetJumpedOn();
	void LooseWings();
};