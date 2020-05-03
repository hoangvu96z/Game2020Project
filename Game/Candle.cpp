﻿#include "Candle.h"

CCandle::CCandle()
{ ; }

void CCandle::Render()
{
	animation_set->at(state)->Render(x, y, nx);
}

void CCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// Continue here
	left = x;
	top = y;
	right = x + CANDLE_BBOX_WIDTH;
	bottom = y + CANDLE_BBOX_HEIGHT;
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT>* coObject)
{
	if (state== CANDLE_DESTROYED&& animation_set->at(state)->IsOver(CANDLE_DESTROYED_EFFECT_TIME))
	{
		this->visible = false;
	}
}