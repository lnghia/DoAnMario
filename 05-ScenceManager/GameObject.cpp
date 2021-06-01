#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Grid.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Grid::GetInstance()->clearObjFromGrid(this);

	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;

	Grid::GetInstance()->putObjectIntoGrid(this);
}

void CGameObject::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	float l1, r1, t1, b1;
	float l2, r2, t2, b2;

	GetBoundingBox(l1, t1, r1, b1);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		/*coObjects->at(i)->GetBoundingBox(l2, t2, r2, b2);

		if (doOverlap(l1, t1, r1, b1, l2, t2, r2, b2)) {
			continue;
		}*/

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CGameObject::SetCanBeStandOn(bool val)
{
	canBeStandOn = val;
}

bool CGameObject::GetCanBeStandOn() {
	return canBeStandOn;
}

DWORD CGameObject::GetDeltaTime()
{
	return dt;
}

void CGameObject::SetRenderPriority(unsigned int p)
{
	renderPriority = p;
}

unsigned int CGameObject::GetRenderPriority()
{
	return renderPriority;
}

bool CGameObject::GetInteractivable()
{
	return interactivable;
}

void CGameObject::SetInteractivable(bool val) {
	interactivable = val;
}

bool CGameObject::GetInvisible()
{
	return invisible;
}

void CGameObject::SetInvisible(bool val) {
	invisible = val;
}

bool CGameObject::GetIsActive()
{
	return isActive;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 70);
}


bool CGameObject::checkAABB(LPGAMEOBJECT obj)
{
	float l, t, r, b;
	float l1, t1, r1, b1;

	this->GetBoundingBox(l, t, r, b);
	obj->GetBoundingBox(l1, t1, r1, b1);

	return !(r < l1 || l > r1 || t > b1 || b < t1);
}

CGameObject::~CGameObject()
{

}