#pragma once
#include "PlayScence.h"
#include "Scence.h"
#include <iostream>
#include <string>
#include <fstream>

#include "GameOverMenu.h"

#define PATH	-1

using namespace std;

class WorldMapScene : public CScene
{
protected:
	CMario* player;
	
	vector<LPGAMEOBJECT> objects;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(const string& line);
	void _ParseSection_Board(const string& line);
	void _ParseSection_Path(string& line);

public:

	GameOverMenu* gameOverMenu = NULL;
	int marioLevel = 0;

	WorldMapScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void SetMarioLevel(int val);

	CMario* GetPlayer() { return player; }

	vector<vector<char>> path;

	int currCellX = 0;
	int currCellY = 0;
	int startX = 0;
	int startY = 0;
	int scene = -1;
};

class WorldMapSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	WorldMapSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

