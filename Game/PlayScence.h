#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Simon.h"
#include "BreakWall.h"
#include "WallPieces.h"
#include "ScoreBoard.h"
#include "TileMap.h"
#include "Candle.h"
#include "Portal.h"
#include "Whip.h"
#include "Items.h"
#include "StartStair.h"
#include "EndStair.h"
#include "VariousStair.h"
#include "MovingPlatform.h"
#include "Dagger.h"
#include "Door.h"
// #include "Gird.h"

#include "BHeart_Items.h"
#include "Boomerang_Items.h"
#include "MoneyPocket_Items.h"
#include "SHeart_Items.h"
#include "Chain_Items.h"
#include "Crown_Items.h"
#include "Dagger_Items.h"
#include "HolyWater_Items.h"

#include "Boomerang_Weapons.h"
#include "HolyWater_Weapons.h"

#include "Bat_Enemies.h"
#include "BlackKnight_Enemies.h"
#include "Zombie_Enemies.h"
#include "HunchBack_Enemies.h"
#include "Skeleton_Enemies.h"
#include<vector>

#define SCREEN_WIDTH	 270
#define SCREEN_HEIGHT 270

#define TILE_WIDTH	32
#define TILE_HEIGHT	32

#define CELL_WIDTH	90
#define CELL_HEIGHT	48
using namespace std;
class CPlayScene: public CScene
{
protected: 
	//CGrid *grid;
	int mapWidth, mapHeight;
	int offset_y; // an empty space to render HUD

	vector<LPTILE> tiledMap;	
	vector<LPGAMEOBJECT> objects;				// Base objects

	CSimon* player;
	CWhip* whip;
	CDagger* dagger;
	ScoreBoard* HUD;
	Boomerang_Weapons* boomerang;

	void _ParseSection_PLAYER(string line);
	void _ParseSection_MAP_INFO(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILE_MAP(string line);

public: 
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CSimon* GetPlayer() { return player; }
	CDagger* GetDagger() { return dagger; }
	Boomerang_Weapons* GetBoomerang() { return boomerang; }
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{	
public: 	
	void KeyState(BYTE *states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};