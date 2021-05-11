#pragma once
#pragma once
#include <d3d9.h>
#include <iostream>
#include <vector>
#include "Textures.h"
#include "Game.h"

using namespace std;

class Map
{
private:
	static Map* instance;
	LPDIRECT3DTEXTURE9 tileSetTexture;
	vector<vector<int>> map;
	int tileWidth, tileHeight, spaceBetweenTiles = 1, tilesPerRowInTileSet, tilesPerColumnInTileSet;
	int width, height;
public:
	Map();
	Map(LPCWSTR _tileSetPath, string _matrixIdsPath, string _mapInfoPath, char _seperatorOfMatrixIds, D3DCOLOR _transcolor);
	Map(string _matrixIdsPath, int mapHeight, int mapWidth, int tileHeight, int tileWidth, int textureId);
	//~Map();

	// Getter
	int getTileSize();
	int getWidth();
	int getHeight();

	void Update(int _dt);
	vector<int> GetTilesOnCam();
	void Draw();
	void loadFromFile(string& _matrixIdsPath, int mapHeight, int mapWidth, int tileHeight, int tileWidth, int textureId, int tilesPerRow, int tilesPerColumn);

	void unLoad();

	void mapInfoReader(string& _mapInfoPath);

	void RenderBoundingBox(int x, int y);

	static Map* getInstance();
};



