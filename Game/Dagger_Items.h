#pragma once
#include "GameObject.h"

#define ITEM_DAGGER_BBOX_WIDTH		16
#define ITEM_DAGGER_BBOX_HEIGHT	10

class Dagger_Items :public CGameObject
{
public:
	Dagger_Items();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};