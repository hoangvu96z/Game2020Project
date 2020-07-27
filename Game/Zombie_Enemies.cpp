#include "Zombie_Enemies.h"
#include "Brick.h"

void Zombie_Enemies::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}

Zombie_Enemies::Zombie_Enemies()
{
	this->healthPoint = 1;
	this->vx = 0;
	SetState(ZOMBIE_STATE_MOVING);
}

void Zombie_Enemies::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (start_untouchable != 0)
	{
		Untouchable();
	}
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
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
					y += ny * 0.4f;
				}
			}
		}
	}
}

void Zombie_Enemies::Render()
{
	animation_set->at(0)->Render(x, y, nx);
}

void Zombie_Enemies::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ZOMBIE_STATE_MOVING:
		vx = 0;
		vy = 0;
		break;
	}
}