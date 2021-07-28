#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <unordered_set>

#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 

	int state;

	bool canBeStandOn = 0;

	DWORD dt; 

	LPANIMATION_SET animation_set;

	bool interactivable = 1;
	bool invisible = 0;
	bool isActive = 1;

	unsigned int renderPriority = 0;

	//bool isPaused = 0;

	UINT currAni;

public: 
	int id = 0;

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(const float& vx, const float& vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void SetCanBeStandOn(bool val);
	bool GetCanBeStandOn();
	DWORD GetDeltaTime();

	void SetVx(const float& vx);
	void SetVy(float vy);

	void SetRenderPriority(unsigned int p);
	unsigned int GetRenderPriority();

	bool GetInteractivable();
	void SetInteractivable(bool val);

	bool GetInvisible();
	void SetInvisible(bool val);

	bool GetIsActive();

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void GetKicked(int nx) {};
	virtual void AddHiddenItem(int hiddenItem, int hiddenItemAni) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void RenderCurrFrame();
	virtual void SetState(int state) { this->state = state; }
	virtual void GetHit(bool byTail, int nx) {};
	virtual void SetSceneId(int val) {};
	virtual void SetGetOutPipeDirection(int val) {};
	virtual void SetExitPoint(float ex, float ey, int width, int height) {};

	bool checkAABB(LPGAMEOBJECT obj);

	~CGameObject();
};

