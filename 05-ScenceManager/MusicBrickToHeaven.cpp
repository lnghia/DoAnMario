#include "MusicBrickToHeaven.h"

void MusicBrickToHeaven::Render()
{
	int ani = MUSICBRICKPORTAL_ANI_APPEAR;
	
	if (state == MUSICBRICKPORTAL_STATE_SINK) {
		ani = MUSICBRICKPORTAL_ANI_SINK;
	}
	currAni = ani;

	animation_set->at(ani)->Render(x, y);
}

void MusicBrickToHeaven::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void MusicBrickToHeaven::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (hopUp && (int)((DWORD)GetTickCount64() - start_hopUp) >= MUSICBRICKPORTAL_HOP_SINK_TIME) {
		y = oldY;
		hopUp = 0;
		//interactivable = 1;
		state = MUSICBRICKPORTAL_ANI_APPEAR;
	}
	else if (moveDown && (int)((DWORD)GetTickCount64() - start_moveDown) >= MUSICBRICKPORTAL_HOP_SINK_TIME) {
		y = oldY;
		moveDown = 0;
		state = MUSICBRICKPORTAL_ANI_APPEAR;
	}
	else if (moveHorizontally && (int)((DWORD)GetTickCount64() - start_moveHorizontally) >= MUSICBRICKPORTAL_HOP_SINK_TIME) {
		x = oldX;
		moveHorizontally = 0;
		moveDirect = 0;
	}
}

void MusicBrickToHeaven::RenderCurrFrame()
{
	animation_set->at(currAni)->RenderCurrFrame(x, y);
}

void MusicBrickToHeaven::HopUpABit()
{
	if (state == MUSICBRICKPORTAL_STATE_INVISIBLE) {
		invisible = 0;
	}
	else if (!hopUp) {
		oldY = y;
		//interactivable = 0;
		hopUp = 1;
		start_hopUp = (DWORD)GetTickCount64();
		y -= 3.0f;
	}
}

void MusicBrickToHeaven::MoveHorizontallyABit(int nx)
{
	if (!moveHorizontally) {
		oldX = x;
		moveHorizontally = 1;
		moveDirect = nx;
		start_moveHorizontally = (DWORD)GetTickCount64();
		x += (nx > 0) ? 3.0f : -3.0f;
	}
}

void MusicBrickToHeaven::MoveDownABit()
{
	if (!moveDown) {
		oldY = y;
		//interactivable = 0;
		moveDown = 1;
		start_moveDown = (DWORD)GetTickCount64();
		y += 3.0f;
	}
	state = MUSICBRICKPORTAL_STATE_SINK;
}
