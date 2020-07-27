#pragma once
#include "GameObject.h"
#include "Simon.h"

#define CROWN_BBOX_WIDTH		16
#define CROWN_BBOX_HEIGHT		16
#define CROWN_MAX_Y					200
#define CROWN_ACTIVE_POINT_X 218
#define CROWN_GROWING_UP_SPEED	0.01f

class Crown_Items : public CGameObject
{
	DWORD start_visible = 0;
public:
	Crown_Items();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};