#pragma once
#include "GameObject.h"

#define ITEM_BIG_HEART_BBOX_WIDTH		12
#define ITEM_BIG_HEART_BBOX_HEIGHT	10

class BHeart_Items :public CGameObject
{
	DWORD timeAppear = -1;
public:
	BHeart_Items();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};