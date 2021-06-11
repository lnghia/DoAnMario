#include "Map.h"
#include <fstream>
#include "Textures.h"
#include "Utils.h"
#include <string>
#include "GameObject.h"

#include <cmath>

Map* Map::instance = NULL;

void Map::mapInfoReader(string& _mapInfoPath) {
	ifstream reader(_mapInfoPath);
	string line = "";

	while (getline(reader, line)) {
		//vector<int> tmp = stringToVectorOfInt(line, " ");
		map.push_back(stringToVectorOfInt(line, " "));
	}

	reader.close();
}

Map* Map::getInstance()
{
	//string tmp = ;

	if (!instance) {
		//instance = new Map("maps\\map_indexes_man1.txt", 656, 2816, 16, 16, 400);
		instance = new Map();
	}

	return instance;
}

Map::Map()
{
}

Map::Map(LPCWSTR _tileSetPath, string _matrixIdsPath, string _mapInfoPath, char _seperatorOfMatrixIds, D3DCOLOR _transcolor)
{

	//CTextures::GetInstance()->Add()
}

Map::Map(string _matrixIdsPath, int mapHeight, int mapWidth, int tileHeight, int tileWidth, int textureId)
{
	tileSetTexture = CTextures::GetInstance()->Get(textureId);
	height = mapHeight;
	width = mapWidth;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;

	tilesPerRowInTileSet = 16;
	tilesPerColumnInTileSet = 8;

	mapInfoReader(_matrixIdsPath);
}

int Map::getTileSize()
{
	return 0;
}

int Map::getWidth() {
	return width;
}

int Map::getHeight()
{
	return height;
}

void Map::Update(int _dt)
{
}

vector<int> Map::GetTilesOnCam()
{
	CGame* instance = CGame::GetInstance();

	int w = instance->GetScreenWidth();

	int left = (instance->GetCamX() / (float)tileWidth);
	int right = ((instance->GetCamX() + instance->GetScreenWidth()) / (float)tileWidth);
	int top = (instance->GetCamY() / (float)tileHeight);
	int bottom = ((instance->GetCamY() + instance->GetScreenHeight()) / (float)tileHeight);

	return vector<int>{ left, right, top, bottom };
	//return vector<int>{0, 175, 0, 26};
}

void Map::RenderBoundingBox(int x, int y)
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l = x, t = y, r = l + tileWidth, b = t + tileHeight;

	//GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 100);
}

void Map::Draw()
{
	vector<int> tilesOnCam = GetTilesOnCam();

	/*for (int row = 0; row <= map.size(); ++row) {
		for (int col = 0; col <= map[row].size(); ++col) {
			int _r = map[row][col] / tilesPerRowInTileSet;
			int _c = map[row][col] % tilesPerRowInTileSet;

			int left = _c * tileWidth + _c * spaceBetweenTiles;
			int right = left + tileWidth;
			int top = _r * tileHeight + _r * spaceBetweenTiles;
			int bottom = top + tileHeight;

			CGame::GetInstance()->Draw(col * tileHeight, row * tileWidth, tileSetTexture, left, top, right, bottom);
		}
	}*/

	//if(tilesOnCam[2]<0 || tilesOnCam[2]>=map.size())

	for (int row = tilesOnCam[2]; row <= tilesOnCam[3]; ++row) {
		if (row >= map.size()) {
			break;
		}
		for (int col = tilesOnCam[0]; col <= tilesOnCam[1]; ++col) {
			if (col >= map[row].size()) {
				break;
			}

			int tmp = map[row][col];

			int _r = map[row][col] / tilesPerRowInTileSet;
			int _c = map[row][col] % tilesPerRowInTileSet;

			int left = _c * tileWidth + _c * spaceBetweenTiles;
			int right = left + tileWidth;
			int top = _r * tileHeight + _r * spaceBetweenTiles;
			int bottom = top + tileHeight;

			CGame::GetInstance()->Draw(col * tileHeight, row * tileWidth, tileSetTexture, left, top, right, bottom);

			//RenderBoundingBox(col * tileHeight, row * tileWidth);
		}
	}
}

void Map::loadFromFile(string& _matrixIdsPath, int mapHeight, int mapWidth, int tileHeight, int tileWidth, int textureId, int tilesPerRow, int tilesPerColumn)
{
	tileSetTexture = CTextures::GetInstance()->Get(textureId);
	height = mapHeight;
	width = mapWidth;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;

	tilesPerRowInTileSet = tilesPerRow;
	tilesPerColumnInTileSet = tilesPerColumn;

	mapInfoReader(_matrixIdsPath);
}

void Map::unLoad()
{
	for(auto& row : map){
		row.clear();
	}
	map.clear();
}
