#include "Boomerang_Weapons.h"
#include "Simon.h"

Boomerang_Weapons::Boomerang_Weapons(): CGameObject()
{
	vx = 0.12f;
}

void Boomerang_Weapons::Render()
{
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

	// Boomerang moving logic
	vx = 0.12f*nx;

	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	float xBm, yBm;
	this->GetPosition(xBm, yBm);
	if (xBm - xS > 150)
	{
		if (turnoverDelayTime < 150)
		{
			vx = 0;
			turnoverDelayTime += dt;
			
		}
		else
		{
			nx = -nx;
			vx = -vx;
			turnoverDelayTime = 0;
		}
	}


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
