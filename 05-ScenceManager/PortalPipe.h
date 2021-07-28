#pragma once
#include "PipeHitBox.h"

class PortalPipe : public PipeHitBox
{
	float spawnX = 0.0f;
	float spawnY = 0.0f;

	int ny = -1;
public:
	int sceneId = 0;
	int getOutPipeDirection = -1;
	float exitX = 0;
	float exitY = 0;
	int exitWidth = 0;
	int exitHeight = 0;

	PortalPipe();
	PortalPipe(int width, int height) :PipeHitBox(width, height) {
		canBeStandOn = true;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	float GetSpawnX();
	float GetSpawnY();
	float GetWidth();
	float GetHeight();

	virtual void SetSceneId(int val);
	virtual void SetGetOutPipeDirection(int val);
	virtual void SetExitPoint(float ex, float ey, int width, int height);
};

