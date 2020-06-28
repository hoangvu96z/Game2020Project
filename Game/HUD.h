#pragma once
#include"Game.h"
#include"GameObject.h"
#include "Simon.h"
#include "DrawCharacters.h"

class HUD
{
	//int score;
	int time;

	static HUD* __instance;

	DrawCharacters* code;
	DWORD count;
	bool isStop = false;

public:
	HUD();
	void Render();
	void Update(DWORD dt);
	static HUD* GetInstance();
};