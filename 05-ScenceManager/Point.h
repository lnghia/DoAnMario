#pragma once
#include "GameObject.h"
#include "Grid.h"

#define POINT_ANI_100	0
#define POINT_ANI_200	1
#define POINT_ANI_400	2
#define POINT_ANI_800	3
#define POINT_ANI_1000	4
#define POINT_ANI_2000	5
#define POINT_ANI_4000	6
#define POINT_ANI_8000	7

#define POINT_ANI_SET	911

#define POINT_FLY_TIME	480

#define POINT_SPEED_Y	-0.2f

#define POINT_BBOX_WIDTH	11
#define POINT_BBOB_HEIGHT	8

#define POINT_GRAVITY	0.0005f

//#define POINT_WIDTH 16
//#define POINT_HEIGHT 



class Point : public CGameObject
{
	unsigned short int point;
	DWORD fly_start = 0;
public:
	Point(){}
	Point(unsigned short int point, float x, float y);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

