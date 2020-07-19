#include "SHeart_Items.h"
#include "Brick.h"
#include "EndStair.h"
#include "Simon.h"
SmallHeart_Items::SmallHeart_Items()
{	
	this->visible = false;	
	this->vx_variability = 0.0007f;
}

void SmallHeart_Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	CGameObject::Update(dt);
	vy += ITEM_GRAVITY * dt;				// simple fall down	

	if (vy != 0)
	{
		vx += vx_variability * dt;		
		vy = 0.06f;

		if (vx >= 0.15f || vx <= -0.15f)
			vx_variability *= -1;
	}
	else vy = 0;	

	if (visible)
	{	
		if (start_visible < ITEM_LIFESPAN)
		{
			start_visible += dt;
		}
		else
		{
			SetVisible(false);
			start_visible = 0;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Block brick
				if (e->ny != 0 || e->nx!=0)
				{
					vx = vy = 0;
				}
			}
			else if (dynamic_cast<CEndStair*>(e->obj))
			{
				// Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void SmallHeart_Items::Render()
{
	animation_set->at(0)->Render(x, y, 1);
}


void SmallHeart_Items::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_SMALL_HEART_BBOX_WIDTH;
	bottom = y + ITEM_SMALL_HEART_BBOX_HEIGHT;
}