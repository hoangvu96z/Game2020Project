#pragma once

#include "PlayScence.h"
#include "GameObject.h"
#define CANDLE_STATE_BIG		0
#define CANDLE_STATE_SMALL	1
#define CANDLE_DESTROYED		2

#define CANDLE_BIG_BBOX_WIDTH	16
#define CANDLE_BIG_BBOX_HEIGHT	32

#define CANDLE_SMALL_BBOX_WIDTH	8
#define CANDLE_SMALL_BBOX_HEIGHT	16

#define CANDLE_DESTROYED_EFFECT_TIME 300
class CCandle : public CGameObject
{
public:
	CCandle();	
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};