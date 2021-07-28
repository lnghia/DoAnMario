#include "BangEffect.h"

void BangEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BANG_EFFECT_BBOX_WIDTH;
	bottom = top + BANG_EFFECT_BBOX_HEIGHT;
}

void BangEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((DWORD)GetTickCount64() - start_live > LIFE_SPAN) {
		isActive = 0;
		interactivable = 0;
		invisible = 1;
		Grid::GetInstance()->ObjIntoTrash(this);
	}
}

void BangEffect::Render()
{
	CAnimationSets::GetInstance()->Get(BANG_EFFECT_ANI_SET)->at(0)->Render(x, y);
}

void BangEffect::RenderCurrFrame()
{
	CAnimationSets::GetInstance()->Get(BANG_EFFECT_ANI_SET)->at(0)->RenderCurrFrame(x, y);
}
