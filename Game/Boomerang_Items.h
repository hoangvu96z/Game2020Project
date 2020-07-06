#pragma once
#include "GameObject.h"

#define ITEM_BOOMERANG_BBOX_WIDTH	16
#define ITEM_BOOMERANG_BBOX_HEIGHT	12

class Boomerang_Items :public CGameObject
{
public:
	Boomerang_Items();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};