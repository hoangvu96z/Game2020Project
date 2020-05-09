﻿#pragma once
#include "GameObject.h"
#include "Portal.h"
#include "Utils.h"
#include "Game.h"
#include "Brick.h"
#include "Animations.h"
#include "Whip.h"

#define SIMON_GRAVITY				0.0007f	

#define SIMON_STATE_IDLE						0
#define SIMON_STATE_WALKING				1
#define SIMON_STATE_JUMP						2
#define SIMON_STATE_SIT							3
#define SIMON_STATE_ATTACK					4
#define SIMON_STATE_SIT_ATTACK			5
#define SIMON_STATE_DIE						400

#define SIMON_ANI_IDLE			0
#define SIMON_ANI_WALKING	1
#define SIMON_ANI_JUMP			2
#define SIMON_ANI_SIT				3

#define SIMON_ANI_ATTACK			4
#define SIMON_ANI_SIT_ATTACK		5

#define SIMON_WALKING_SPEED			0.06f
#define SIMON_JUMP_SPEED_Y			0.18f
#define SIMON_DIE_DEFLECT_SPEED	0.5
#define SIMON_ATTACK_TIME	300

#define SIMON_BBOX_WIDTH			15
#define SIMON_BBOX_HEIGHT			30
class CSimon : public CGameObject
{
	CWhip* whip;
	
public: 
	bool isStanding;
	CSimon();
	virtual void Update(DWORD dt, vector <LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool isOnGround() { return vy == 0; }
	void Render();
	void SetState(int state);	

};