#pragma once
#include "Koopas.h"

#define REDKOOPAS_ANI_OUT_A_SHELL			4
#define REDKOOPAS_ANI_IN_SHELL_UPWARD		5
#define REDKOOPAS_ANI_OUT_A_SHELL_UPWARD	6
#define REDKOOPAS_ANI_SPIN_UPWARD			7

class RedKoopas : public CKoopas
{
	bool isStanding = 0;

	int beneathSurfaceX;
	int beneathSurfaceY;
	int beneathSurfaceW;
	int beneathSurfaceH;
public:
	RedKoopas();
	RedKoopas(short int nx);
	RedKoopas(short int nx, const int& x, const int& y, const int& w, const int& h);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};

