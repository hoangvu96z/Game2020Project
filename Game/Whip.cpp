﻿#include "Whip.h"
#include "Candle.h"
#define WHIP_ANI_SET 36

CWhip::CWhip() : CGameObject()
{
	SetState(NORMAL_WHIP);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SetDamage(state);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT temp = coObjects->at(i);
		if (this->IsOverlapping(temp))
		{
			if (dynamic_cast<CCandle*>(temp))
			{
				temp->SetState(CANDLE_DESTROYED);
				temp->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
			}

			else if (dynamic_cast<CBreakWall*>(temp))
			{
				CBreakWall* breakwall = dynamic_cast<CBreakWall*>(temp);
				breakwall->Destroy();
			}

			else if (dynamic_cast<BlackKnight_Enemies*>(temp) || dynamic_cast<Bat_Enemies*>(temp))
			{
				temp->TakeDamage(this->damage);
				float l, t, r, b;
				temp->GetBoundingBox(l, t, r, b);
				hitEffects.push_back({ (l + r) / 2, (t + b) / 2 });				
			}
		}
	}
}

void CWhip::SetDamage(int state)
{
	if (state == NORMAL_WHIP)
	{
		damage = 1;
	}
	else damage = 2;
}


void CWhip::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	top = y + OFFSET_TOP;
	bottom = top + WHIP_BBOX_HEIGHT;
	switch (state)
	{
	case NORMAL_WHIP:
	{
		if (nx > 0)
			left = x + OFFSET_RIGHT_CHAIN;
		else
			left = x + OFFSET_LEFT_SHORT_CHAIN;
		right = left + SHORT_CHAIN_BBOX_WIDTH;
		break;
	}

	case SHORT_CHAIN:
	{
		if (nx > 0)
			left = x + OFFSET_RIGHT_CHAIN;
		else
			left = x + OFFSET_LEFT_SHORT_CHAIN;
		right = left + SHORT_CHAIN_BBOX_WIDTH;
		break;
	}

	case LONG_CHAIN:
	{
		if (nx > 0)
			left = x + OFFSET_RIGHT_CHAIN;
		else
			left = x + OFFSET_LEFT_LONG_CHAIN;
		right = left + LONG_CHAIN_BBOX_WIDTH;
		break;
	}
	}
}

void CWhip::ShowHitEffect()
{
	if (hitEffects.size() > 0)
	{
		if (startShow == 0)
		{
			startShow = GetTickCount();
		}
		else if (GetTickCount() - startShow > HIT_EFFECT_LIFE_SPAN)
		{
			startShow = 0;
			hitEffects.clear();
		}
		for (auto effect : hitEffects)
		{
			hitEffect->Render( effect[0], effect[1], -1);
		}
	}
}
void CWhip::SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding)
{
	if (nx > 0)
	{
		simonPos.x -= 49.0f;
		if (isStanding)
			simonPos.y -= 3.0f;
		else
			simonPos.y += 3.0f;
	}
	else
	{
		simonPos.x -= 53.0f;
		if (isStanding)
			simonPos.y -= 3.0f;
		else
			simonPos.y += 3.0f;
	}
	SetPosition(simonPos.x, simonPos.y);
}

void CWhip::PowerUp()
{
	if (state == NORMAL_WHIP)
		SetState(SHORT_CHAIN);
	else if (state == SHORT_CHAIN)
		SetState(LONG_CHAIN);
}

CWhip* CWhip::__instance = NULL;
CWhip* CWhip::GetInstance()
{
	if (__instance == NULL) __instance = new CWhip();
	return __instance;
}

void CWhip::Render(int currentFrame)
{
	CAnimationSets::GetInstance()->Get(WHIP_ANI_SET)->at(state)->RenderByFrame(currentFrame, nx, x, y);
	ShowHitEffect();
}

void CWhip::SetState(int state)
{
	this->state = state;
}