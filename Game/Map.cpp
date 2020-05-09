#include "Map.h"
#include "tilemap.h"
#include "Utils.h"

CMaps *CMaps::_instance = NULL;

CMap::CMap(int mapId, LPCWSTR texturePath, LPCWSTR dataPath, int map_width, int map_height)
{
	// Set gia tri
	this->map_Width = map_width;
	this->map_Height = map_height;
	this->mapId = mapId;
	this->texturePath = texturePath;
	this->dataPath = dataPath;

	nums_row = map_Height / TILE_HEIGHT;
	nums_col = map_Width / TILE_WIDTH;

	LoadResources();
	LoadMap();
}

void CMap::LoadResources()
{
	CTextures *texture = CTextures::GetInstance();
	texture->Add(mapId, texturePath, TILEMAP_TRANSPARENT_COLOR);
	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(mapId);
	// get size of texture to save block tiles
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);
	// calculate num of cols and rows
	int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / TILE_WIDTH;
	// saving tile list to sprires and sort by id
	int id_sprite = 1;
	for (UINT i = 0; i < nums_rowToRead; i++)
	{
		for (UINT j = 0; j < nums_colToRead; j++)
		{
			int idTile = mapId * 10 + id_sprite;
			sprites->Add(idTile, TILE_WIDTH * j, TILE_HEIGHT * i, TILE_WIDTH * (j + 1), TILE_HEIGHT * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}
}

void CMap::LoadMap()
{
	fstream fs;
	fs.open(dataPath, ios::in);
	if (fs.fail())
	{
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);
		// save sprite
		vector<LPSPRITE> spriteline;
		stringstream ss(line);
		int n;
		while (ss >> n)
		{
			int idTile = mapId * 1000 + n;
			spriteline.push_back(sprites->Get(idTile));
		}
		tilemap.push_back(spriteline);
	}

	fs.close();
}



void CMaps::Add(LPCWSTR dataPath, LPCWSTR texturePath, int ID)
{
	LPTILEMAP tilemap = new CMap(ID, texturePath, dataPath, MAP_1_WITDH, MAP_1_HEIGHT);
	tilemaps[ID] = tilemap;
}

void CMap::Draw(D3DXVECTOR2 camPosition)
{
	int startCol = (int)camPosition.x / 32;
	int endCol = startCol + SCREEN_WIDTH / 32;

	for (UINT i = 0; i < nums_row; i++)
	{
		for (UINT j = startCol; j <= endCol; j++)
		{
			float x = TILE_WIDTH * (j - startCol) + camPosition.x - (int)camPosition.x % 32;
			float y = TILE_HEIGHT * i + 80;
			tilemap[i][j]->Draw(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

CMaps *CMaps::GetInstance()
{
	if (_instance == NULL)
		_instance = new CMaps();
	return _instance;
}