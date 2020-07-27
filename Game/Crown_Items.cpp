#include "Crown_Items.h"

Crown_Items::Crown_Items()
{
	SetVisible(true);
}

void Crown_Items::Render()
{
	animation_set->at(0)->Render(x, y, 1);
}

void Crown_Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	if (y <= CROWN_MAX_Y)
	{
		y = CROWN_MAX_Y; // Stop growing up	
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
	else
	{
		if (xS >= CROWN_ACTIVE_POINT_X)  // Active point
		{
			vy = -CROWN_GROWING_UP_SPEED;
		}
	}
	x += dx;
	y += dy;
}

void Crown_Items::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CROWN_BBOX_WIDTH;
	bottom = y + CROWN_BBOX_HEIGHT;
}