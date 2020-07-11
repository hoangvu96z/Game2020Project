#pragma once
#include"Game.h"
#include"GameObject.h"
#include "Simon.h"
#include "DrawCharacters.h"

class ScoreBoard
{
	int time;	
	static ScoreBoard* __instance;

	DrawCharacters* code;
	DWORD count;
	bool isStop= false;

public: 
	ScoreBoard();	
	void Render();
	void Update(DWORD dt);
	static ScoreBoard* GetInstance();
};