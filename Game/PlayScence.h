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
#include "Dagger_Items.h"
#include "Dagger.h"

using namespace std;
class CPlayScene: public CScene
{
protected: 
	CSimon* player;	// A play scene has to have player, right? 
	CWhip* whip;
	CTileMap* map;
	CDagger* dagger;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

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