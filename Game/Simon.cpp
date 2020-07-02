﻿#include "Simon.h"
#include "Candle.h"

CSimon::CSimon(float x, float y) :CGameObject()
{
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	SetState(SIMON_STATE_IDLE);
	whip = new CWhip();
	this->autoMove = false;
	nextSceneWhip  = CWhip::GetInstance();
	whip->SetState(nextSceneWhip->GetState());
	dagger = new CDagger();
}

void CSimon::Reset()
{
	onStairs = 0;
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CSimon::Update(DWORD dt, vector <LPGAMEOBJECT>* coObjects)
{
	// Calculate x,y
	CGameObject::Update(dt);
	whip->Update(dt, coObjects);
	dagger->Update(dt, coObjects);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	// Simple logic with screen edge
	if (vx < 0 && x < 0) x = 0;
	vector <LPCOLLISIONEVENT> coEvents;
	vector <LPCOLLISIONEVENT> coEventsResult;

	// turn off collision when simon is die
	if (state != SIMON_STATE_DIE)
	{
		CalcPotentialCollisions(coObjects, coEvents);
	}

	// No collision, process normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;			
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*
		Collision logic with others objects
		*/
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];
			// collision logic with Candle
			if (dynamic_cast<CCandle*>(e->obj))
			{
				// Process normally
				x += dx;
				y += dy;
			}
			// Collision logic with Brick 
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (onStairs == 0)//Turn off collision with brick when simon is on stairs
				{
					if (e->ny != 0)
					{
						if (e->ny == -1) vy = 0;
						else 	y += dy;
					}
				}
				else // Process normally
				{							
					x += dx;
					y += dy;
				}	
			}

			// Collision logic with item
			else if (dynamic_cast<BHeart_Items*>(e->obj))
			{
				DebugOut(L"[ITEMS] Collision with Heart\n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					subWeapon = true;
				}
			}
			else if (dynamic_cast<Chain_Items*>(e->obj))
			{
				DebugOut(L"[ITEMS] Collision with Chain\n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					this->whip->PowerUp();
					this->nextSceneWhip->PowerUp();
				}
			}
			else if (dynamic_cast<CStartStair*> (e->obj))
			{
				DebugOut(L"[INFO] Start Stair detection ! Direction: %d \n",this->nx);
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;

			}
			else if (dynamic_cast<CEndStair*> (e->obj))
			{
				DebugOut(L"[INFO] End Stair detection ! Direction: %d \n", this->nx);
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;

			}
			else if (dynamic_cast<Dagger_Items*>(e->obj))
			{
				DebugOut(L"[ITEMS] Collision with Dagger\n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
				}
			}
			// switching scene logic		
			else if (dynamic_cast<CPortal*> (e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*> (e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;				
			}
		}
	}

	// Clear up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}

	// Check collsion when simon attacking
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK)
	{
		whip->SetOrientation(nx);
		whip->SetWhipPosition(D3DXVECTOR2(x, y), isStanding);
		if (animation_set->at(state)->GetCurrentFrame() == 2) // Only check collsion at the last frame of the whip
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT temp = coObjects->at(i);
				if (dynamic_cast<CCandle*>(temp))
				{
					CCandle* candle = dynamic_cast<CCandle*> (temp);
					float left, top, right, bottom;
					temp->GetBoundingBox(left, top, right, bottom);

					if (whip->isColliding(left, top, right, bottom) == true)
					{
						temp->SetState(CANDLE_DESTROYED);				
						temp->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
					}
				}
			}
		}
	}
	
}
void CSimon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case SIMON_STATE_IDLE:
		{
			isStanding = true;
			vx = 0;
			break;
		}
		case SIMON_STATE_WALKING:
		{
			if (nx > 0)
				vx = SIMON_WALKING_SPEED;
			else
				vx = -SIMON_WALKING_SPEED;
			break;
		}
		case SIMON_STATE_DIE:
		{
			vy = -SIMON_DIE_DEFLECT_SPEED;
			break;
		}
		case SIMON_STATE_JUMP:
		{
			isStanding = true;
			vy = -SIMON_JUMP_SPEED_Y;
			break;
		}
		case SIMON_STATE_SIT:
		{
			isStanding = false;
			vx = 0;
			vy = 0;
			break;
		}
		case SIMON_STATE_ATTACK:
		{
			animation_set->at(SIMON_ANI_ATTACK)->Reset();
			animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
			break;
		}
		case SIMON_STATE_SIT_ATTACK:
		{
			animation_set->at(SIMON_ANI_SIT_ATTACK)->Reset();
			animation_set->at(SIMON_ANI_SIT_ATTACK)->SetAniStartTime(GetTickCount());
			break;
		}
		case SIMON_STATE_THROW:
		{
			vx = 0;
			animation_set->at(SIMON_ANI_THROW)->Reset();
			animation_set->at(SIMON_ANI_THROW)->SetAniStartTime(GetTickCount());
			break;
		}
		case SIMON_STATE_GO_UPSTAIR:
		{
			GoUpStair();
			break;
		}
		case SIMON_STATE_GO_DOWNSTAIR:
		{
			GoDownStair();
			break;
		}
	}
}

void CSimon::Render()
{
	int ani = -1;

	if (state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_IDLE;
	}
	else if (state == SIMON_STATE_ATTACK)
	{
		if (onStairs == 1)
		{
			ani = SIMON_ANI_ATTACK_UPSTAIR;
		}
		else if (onStairs == -1)
		{
			ani = SIMON_ANI_ATTACK_DOWNSTAIR;
		}
	}
	else if (state == SIMON_STATE_SIT_ATTACK) ani = SIMON_ANI_SIT_ATTACK;
	else if (state == SIMON_STATE_THROW) ani = SIMON_ANI_THROW;
	else if (state == SIMON_STATE_JUMP) ani = SIMON_ANI_JUMP;
	else if (state == SIMON_STATE_SIT) ani = SIMON_ANI_SIT;
	// else if (state == SIMON_STATE_GO_UPSTAIR) ani = SIMON_ANI_GO_UPSTAIR;
	else if (onStairs !=0)
	{
		if (vx != 0)
		{
			if (onStairs == 1) ani = SIMON_ANI_GO_UPSTAIR;
			else if (onStairs == -1) ani = SIMON_ANI_GO_DOWNSTAIR;
		}
		else
		{
			if (onStairs == 1) ani = SIMON_ANI_IDLE_UPSTAIR;
			else if (onStairs == -1) ani = SIMON_ANI_IDLE_DOWNSTAIR;
		}

	}
	else
	{
		if (vx == 0) ani = SIMON_ANI_IDLE;
		else	ani = SIMON_ANI_WALKING;
	}
	
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, nx, alpha);
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK)
	{
		whip->Render(animation_set->at(ani)->GetCurrentFrame());
	}
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y+ SIMON_BBOX_HEIGHT;
}

CSimon* CSimon::__instance = NULL;
CSimon* CSimon::GetInstance()
{
	if (__instance == NULL) __instance = new CSimon();
	__instance->visible = true;
	return __instance;
}

void CSimon::GoUpStair()
{
	// Check if Simon is on stairs and want to go up
	if (onStairs == 0)
	{
		LPGAMEOBJECT stairs = NULL;

		// Check if Simon is on a stairs-start
		for (UINT i = 0; i < ovObjects.size(); ++i)
		{
			if (dynamic_cast<CStairBottom *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}
		}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL)
		{
			state = SIMON_STATE_IDLE;
			return;
		}

		// Enable go up stair
		float xS, yS;
		stairs->GetPosition(xS, yS);
		nx = stairs->GetOrientation();
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = -SIMON_GO_UPSTAIR_SPEED;
		onStairs = 1;
	}
	else
	{
		if (onStairs == -1) // Simon is going down stair and want to go back
		{
			this->nx = -nx;
			onStairs = 1;
		}
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = -SIMON_GO_UPSTAIR_SPEED;
	}
}

void CSimon::ProceedOnStairs()
{
	// Try getting out the stairs
	LPGAMEOBJECT stairs = NULL;
	if (onStairs == 1)
	{
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairTop *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		if (stairs == NULL)
			return;

		float xS, yS;
		stairs->GetPosition(xS, yS);

		// Check if simon has reached the stairs exit
		if (y < yS)
		{
			y = yS - 0.2f;
			onStairs = 0;
			vx = vy = 0;
		}
	}
	else if (onStairs == -1)
	{
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairBottom *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		if (stairs == NULL)
			return;

		float xS, yS;
		stairs->GetPosition(xS, yS);

		// Check if simon has reached the stairs exit
		if (y > yS)
		{
			y = yS - 0.2f;
			onStairs = 0;
			vx = vy = 0;
		}
	}
}

void CSimon::GoDownStair()
{
	// Check if Simon is on stairs and want to go down
	if (onStairs == 0)
	{
		LPGAMEOBJECT stairs = NULL;

		// Check if Simon is on a stairs-start
		for (UINT i = 0; i < ovObjects.size(); ++i)
		{
			if (dynamic_cast<CStairTop *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}
		}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL)
		{
			state = SIMON_STATE_IDLE;
			return;
		}

		// Enable go down stair
		float xS, yS;
		stairs->GetPosition(xS, yS);
		nx = stairs->GetOrientation();
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = SIMON_GO_UPSTAIR_SPEED;
		onStairs = -1;
	}
	else
	{
		if (onStairs == 1) // Simon is going up stair and want to go back
		{
			this->nx = -nx;
			onStairs = -1;
		}
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = SIMON_GO_UPSTAIR_SPEED;
	}
}

void CSimon::StartAutoMove(float vx, float xDestination)
{
	if (!autoMove)
	{
		autoMoveInfo.xDes = xDestination;
		autoMoveInfo.vx = vx;
		autoMove = true;
	}
	// Proceed Auto Move
}

// Overlapping
	ovObjects.clear();
	for (UINT i = 0; i < coObjects->size(); ++i)
		if (this->IsOverlapping(coObjects->at(i)))
			ovObjects.push_back(coObjects->at(i));

	// Being On Stairs
	if (onStairs != 0)
	{
		ProceedOnStairs();	
		vy = 0;
	}