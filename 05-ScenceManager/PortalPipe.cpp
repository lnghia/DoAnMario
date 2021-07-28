#include "PortalPipe.h"

PortalPipe::PortalPipe()
{
}

void PortalPipe::Render()
{
	RenderBoundingBox();
}

void PortalPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

float PortalPipe::GetSpawnX()
{
	return spawnX;
}

float PortalPipe::GetSpawnY()
{
	return spawnY;
}

float PortalPipe::GetWidth()
{
	return width;
}

float PortalPipe::GetHeight()
{
	return height;
}

void PortalPipe::SetSceneId(int val)
{
	sceneId = val;
}

void PortalPipe::SetGetOutPipeDirection(int val)
{
	getOutPipeDirection = val;
}

void PortalPipe::SetExitPoint(float ex, float ey, int width, int height)
{
	exitX = ex;
	exitY = ey;
	this->exitWidth = width;
	this->exitHeight = height;
}
