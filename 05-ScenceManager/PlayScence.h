#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "JustForShow.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "PiranhaPlant.h"
#include "InteractivableTransObject.h"
#include "PipeHitBox.h"
#include "ColorBrickHitBox.h"
#include "QBrick.h"
#include "Mushroom.h"
#include "Ground.h"
#include "FloatingCoin.h"
#include "Board.h"
#include "RedKoopas.h"
#include "CourseClearBoard.h"


class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 

	CourseClearBoard* courseBoard = NULL;

	vector<LPGAMEOBJECT> objects;
	

	UINT objId;

	void _ParseSection_TEXTURES(const string& line);
	void _ParseSection_SPRITES(const string& line);
	void _ParseSection_ANIMATIONS(const string& line);
	void _ParseSection_ANIMATION_SETS(const string& line);
	void _ParseSection_OBJECTS(const string& line);
	void _ParseSection_OBJECTS(const string& line, ofstream& writer, ofstream& writer2);
	void _ParseSection_GRID(const string& line);
	void _ParseSection_MAP(const string& line);
	void _ParseSection_Board(const string& line);

public:
	unordered_map<int, LPGAMEOBJECT> objs_with_id;
	vector<LPGAMEOBJECT> portalPipeImages;

	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	void handleCollisionsWithEnemiesAABB(vector<LPGAMEOBJECT>& collidable_objs);
	void handleCollisionsWithItemsAABB(vector<LPGAMEOBJECT>& collidable_objs);

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
	//CPlayScenceKeyHandler(CScene* s, CMario* player) :CScenceKeyHandler(s) { this->player = player; }
};

