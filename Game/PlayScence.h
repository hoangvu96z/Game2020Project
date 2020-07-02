#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Simon.h"
#include "TileMap.h"
#include "Candle.h"
#include "Whip.h"
#include "Items.h"
#include "BHeart_Items.h"
#include "Chain_Items.h"
#include "HUD.h"
#include "Portal.h"
#include "BlackKnight_Enemy.h"
#include "Dagger_Items.h"
#include "Dagger.h"
#include "StartStair.h"
#include "EndStair.h"
#include<vector>

#define SCREEN_WIDTH	 250
#define SCREEN_HEIGHT 230

#define TILE_WIDTH	32
#define TILE_HEIGHT	32
using namespace std;
class CPlayScene: public CScene
{
protected: 
	CSimon* player;
	CWhip* whip;
	CDagger* dagger;
	vector<LPGAMEOBJECT> objects;
	int mapWidth, offset_y;
	vector<LPTILE> tiledMap;
	HUD* HUD;

	void _ParseSection_MAP_INFO(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILE_MAP(string line);

public: 
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	friend class CPlayScenceKeyHandler;

	CSimon* GetPlayer() { return player; }
	CDagger* GetDagger() { return dagger; }
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{	
	public: 	
		void KeyState(BYTE *states);
		void OnKeyDown(int KeyCode);
		void OnKeyUp(int KeyCode);
		CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};