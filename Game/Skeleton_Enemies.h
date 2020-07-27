#pragma once
#include "GameObject.h"

#define SKELETON_BBOX_WIDTH	16
#define SKELETON_BBOX_HEIGHT	32

#define SKELETON_STATE_DANCING	0

class Skeleton_Enemies : public CGameObject
{
	float start_x;
	float start_y;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	Skeleton_Enemies(float x = 0.0f, float y = 0.0f);
	virtual void SetState(int state);
}; 