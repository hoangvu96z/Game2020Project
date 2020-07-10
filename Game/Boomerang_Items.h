#pragma once
#include "GameObject.h"

#define BOOMERANG_BBOX_WIDTH		16
#define BOOMERANG_BBOX_HEIGHT	12

class Boomerang_Items: public CGameObject
{
	DWORD turnoverDelayTime=0;
public:
	Boomerang_Items();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};