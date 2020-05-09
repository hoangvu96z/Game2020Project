
#include "BHeart_Items.h"
#include "Brick.h"

BHeart_Items::BHeart_Items()
{
	this->visible = false;
}

void BHeart_Items::Update(DWORD dt, vector<LPGAMEOBJECT> *objects)
{
	CGameObject::Update(dt);
	vy += ITEM_GRAVITY * dt; // simple fall down

	vector<LPCOLLISIONEVENT> events;
	vector<LPCOLLISIONEVENT> eventsResult;

	events.clear();
	CalcPotentialCollisions(objects, events);

	if (events.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(events, eventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < eventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = eventsResult[i];

			if (dynamic_cast<CBrick *>(e->obj))
			{
				if (e->ny != 0)
				{
					y += 0.4f * e->ny;
					vy = 0;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < events.size(); i++)
		delete events[i];
}

void BHeart_Items::Render()
{
	animation_set->at(0)->Render(x, y, nx);
}

void BHeart_Items::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + ITEM_BIG_HEART_BBOX_WIDTH;
	bottom = y + ITEM_BIG_HEART_BBOX_HEIGHT;
}
