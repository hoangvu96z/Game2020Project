#include "Bat_Enemies.h"
#include "BlackKnight_Enemies.h"
#include "Simon.h"
Bat_Enemies::Bat_Enemies(float x, float y) : CGameObject()
{
	start_x = x;
	start_y = y;
	vx = vy = 0;
	this->healthPoint = 2;
	SetState(BAT_STATE_IDLE);
}

void Bat_Enemies::Render()
{
	int ani = -1;
	if (vx == 0)
		ani = 0;
	else
		ani = 1;
	animation_set->at(ani)->Render(x, y, nx);
}

void Bat_Enemies::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BAT_STATE_IDLE:
	{
		vx = vy = 0;
		break;
	}
	case BAT_STATE_FLYING_DOWN:
	{
		vx = 0.08f;
		vy = 0.08f;
		break;
	}
	case BAT_STATE_FLYING_HORIZONTAL:
	{
		vx = 0.1f;
		vy = 0.0f;
		break;
	}
	}
}

void Bat_Enemies::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Activating Bat logic
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	float xB, yB;
	this->GetPosition(xB, yB);

	if (xS - xB <= POINT_ACTIVE_BAT_X && yS - yB <= POINT_ACTIVE_BAT_Y) // Active Point 
	{
		vx = 0.08f;
		vy = 0.08f;
	}
	if (y - start_y >= BAT_FLYING_DOWN_DY)	// Redirecting point
	{
		vx = 0.1f;
		vy = 0.0f;
	}

	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<BlackKnight_Enemies*>(e->obj))
			{
				// Process normally	
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}

	}


	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Bat_Enemies::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGHT;
}