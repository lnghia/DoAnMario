#include "HiddenItem.h"

#include "Coin.h"
#include "ColorBrickHitBox.h"

HiddenItem::HiddenItem(float x, float y)
{
	this->x = x;
	this->y = y;

	interactivable = 1;
	invisible = 0;
	renderPriority = 100;
	canBeStandOn = 0;
}

void HiddenItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void HiddenItem::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void HiddenItem::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void HiddenItem::GotObsorbed(CMario* mario)
{
	/*float x, y;

	mario->GetPosition(x, y);
	mario->SetPosition(x, y - 0.4f);*/

	isActive = 0;
	interactivable = 0;
	invisible = 1;
}
