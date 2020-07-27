#include "Boomerang_Weapons.h"
#include "Simon.h"
#include "Candle.h"

#define BOOMERANG_VX	0.12f
#define BOOMERANG_MAX_DISTANCE	150

Boomerang_Weapons::Boomerang_Weapons(): CGameObject()
{
	vx = BOOMERANG_VX;
	this->damage = 2;
}

void Boomerang_Weapons::Render()
{
	ShowHitEffect();
	animation_set->at(0)->Render(x, y, nx);
}

void Boomerang_Weapons::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}

void Boomerang_Weapons::Update(DWORD dt, vector <LPGAMEOBJECT>* coObjects)
{

	float left, top, right, bottom;
	CGame::GetInstance()->GetCameraBoundingBox(left, top, right, bottom);

	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	if (!turnOver)
	{
		if (x > (right - BOOMERANG_BBOX_WIDTH) || x <= left)
		{
			this->nx = -nx;
			turnOver = true;
		}
		else if (abs(x - xS) > BOOMERANG_MAX_DISTANCE)
		{
			this->nx = -nx;
			turnOver = true;
		}
	}
	else
	{
		if ((x > right) || (x <= left)) 
		{
			SetVisible(false);
			turnOver = false;
		}
	}
	vx = BOOMERANG_VX * nx;
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size()==0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}
void Boomerang_Weapons::ShowHitEffect()
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

		// rendering hit effect based on the coordinate vector
		for (auto coord : hitEffects)
			hitEffect->Render(coord[0], coord[1], -1);
	}
}