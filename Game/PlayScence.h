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
#include "Portal.h"
#include "BlackKnight_Enemy.h"
#include "Dagger_Items.h"
#include "Dagger.h"

#define SCREEN_WIDTH	 250
#define SCREEN_HEIGHT 230

#define MAP_1					100
#define MAP_1_WIDTH		768
#define MAP_1_HEIGHT		192
#define MAP_1_MARGIN		22

#define MAP_2					200
#define MAP_2_WIDTH		300
#define MAP_2_HEIGHT		193
#define MAP_2_MARGIN		-6

#define MAP_1_TEX_PATH  L"resources\\tilesheet1.png"
#define MAP_1_MATRIX_PATH  L"resources\\matrix1.txt"

#define MAP_2_TEX_PATH  L"resources\\tilesheet2.png"
#define MAP_2_MATRIX_PATH  L"resources\\matrix2.txt"

#define TILE_WIDTH	32
#define TILE_HEIGHT	32
using namespace std;
class CPlayScene: public CScene
{
protected: 
	CSimon* player;	// A play scene has to have player, right? 
	CWhip* whip;
	CDagger* dagger;
	vector<LPGAMEOBJECT> objects;
	int mapWidth, offset_y;
	vector<LPTILE> tiledMap;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
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
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{	
	public: 	
		void KeyState(BYTE *states);
		void OnKeyDown(int KeyCode);
		void OnKeyUp(int KeyCode);
		CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};