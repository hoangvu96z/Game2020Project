#include "BlackKnight_Enemies.h"
#include "Brick.h"

void BlackKnight_Enemies::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x; 
	top = y;
	right = left + BLACK_KNIGHT_BBOX_WIDTH;
	bottom = top + BLACK_KNIGHT_BBOX_HEIGHT;
}

void BlackKnight_Enemies::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += 0.0018f * dt;
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
		float min_tx, min_ty, nx,ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx;
		y += min_ty * dy;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* b = dynamic_cast<CBrick*>(e->obj);
				this->start_x = b->x+1;
				float end_x = start_x + b->width- BLACK_KNIGHT_BBOX_WIDTH-1;
				if (e->ny!=0)
				{
					vy = 0;
					y += ny * 0.4f;
				}
				if (x >= end_x || x <= start_x)
				{
					ReDirection();
				}
			}
		}
	}
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BlackKnight_Enemies::Render()
{
	animation_set->at(state)->Render(x, y, nx);
}

BlackKnight_Enemies::BlackKnight_Enemies(float x, float y)
{
	this->start_x = x;
	this->start_y = y;
	SetState(BLACK_KNIGHT_STATE_WALKING);
}

void BlackKnight_Enemies::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BLACK_KNIGHT_STATE_WALKING:
		vx = BLACK_KNIGHT_WALKING_SPEED;
		vy = 0;
		break;
	}

}