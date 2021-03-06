﻿#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);	
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_PLAYER					0
#define SCENE_SECTION_OBJECTS					6
#define SCENE_SECTION_MAP_INFO				7
#define SCENE_SECTION_TILE_MAP				8

#define OBJECT_TYPE_SIMON						0
#define OBJECT_TYPE_BRICK						1
#define OBJECT_TYPE_CANDLE					2

#define OBJECT_TYPE_ITEM_BIG_HEART			4
#define OBJECT_TYPE_ITEM_SMALL_HEART	44
#define OBJECT_TYPE_ITEM_CHAIN				5
#define OBJECT_TYPE_ITEM_MONEY_BAG		10

#define OBJECT_TYPE_ITEM_DAGGER				80
#define OBJECT_TYPE_ITEM_BOOMERANG	81
#define OBJECT_TYPE_ITEM_HOLY_WATER		82

#define OBJECT_TYPE_DAGGER					7
#define OBJECT_TYPE_BOOMERANG			71
#define OBJECT_TYPE_HOLY_WATER			72

#define OBJECT_TYPE_ZOMBIE					63
#define OBJECT_TYPE_HUNCH_BACK			64
#define OBJECT_TYPE_SKELETON				65

#define OBJECT_TYPE_BLACK_KNIGHT		8
#define OBJECT_TYPE_BAT							9

#define OBJECT_TYPE_VARIOUS_STAIR	-1
#define OBJECT_TYPE_STAIR_BOTTOM	-2
#define OBJECT_TYPE_STAIR_TOP			-3

#define OBJECT_TYPE_MOVING_PLATFORM	30
#define OBJECT_TYPE_CROWN_ITEM				40
#define OBJECT_TYPE_PORTAL						50
#define OBJECT_TYPE_DOOR							51
#define OBJECT_TYPE_BREAK_WALL				90
#define OBJECT_TYPE_WALL_PIECES				91

#define MAX_SCENE_LINE 1024

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_PLAYER(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* playerObj = NULL;

	if (player != NULL)
	{
		DebugOut(L"[ERROR] SIMON object was created before! ");
		return;
	}

	playerObj = CSimon::GetInstance();
	player = (CSimon*)playerObj;

	playerObj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	playerObj->SetAnimationSet(ani_set);
	DebugOut(L"[INFO] Player object created!\n");
}


void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CItems* items = CItems::GetInstance();
	CGameObject *obj = NULL;

	switch (object_type)
	{	
	// case OBJECT_TYPE_SIMON:
	// {
	// 	if (player != NULL)
	// 	{
	// 		DebugOut(L"[ERROR] SIMON object was created before! ");
	// 		return;
	// 	}
	// 	obj = CSimon::GetInstance();
	// 	player = (CSimon*)obj;
	// 	break;
	// }
	case OBJECT_TYPE_BRICK:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CBrick();
		obj->SetWidth(width);
		obj->SetHeight(height);
		break;

	}
	case OBJECT_TYPE_DAGGER:
	{
		obj = new CDagger();
		dagger = (CDagger*)obj;
		obj->SetVisible(false);
		break;
	}
	case OBJECT_TYPE_BOOMERANG:
	{
		obj = new Boomerang_Weapons();
		boomerang = (Boomerang_Weapons*)obj;
		obj->SetVisible(false);
		break;
	}
	case OBJECT_TYPE_ITEM_SMALL_HEART:
	{
		obj = new SmallHeart_Items ();
		items->AddItem((int)ItemType::SMALL_HEART, obj);
		break;
	}
	case OBJECT_TYPE_MOVING_PLATFORM: obj = new CMovingPlatform(); break;
	case OBJECT_TYPE_CROWN_ITEM: obj = new Crown_Items(); break;
	case OBJECT_TYPE_BREAK_WALL: obj = new CBreakWall(x,y); break;
	case OBJECT_TYPE_WALL_PIECES: 
	{
		obj = new CWallPiece();		
		CWallPieces::GetInstance()->AddPiece((CWallPiece*)obj);
		break;
	}
	case OBJECT_TYPE_HOLY_WATER:
	{
		obj =  HolyWater_Weapons::GetInstance();
		CSubWeapons::GetInstance()->Add((int)SubWeapon::HOLYWATER, obj);
		break;
	}
	case OBJECT_TYPE_BAT:
	{
		obj = new Bat_Enemies(x,y);
		break;
	}	
	case OBJECT_TYPE_BLACK_KNIGHT: 
	{
		obj = new BlackKnight_Enemies(x,y); 
		break;
	}
	case OBJECT_TYPE_ZOMBIE:
	{
		obj = new Zombie_Enemies();
		break;
	}
	case OBJECT_TYPE_SKELETON:
	{
		obj = new Skeleton_Enemies(x, y);
		break;
	}
	case OBJECT_TYPE_HUNCH_BACK:
	{
		obj = new HunchBack_Enemies();
		break;
	}
	case OBJECT_TYPE_CANDLE: 
	{
		int it = atoi(tokens[4].c_str());
		int state = atoi(tokens[5].c_str());
		obj = new CCandle();	
		obj->SetState(state);
		obj->SetItemId(it);		
		break;
	}
	case OBJECT_TYPE_ITEM_BIG_HEART: 
	{
		obj = new BHeart_Items();
		items->AddItem((int)ItemType::BIG_HEART, obj);
		break;
	}
	case OBJECT_TYPE_ITEM_CHAIN:
	{
		 obj = new Chain_Items();
		 items->AddItem((int)ItemType::DAGGER, obj);
		break;
	}
	case OBJECT_TYPE_ITEM_DAGGER:
	{
		obj = new Dagger_Items();
		CItems::GetInstance()->AddItem((int)ItemType::DAGGER, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_MONEY_BAG:
	{
		int state = atoi(tokens[4].c_str());
		obj = new MoneyPocket_Items();
		obj->SetState(state);
		items->AddItem((int)ItemType::MONEY_BAG, obj);
		break;
	}
	case OBJECT_TYPE_ITEM_BOOMERANG:
	{
		obj = new Boomerang_Items();
		items->AddItem((int)ItemType::BOOMERANG, obj);
		break;
	}
	case OBJECT_TYPE_ITEM_HOLY_WATER:
	{
		obj = new HolyWater_Items();
		items->AddItem((int)ItemType::HOLY_WATER, obj);
		break;
	}
	case OBJECT_TYPE_VARIOUS_STAIR:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());
		obj = new CVariousStair(x, y, r, b);		
		obj->SetOrientation(nx);
		break;
	}
	case OBJECT_TYPE_STAIR_BOTTOM:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());		
		obj = new CStartStair(x,y,r,b);
		obj->SetOrientation(nx);
		break;
	}
	case OBJECT_TYPE_STAIR_TOP:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());
		obj = new CEndStair(x, y, r, b);	
		obj->SetOrientation(nx);
		break;
	}
	case OBJECT_TYPE_DOOR:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		obj = new CDoor(x, y, r, b);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;

	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_MAP_INFO(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines
	this->mapWidth = atoi(tokens[0].c_str());
	this->mapHeight = atoi(tokens[1].c_str());
	this->offset_y= atoi(tokens[2].c_str());
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	HUD = ScoreBoard::GetInstance();

	ifstream f;
	f.open(sceneFilePath);
	
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines

		if (line == "[PLAYER]") {
			section = SCENE_SECTION_PLAYER; continue;}
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue;
			}		
		if (line == "[MAP_INFO]") {
			section = SCENE_SECTION_MAP_INFO; continue;
			}
		if (line == "[TILE_MAP]") {
			section = SCENE_SECTION_TILE_MAP; continue;
			}
		if (line[0] == '[') { 
			section = SCENE_SECTION_UNKNOWN; continue; 
		}	

		//
		// data section
		//
		switch (section)
		{
			case SCENE_SECTION_PLAYER:_ParseSection_PLAYER(line); break;
			case SCENE_SECTION_MAP_INFO: _ParseSection_MAP_INFO(line); break;
			case SCENE_SECTION_TILE_MAP:_ParseSection_TILE_MAP(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"resources\\bbox.png", D3DCOLOR_XRGB(0, 0, 0));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}


void CPlayScene::Update(DWORD dt)
{	
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Update(dt, &coObjects);
	}

	if (player == NULL) return;
	player->Update(dt, &coObjects);

	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy);	
	CGame* game = CGame::GetInstance();

	if (cx >= SCREEN_WIDTH/ 2)
	{
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;

		if (cx > mapWidth - SCREEN_WIDTH )
		{
			cx = mapWidth - SCREEN_WIDTH;
		}
	}
	else if (cx < SCREEN_WIDTH / 2)
	{
		cx = 0.0f;
	}
	
	game ->SetCamPos(cx, 0.0f /*cy*/);
	HUD->Update(dt);
}

void CPlayScene::Render()
{
	// Render map
	for (int i = 0; i < tiledMap.size(); i++) 
	{
		tiledMap[i]->Render();
	}
	for (int i = objects.size()-1; i >=0;i--) // Simon is rendered at the last 
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Render();
	}
	player->Render();
	HUD->Render();
}

void CPlayScene::_ParseSection_TILE_MAP(string line)
{
	D3DSURFACE_DESC surfaceDesc;
	int cureneMapId = (CGame::GetInstance()->GetSceneId());
	LPDIRECT3DTEXTURE9 tilesheet = CTextures::GetInstance()->Get(cureneMapId);
	int level = 0;
	tilesheet->GetLevelDesc(level, &surfaceDesc);

	//int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / TILE_WIDTH;
	
	vector<string> map_tokens = split(line);
	
	for (int i = 0; i < map_tokens.size(); i++)
	{
		RECT rectTile;
		int index = atoi(map_tokens[i].c_str());
		rectTile.left = (index % nums_colToRead) * TILE_WIDTH;
		rectTile.top = (index / nums_colToRead) * TILE_HEIGHT;
		rectTile.right = rectTile.left + TILE_WIDTH;
		rectTile.bottom = rectTile.top + TILE_HEIGHT;
		int x, y;
		x = i * TILE_WIDTH;
		y = this->offset_y;
		CTile* tile = new CTile(x, y, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
		tiledMap.push_back(tile);
	}
	this->offset_y += TILE_HEIGHT;
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{		
		if (dynamic_cast<CSimon*>(objects[i]) || 
			dynamic_cast<CWhip*>(objects[i]) || 
			dynamic_cast<CDagger*>(objects[i]) ||
			dynamic_cast<Boomerang_Weapons*>(objects[i]))
		{
			;
		}
		else
		{
			delete objects[i];
		} 
	}

	objects.clear();
	CItems::GetInstance()->Clear();
	player = NULL;
	tiledMap.clear();
}


void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"KeyDown: %d\n", KeyCode);

	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();
	CDagger *dagger = ((CPlayScene*)scence)->GetDagger();
	Boomerang_Weapons *boomerang = ((CPlayScene*)scence)->GetBoomerang();
	switch (KeyCode)
	{	
	case DIK_SPACE:
	{
		if (simon->GetState() == SIMON_STATE_JUMP ||
			simon->GetState() == SIMON_STATE_ATTACK || 
			simon->GetState() == SIMON_STATE_SIT_ATTACK ||
			simon->onStairs !=0)
			return;

		simon->SetState(SIMON_STATE_JUMP);
		break;
	}		
	case DIK_S: // Attack
	{
		if ((simon->GetState() == SIMON_STATE_ATTACK ||simon->GetState() == SIMON_STATE_SIT_ATTACK)) return;
		if (simon->GetState() == SIMON_STATE_IDLE ||	simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_GO_UPSTAIR || simon->GetState() == SIMON_ANI_GO_DOWNSTAIR)
		{
			simon->SetState(SIMON_STATE_ATTACK);
		}
		else if (simon->GetState() == SIMON_STATE_SIT)
		{
			simon->SetState(SIMON_STATE_SIT_ATTACK);
		}

		if (CGame::GetInstance()->IsKeyDown(DIK_UP) ) // Sub weapon attack 
		{
			float xS, yS;
			simon->GetPosition(xS, yS);
			dagger->SetPosition(xS, yS);
			dagger->SetOrientation(simon->nx);
			dagger->SetVisible(true);
			simon->SetState(SIMON_STATE_THROW);
		}

		break;
	}	
	case DIK_D:
	{
		if (simon->subWeapon == false)
			return;
		if (simon->GetState() == SIMON_STATE_THROW && boomerang->visible == true) return;
		float xS, yS;
		simon->GetPosition(xS, yS);
		boomerang->SetPosition(xS, yS);
		boomerang->SetOrientation(simon->nx);
		boomerang->SetVisible(true);
		simon->SetState(SIMON_STATE_THROW);
		break;
	}
	case DIK_Q: // Upgrade whip
	{
		simon->whip->PowerUp();
		break;
	}

	case DIK_1:
		CGame::GetInstance()->SwitchScene(1);
		break;

	case DIK_2:
		CGame::GetInstance()->SwitchScene(2);
		break;

	case DIK_3:
		CGame::GetInstance()->SwitchScene(3);
		break;
	case DIK_4:
		CGame::GetInstance()->SwitchScene(4);
		break;

	case DIK_5:
		CGame::GetInstance()->SwitchScene(5);
		break;

	case DIK_6:
		CGame::GetInstance()->SwitchScene(6);
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{}


void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();

	// When Simon is not touched on the ground, continue rendering jump animation
	if (simon->GetState() == SIMON_STATE_JUMP && simon->isOnGround() == false)		
		return;

	// Condition to stopping Simon's attacking loop
	if (simon->GetState()== SIMON_STATE_ATTACK &&
		simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	
	if (simon->GetState() == SIMON_STATE_SIT_ATTACK && 
		simon->animation_set->at(SIMON_ANI_SIT_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_THROW &&
		simon->animation_set->at(SIMON_ANI_THROW)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_ATTACK &&
		simon->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_ATTACK &&
		simon->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_DEFLECT && 
		simon->animation_set->at(SIMON_ANI_DEFLECT)->IsOver(SIMON_DEFLECT_TIME) == false)
		return ;
	
	if (simon->powerUp == true) return;
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->onStairs == 0)
		{
			simon->SetOrientation(1);
			simon->SetState(SIMON_STATE_WALKING);
		}
		else
		{
			simon->SetOrientation(1);
			simon->SetState(SIMON_STATE_GO_UPSTAIR);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->onStairs == 0)
		{
			simon->SetOrientation(-1);
			simon->SetState(SIMON_STATE_WALKING);
		}
		else
		{
			simon->SetState(SIMON_STATE_GO_DOWNSTAIR);						
		}
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->onStairs == 0 && simon->ovObjects.size() == 0 )
			simon->SetState(SIMON_STATE_SIT);
		else
			simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
	}
	else if (game->IsKeyDown(DIK_UP))
		simon->SetState(SIMON_STATE_GO_UPSTAIR);
	else
		simon->SetState(SIMON_STATE_IDLE);
}