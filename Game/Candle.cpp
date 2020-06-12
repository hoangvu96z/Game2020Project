﻿#include "Candle.h"

CCandle::CCandle()
{}
void CCandle::Render()
{
	animation_set->at(state)->Render(x, y, -1);
}

// Update Candle
void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state== CANDLE_DESTROYED 
		&& animation_set->at(state)->IsOver(CANDLE_DESTROYED_EFFECT_TIME))
	{
		this->visible = false;		
		CItems::GetInstance()->CheckAndDrop(this);
		return;
	}
}


void CCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (state)
	{
		case CANDLE_STATE_BIG:
		{
			right = x + CANDLE_BIG_BBOX_WIDTH;
			bottom = y + CANDLE_BIG_BBOX_HEIGHT;
		}
		case CANDLE_STATE_SMALL:
		{
			right = x + CANDLE_SMALL_BBOX_WIDTH;
			bottom = y + CANDLE_SMALL_BBOX_HEIGHT;
		}
	}
}