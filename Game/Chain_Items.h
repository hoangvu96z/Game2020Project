#pragma once
#include "GameObject.h"

#define ITEM_CHAIN_BBOX_WIDTH		16
#define ITEM_CHAIN_BBOX_HEIGHT		16

class Chain_Items :public CGameObject
{
		DWORD timeAppear = -1;
public:
	Chain_Items();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};