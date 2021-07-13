#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Boomerang.h"

#define BROS_WALKING_SPEED	0.035f
#define BROS_GRAVITY		0.001f

#define BROS_BOOMERANG_BBOX_WIDTH 24
#define BROS_BOOMERANG_BBOX_HEIGHT 29
#define BROS_BBOX_WIDTH		16
#define BROS_BBOX_HEIGHT		24

#define BROS_STATE_WALKING				 100
#define BROS_STATE_DIE					 200
#define BROS_STATE_WALKING_NO_BOOMERANG	 300

#define BROS_ANI_WALKING_RIGHT	0
#define BROS_ANI_WALKING_LEFT	1
#define BROS_ANI_THREW_RIGHT	2
#define BROS_ANI_THREW_LEFT		3
#define BROS_ANI_DIE			4

#define BOOMERANG_ANI_SET_ID	332

#define BROS_LOADBOOMERANG_TIME 4000


class BoomerangGuy : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	Boomerang* boomerang = NULL;

	bool holdingBoomerang = 0;
	bool threwBoomerang = 0;

	DWORD start_hold_boomerang = 0;

public:
	float xMax = 0;
	float xMin = 0;
	int Load = 0;
	DWORD load_start;
	BoomerangGuy(float max, float min);
	void TakeOutBoomerang();
	void ThrowBoomerang();
	virtual void SetState(int state);

	void LoadBoomerang() { holdingBoomerang = 1; threwBoomerang = 0; load_start = DWORD(GetTickCount64()); }

	void FaceRight();
	void FaceLeft();
};