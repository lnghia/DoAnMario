#include <iostream>
#include <fstream>
#include <algorithm>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Map.h"
#include "Grid.h"
#include "Wood.h"
#include "BrokenBrick.h"
#include "BrokenQuestionBrick.h"
#include "NoteBrick.h"
#include "EndGameBrick.h"
#include "BoomerangGuy.h"
#include "PiranhaFlower.h"
#include "PortalPipe.h"
#include "GreenMushroom.h"


#include "ObjectCheatSheet.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CPlayScenceKeyHandler(this);
	objId = 0;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/


void CPlayScene::_ParseSection_TEXTURES(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < (int)tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	if (atoi(tokens[0].c_str()) == 35) {
		int tmp = 1;
	}

	for (int i = 1; i < (int)tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(const string& line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y

	int id = (int)atoi(tokens[0].c_str());
	int object_type = atoi(tokens[1].c_str());
	float x = (float)atof(tokens[2].c_str());
	float y = (float)atof(tokens[3].c_str());

	int ani_set_id = atoi(tokens[4].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: {
		int level = (int)atoi(tokens[5].c_str());

		obj = new CGoomba(level);

		break;
	}
	case OBJECT_TYPE_QBRICK: {
		int hiddenItemType = atoi(tokens[5].c_str());
		int	hiddenItemAni = atoi(tokens[6].c_str());

		if (tokens.size() > 7) {
			int backupItem = atoi(tokens[7].c_str());
			int backupItemAni = atoi(tokens[8].c_str());

			obj = new QBrick(hiddenItemType, hiddenItemAni, backupItem, backupItemAni);

			break;
		}

		obj = new QBrick(hiddenItemType, hiddenItemAni);

		break;
	}
	case OBJECT_TYPE_BRICK:
		obj = new CBrick();
		break;
	case OBJECT_TYPE_KOOPAS: {
		short int initMovingDirec = (short int)atoi(tokens[5].c_str());
		int level = (int)atoi(tokens[6].c_str());

		obj = new CKoopas(initMovingDirec, level);

		break;
	}
	case OBJECT_TYPE_RED_KOOPAS:
	{
		short int initMovingDirec = (short int)atoi(tokens[5].c_str());
		int x = atoi(tokens[6].c_str());
		int y = atoi(tokens[7].c_str());
		int h = atoi(tokens[8].c_str());
		int w = atoi(tokens[9].c_str());

		obj = new RedKoopas(initMovingDirec, x, y, w, h);

		break;
	}
	case OBJECT_TYPE_PORTAL_PIPE: {
		float pipeWidth = (float)atof(tokens[5].c_str());
		float pipeHeight = (float)atof(tokens[6].c_str());
		int sceneId = (int)atoi(tokens[7].c_str());
		int movingDir = (int)atoi(tokens[8].c_str());
		float exitX = (float)atof(tokens[9].c_str());
		float exitY = (float)atof(tokens[10].c_str());
		int exitWidth = (int)atoi(tokens[11].c_str());
		int exitHeight = (int)atoi(tokens[12].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Piranha Plant");
		}

		obj = new PortalPipe((int)pipeWidth, (int)pipeHeight);
		obj->SetSceneId(sceneId);
		obj->SetGetOutPipeDirection(movingDir);
		obj->SetExitPoint(exitX, exitY, exitWidth, exitHeight);
		
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[5].c_str());
		float b = (float)atof(tokens[6].c_str());
		int scene_id = atoi(tokens[7].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_JUST_FOR_SHOW: {
		int renderPriority;

		if (tokens.size() > 5) {
			renderPriority = (int)atoi(tokens[5].c_str());
		}
		else {
			renderPriority = 30;
		}

		obj = new JustForShow();
		obj->SetRenderPriority(renderPriority);
		break;
	}

	case OBJECT_TYPE_INVISIBLE: {
		int width = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());

		obj = new InteractivableTransObject(width, height);

		break;
	}
	case OBJECT_TYPE_PIPE_HITBOX: {
		float width = (float)atof(tokens[5].c_str());
		float height = (float)atof(tokens[6].c_str());

		obj = new PipeHitBox((int)width, (int)height);

		break;
	}
	case OBJECT_TYPE_PIPE_FOR_SHOW: {
		obj = new JustForShow();

		obj->SetRenderPriority(99);

		break;
	}
	case OBJECT_TYPE_COLOR_BRICK_HITBOX: {
		int width = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());

		obj = new ColorBrickHitBox(width, height);

		break;
	}
	case OBJECT_TYPE_GROUND: {
		int width = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());

		obj = new Ground(width, height);

		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT: {
		float pipeX = (float)atof(tokens[5].c_str());
		float pipeY = (float)atof(tokens[6].c_str());
		float pipeWidth = (float)atof(tokens[7].c_str());
		float pipeHeight = (float)atof(tokens[8].c_str());
		int level = (int)atoi(tokens[9].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Piranha Plant");
		}

		obj = new PiranhaPlant(pipeX, pipeY, pipeWidth, pipeHeight, player, level);
		break;
	}
	case OBJECT_TYPE_WOOD: {
		obj = new Wood();
		break;
	}
	case OBJECT_TYPE_FLOATING_COIN: {
		obj = new FloatingCoin(x, y);
		break;
	}
	case OBJECT_TYPE_BROKEN_BRICK: {
		int hiddenItemType = (int)atoi(tokens[5].c_str());

		obj = new BrokenBrick(hiddenItemType);
		break;
	}
	case OBJECT_TYPE_QUESTION_BROKEN_BRICK: {
		obj = new BrokenQuestionBrick(x, y);

		int item;
		int ani;

		for (UINT i = 4; i < tokens.size(); i += 2) {
			item = (int)atoi(tokens[i].c_str());
			ani = (int)atoi(tokens[i + 1].c_str());

			obj->AddHiddenItem(item, ani);
		}

		break;
	}
	case OBJECT_TYPE_NOTE_BRICK: {
		obj = new NoteBrick(x, y);

		break;
	}
	case OBJECT_TYPE_ENDGAME_BRICK: {
		obj = new EndGameBrick();

		break;
	}
	case OBJECT_TYPE_BOOMERANG_GUY: {
		float xMax = (float)atof(tokens[5].c_str());
		float xMin = (float)atof(tokens[6].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Boomerang Guy");
		}

		obj = new BoomerangGuy(xMax, xMin, player);

		break;
	}
	case OBJECT_TYPE_PIRANHA_FLOWER: {
		float pipeX = (float)atof(tokens[5].c_str());
		float pipeY = (float)atof(tokens[6].c_str());
		float pipeWidth = (float)atof(tokens[7].c_str());
		float pipeHeight = (float)atof(tokens[8].c_str());
		int level = (int)atoi(tokens[9].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Piranha Plant");
		}

		obj = new PiranhaFlower(pipeX, pipeY, pipeWidth, pipeHeight, player, level);
		break;
	}
	case OBJECT_TYPE_COURSE_BOARD: {
		obj = new CourseClearBoard();
		courseBoard = (CourseClearBoard*)obj;

		break;
	}
	case OBJECT_TYPE_PORTAL_PIPE_IMAGE: {
		obj = new JustForShow();

		portalPipeImages.push_back(obj);
		
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
	//objects.push_back(obj);
	//Grid::GetInstance()->putObjectIntoGrid(obj);
	objs_with_id.insert({ id, obj });
	obj->id = id;
}

void CPlayScene::_ParseSection_OBJECTS(const string& line, ofstream& writer, ofstream& writer2)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: {
		int level = (int)atoi(tokens[4].c_str());

		obj = new CGoomba(level);

		break;
	}
	case OBJECT_TYPE_QBRICK: {
		int hiddenItemType = atoi(tokens[4].c_str());
		int	hiddenItemAni = atoi(tokens[5].c_str());

		if (tokens.size() > 6) {
			int backupItem = atoi(tokens[6].c_str());
			int backupItemAni = atoi(tokens[7].c_str());

			obj = new QBrick(hiddenItemType, hiddenItemAni, backupItem, backupItemAni);

			break;
		}

		obj = new QBrick(hiddenItemType, hiddenItemAni);

		break;
	}
	case OBJECT_TYPE_BRICK:
		obj = new CBrick();
		break;
	case OBJECT_TYPE_KOOPAS: {
		short int initMovingDirec = (short int)atoi(tokens[4].c_str());
		int level = (int)atoi(tokens[5].c_str());

		obj = new CKoopas(initMovingDirec, level);

		break;
	}
	case OBJECT_TYPE_RED_KOOPAS:
	{
		short int initMovingDirec = (short int)atoi(tokens[4].c_str());
		int x = atoi(tokens[5].c_str());
		int y = atoi(tokens[6].c_str());
		int h = atoi(tokens[7].c_str());
		int w = atoi(tokens[8].c_str());

		obj = new RedKoopas(initMovingDirec, x, y, w, h);

		break;
	}
	case OBJECT_TYPE_PORTAL_PIPE: {
		float pipeWidth = (float)atof(tokens[4].c_str());
		float pipeHeight = (float)atof(tokens[5].c_str());
		int sceneId = (int)atoi(tokens[6].c_str());
		int movingDir = (int)atoi(tokens[7].c_str());
		float exitX = (float)atof(tokens[8].c_str());
		float exitY = (float)atof(tokens[9].c_str());
		int exitWidth = (int)atoi(tokens[10].c_str());
		int exitHeight = (int)atoi(tokens[11].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Piranha Plant");
		}

		obj = new PortalPipe((int)pipeWidth, (int)pipeHeight);
		obj->SetSceneId(sceneId);
		obj->SetGetOutPipeDirection(movingDir);
		obj->SetExitPoint(exitX, exitY, exitWidth, exitHeight);

		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[4].c_str());
		float b = (float)atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_JUST_FOR_SHOW: {
		obj = new JustForShow();
		obj->SetRenderPriority(30);
		break;
	}

	case OBJECT_TYPE_INVISIBLE: {
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());

		obj = new InteractivableTransObject(width, height);

		break;
	}
	case OBJECT_TYPE_PIPE_HITBOX: {
		float width = (float)atof(tokens[4].c_str());
		float height = (float)atof(tokens[5].c_str());

		obj = new PipeHitBox((int)width, (int)height);

		break;
	}
	case OBJECT_TYPE_PIPE_FOR_SHOW: {
		obj = new JustForShow();

		obj->SetRenderPriority(99);

		break;
	}
	case OBJECT_TYPE_COLOR_BRICK_HITBOX: {
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());

		obj = new ColorBrickHitBox(width, height);

		break;
	}
	case OBJECT_TYPE_GROUND: {
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());

		obj = new Ground(width, height);

		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT: {
		float pipeX = (float)atof(tokens[4].c_str());
		float pipeY = (float)atof(tokens[5].c_str());
		float pipeWidth = (float)atof(tokens[6].c_str());
		float pipeHeight = (float)atof(tokens[7].c_str());
		int level = (int)atoi(tokens[8].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Piranha Plant");
		}

		obj = new PiranhaPlant(pipeX, pipeY, pipeWidth, pipeHeight, player, level);
		break;
	}
	case OBJECT_TYPE_WOOD: {
		obj = new Wood();
		break;
	}
	case OBJECT_TYPE_FLOATING_COIN: {
		obj = new FloatingCoin(x, y);
		break;
	}
	case OBJECT_TYPE_BROKEN_BRICK: {
		int hiddenItemType = (int)atoi(tokens[4].c_str());

		obj = new BrokenBrick(hiddenItemType);
		break;
	}
	case OBJECT_TYPE_QUESTION_BROKEN_BRICK: {
		obj = new BrokenQuestionBrick(x, y);

		int item;
		int ani;

		for (UINT i = 4; i < tokens.size(); i += 2) {
			item = (int)atoi(tokens[i].c_str());
			ani = (int)atoi(tokens[i + 1].c_str());

			obj->AddHiddenItem(item, ani);
		}

		break;
	}
	case OBJECT_TYPE_NOTE_BRICK: {
		obj = new NoteBrick(x, y);

		break;
	}
	case OBJECT_TYPE_ENDGAME_BRICK: {
		obj = new EndGameBrick();

		break;
	}
	case OBJECT_TYPE_BOOMERANG_GUY: {
		float xMax = (float)atof(tokens[4].c_str());
		float xMin = (float)atof(tokens[5].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Boomerang Guy");
		}

		obj = new BoomerangGuy(xMax, xMin, player);

		break;
	}
	case OBJECT_TYPE_PIRANHA_FLOWER: {
		float pipeX = (float)atof(tokens[4].c_str());
		float pipeY = (float)atof(tokens[5].c_str());
		float pipeWidth = (float)atof(tokens[6].c_str());
		float pipeHeight = (float)atof(tokens[7].c_str());
		int level = (int)atoi(tokens[8].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Piranha Plant");
		}

		obj = new PiranhaFlower(pipeX, pipeY, pipeWidth, pipeHeight, player, level);
		break;
	}
	case OBJECT_TYPE_COURSE_BOARD: {
		obj = new CourseClearBoard();
		courseBoard = (CourseClearBoard*)obj;

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
	//objects.push_back(obj);
	Grid::GetInstance()->putObjectIntoGrid(obj);

	vector<int> cords = Grid::GetInstance()->getOverLapCells(obj);

	string out_line = to_string(objId);
	string out_line2 = to_string(objId) + "\t" + line;

	++objId;

	//DebugOut(L"%d\n", writer.is_open());

	for (auto& cord : cords) {
		out_line += "\t" + to_string(cord);
	}

	writer << out_line << '\n';
	writer2 << out_line2 << '\n';
}

void CPlayScene::_ParseSection_GRID(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	// 1: cellWidth, 2: cellHeight
	Grid::GetInstance()->unload();
	Grid::GetInstance()->load(stoi(tokens[0]), stoi(tokens[1]));
	Grid::GetInstance()->loadFromFile(tokens[2], objs_with_id);
}

void CPlayScene::_ParseSection_MAP(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 8) return;

	Map::getInstance()->unLoad();
	Map::getInstance()->loadFromFile(tokens[0],
		stoi(tokens[1]),
		stoi(tokens[2]),
		stoi(tokens[3]),
		stoi(tokens[4]),
		stoi(tokens[5]),
		stoi(tokens[6]),
		stoi(tokens[7]));

	CGame* game = CGame::GetInstance();

	if (Map::getInstance()->getHeight() < game->GetScreenHeight()) {
		game->SetCamPos(0.0f, 0.0f);
	}
	else {
		game->SetCamPos(0.0f, (float)(Map::getInstance()->getHeight() - game->GetScreenHeight() - 1));
	}

	DebugOut(L"[INFO] Done loading map resources\n");
}

void CPlayScene::_ParseSection_Board(const string& line)
{
	/*if (Board::GetInstance()) {
		return;
	}*/

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	string obj = tokens[0];
	Board* board = Board::GetInstance();

	board->RefreshPos();

	if (obj == "CARDSTACK") {
		board->GetCardStack()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "POINT") {
		board->GetPoint()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "MONEY") {
		board->GetMoney()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "TIME") {
		board->GetTime()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "LIVES") {
		board->GetLives()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "WORLDNUM") {
		board->GetWorldNum()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "WORLDFIELD") {
		board->GetWorldField()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "PLAYERCHAR") {
		board->GetPlayerChar()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "CLOCK") {
		board->GetClock()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "DOLLARSIGN") {
		board->GetDollarSign()->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "BOARD") {
		board->SetAniSet(atoi(tokens[1].c_str()));
	}
	else if (obj == "SPEEDBAR") {
		board->GetSpeedBar()->SetAniSet(atoi(tokens[1].c_str()));
		board->GetSpeedBar()->SetPlayer(player);
	}

	if (timeLeft) {
		board->GetTime()->SetTime(timeLeft);
	}

	DebugOut(L"[INFO] Done loading board resources \n");
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	//ofstream w1, w2;

	f.open(sceneFilePath);
	/*w1.open(L"gen\\grid_extra_1_1.txt");
	w2.open(L"gen\\extra_1_1_objs.txt");*/

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = MAP; continue;
		}
		if (line == "[GRID]") {
			section = GRID; continue;
		}
		if (line == "[BOARD]") {
			section = BOARD; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line/*, w1, w2*/); break;
		case MAP: _ParseSection_MAP(line); break;
		case GRID: _ParseSection_GRID(line); break;
		case BOARD: _ParseSection_Board(line); break;
		}
	}

	f.close();
	/*w1.close();
	w2.close();*/

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	Board::GetInstance()->GetWorldNum()->SetContent((int)worldNum);

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	if (!Board::GetInstance()->GetTime()->GetIsTicking()) {
		Board::GetInstance()->GetTime()->StartTicking();
	}

	CGame* game = CGame::GetInstance();

	vector<LPGAMEOBJECT> coObjects;
	/*for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}*/

	vector<LPGAMEOBJECT> objectsInCamera = Grid::GetInstance()->GetObjectsInCamera();

	if (player->GetState() == MARIO_STATE_DIE) {
		//coObjects = Grid::GetInstance()->GetPotentialCollidableObjects(player);
		player->Update(dt, &coObjects);

		return;
	}

	if (Board::GetInstance()->GetTime()->GetIsTicking() && Board::GetInstance()->GetTime()->GetCurrMoment() <= 0) {
		player->SetState(MARIO_STATE_DIE);
		player->Update(dt);

		return;
	}

	if ((player->GetUntouchable() && (int)(((DWORD)GetTickCount64() - player->GetUntouchableStart())) < player->transform_duration_time) ||
		(player->GetTransforming() && (int)(((DWORD)GetTickCount64() - player->GetStartTransforming())) < player->transform_duration_time)) {

		return;
	}

	if (player->toExtraScene || player->gettingOutPipe) {
		player->Update(dt);
		Board::GetInstance()->Update(dt);

		return;
	}

	coObjects.clear();
	coObjects = Grid::GetInstance()->GetPotentialCollidableObjects(player);

	if (player->GetMadeItToNextScene()) {
		courseBoard->SetInvisible(0);
		courseBoard->SetCardType(Board::GetInstance()->GetLatestCardType());
	}

	player->Update(dt, &coObjects);

	if (!player) {
		return;
	}

	for (auto& obj : objectsInCamera) {
		if (dynamic_cast<CMario*>(obj) || !obj) {
			continue;
		}

		float x, y;

		obj->GetPosition(x, y);

		coObjects.clear();
		if (obj->GetInteractivable())
			coObjects = Grid::GetInstance()->GetPotentialCollidableObjects(obj);
		//coObjects = objectsInCamera;
		obj->Update(dt, &coObjects);
		/*if (dynamic_cast<Coin*>(obj)) {
			int tmp = 1;
		}*/
		if (!obj->GetIsActive()) {
			Grid::GetInstance()->clearObjFromGrid(obj);
			/*delete obj;
			obj = NULL;*/
		}

		/*if (player->GetTransforming()) {
			break;
		}*/
	}

	/*coObjects.clear();

	Grid::GetInstance()->GetPotentialCollidableObjects(player);

	player->Update(dt, &coObjects);*/

	vector<LPGAMEOBJECT> collidableObjsToPlayer = Grid::GetInstance()->GetPotentialCollidableObjects(player);

	handleCollisionsWithEnemiesAABB(collidableObjsToPlayer);
	handleCollisionsWithItemsAABB(collidableObjsToPlayer);

	collidableObjsToPlayer.clear();

	/*for (size_t i = 0; i < objectsInCamera.size(); i++)
	{
		coObjects.clear();
		coObjects = Grid::GetInstance()->GetPotentialCollidableObjects(objects[i]);
		objects[i]->Update(dt, &coObjects);
	}*/

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	float _cx, _cy;

	player->GetPosition(cx, cy);

	_cx = game->GetCamX();
	_cy = game->GetCamY();

	if (!player->GetTransforming() && !player->GetIsAttackingTail()) {
		cx -= game->GetScreenWidth() / 2;
	}
	else {
		cx = _cx;
	}

	if (player->GetIsFlying() || player->GetIsFalling() || player->GetIsGliding() || (player->GetIsFallingTail() && _cy + game->GetScreenHeight() < Map::getInstance()->getHeight()) || _cy + game->GetScreenHeight() < Map::getInstance()->getHeight()) {
		cy -= game->GetScreenHeight() / 2;
		if (_cy + game->GetScreenHeight() > Map::getInstance()->getHeight()) {
			cy = (float)(Map::getInstance()->getHeight() - game->GetScreenHeight() - 1);
		}
	}
	else {
		cy = _cy;
	}

	cx = (cx < 0) ? 0.0f : cx;
	cy = (cy < 0) ? 0.0f : cy;


	if (cx + game->GetScreenWidth() > Map::getInstance()->getWidth()) {
		cx = (float)(Map::getInstance()->getWidth() - game->GetScreenWidth() - 1);
	}

	//DebugOut(L"[CAM POS]: %f %f\n", cx, cy);

	CGame::GetInstance()->SetCamPos(round(cx), /*100.0f*/ round(cy));
	Board::GetInstance()->Update(dt);
	//CGame::GetInstance()->SetCamPos(cx, /*100.0f*/ cy);
}

void CPlayScene::Render()
{
	if (!player) {
		return;
	}

	Map::getInstance()->Draw();

	/*for (int i = 0; i < objects.size(); i++)
		if (objects[i] != player && !objects[i]->GetInvisible()) {
			objects[i]->Render();
		}*/

	vector<LPGAMEOBJECT> objectsInCamera = Grid::GetInstance()->GetObjectsInCamera();

	sort(objectsInCamera.begin(), objectsInCamera.end(), cmp);

	bool renderPause = ((player->GetUntouchable() && (int)((DWORD)GetTickCount64() - player->GetUntouchableStart()) < player->transform_duration_time) ||
		(player->GetTransforming() && (int)((DWORD)GetTickCount64() - player->GetStartTransforming()) < player->transform_duration_time) ||
		player->GetState() == MARIO_STATE_DIE || player->toExtraScene || player->gettingOutPipe);

	/*if (!renderPause) {
		player->SetState(MARIO_STATE_IDLE);
		player->SetLevel(player->GetBackupLevel());
	}*/

	for (auto& obj : objectsInCamera) {
		/*if (dynamic_cast<Point*>(obj)) {
			int tmp = 1;
		}*/
		if (obj != player && !obj->GetInvisible()) {
			if (renderPause) {
				obj->RenderCurrFrame();
				continue;
			}
			obj->Render();
		}
	}

	objectsInCamera.clear();

	Grid::GetInstance()->cleanObjTrashBin();

	/*for (auto& obj : objects) {
		if (obj != player && !obj->GetInvisible()) {
			obj->Render();
		}
	}*/

	if (renderPause && player->toExtraScene || player->gettingOutPipe) {
		player->Render();
	}
	else if (renderPause && player->GetState() != MARIO_STATE_DIE) {
		player->StartTransforming();
		//player->RenderSizeTransforming();
	}
	else {
		player->Render();
	}

	player->FinishTransforming();

	for (auto& item : portalPipeImages) {
		item->Render();
	}

	Board::GetInstance()->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	objs_with_id.clear();
	portalPipeImages.clear();
	player = NULL;
	courseBoard = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

//void CPlayScene::handleCollisionWithObjs(LPGAMEOBJECT obj, vector<LPGAMEOBJECT>& collidable_objs) {
//	for (auto& obj : collidable_objs) {
//		if (player->checkAABB(obj) && obj->GetIsActive()) {
//			if (dynamic_cast<PipeHitBox*>(obj)) {
//				PipeHitBox* pipe = dynamic_cast<PipeHitBox*>(obj);
//
//				vx = 0;
//			}
//		}
//	}
//}


void CPlayScene::handleCollisionsWithEnemiesAABB(vector<LPGAMEOBJECT>& collidable_objs)
{
	float ml, mt, mr, mb;

	player->GetBoundingBox(ml, mt, mr, mb);

	for (auto& obj : collidable_objs) {
		if (player->checkAABB(obj) && obj->GetIsActive()) {
			if (dynamic_cast<CGoomba*>(obj)) {
				CGoomba* goomba = dynamic_cast<CGoomba*>(obj);

				if (!player->GetUntouchable()) {
					if (player->GetLevel() == MARIO_LEVEL_BIG) {
						player->SetBackupLevel(MARIO_LEVEL_SMALL);
						player->SetBackupState(player->GetState());
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->turnIntoSmall();
						player->StartUntouchable();
					}
					else if (player->GetLevel() == MARIO_LEVEL_SMALL) {
						player->SetState(MARIO_STATE_DIE);
					}
					else if (player->GetLevel() == MARIO_LEVEL_FIRE) {
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->FireToBig();
						player->StartUntouchable();
					}
					else if (player->GetLevel() == MARIO_LEVEL_RACOON) {
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->RacoonToBig();
						player->StartUntouchable();
					}
				}
			}
			else if (dynamic_cast<FireBall*>(obj)) {
				if (!player->GetUntouchable()) {
					if (player->GetLevel() == MARIO_LEVEL_BIG) {
						player->SetBackupLevel(MARIO_LEVEL_SMALL);
						player->SetBackupState(player->GetState());
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->turnIntoSmall();
						player->StartUntouchable();
					}
					else if (player->GetLevel() == MARIO_LEVEL_SMALL) {
						player->SetState(MARIO_STATE_DIE);
					}
					else if (player->GetLevel() == MARIO_LEVEL_FIRE) {
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->FireToBig();
						player->StartUntouchable();
					}
					else if (player->GetLevel() == MARIO_LEVEL_RACOON) {
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->RacoonToBig();
						player->StartUntouchable();
					}
				}
			}
			else if (dynamic_cast<RedKoopas*>(obj)) {
				RedKoopas* koopas = dynamic_cast<RedKoopas*>(obj);
				int state = koopas->GetState();

				if ((player->GetBeingHoldedObj() && player->GetBeingHoldedObj() == obj)) {
					continue;
				}
				else if (player->hasJustKicked) {
					float _l, _t, _r, _b;
					float tmpX, tmpY;

					player->GetBoundingBox(_l, _t, _r, _b);
					koopas->GetPosition(tmpX, tmpY);
					if (player->GetNx() > 0) {
						koopas->SetPosition(_l - 1 - 16, tmpY);
					}
					else {
						koopas->SetPosition(_r + 1, tmpY);
					}
					player->hasJustKicked = 0;
				}
				else if (!player->GetUntouchable())
				{
					float tmpx, tmpy;

					koopas->GetPosition(tmpx, tmpy);

					if (!koopas->GetHarmless() && mb > tmpy)
					{
						if (player->GetLevel() == MARIO_LEVEL_BIG) {
							player->SetBackupLevel(MARIO_LEVEL_SMALL);
							player->SetBackupState(player->GetState());
							player->SetStartTransforming((DWORD)GetTickCount64());
							player->turnIntoSmall();
							player->StartUntouchable();
						}
						else if (player->GetLevel() == MARIO_LEVEL_SMALL) {
							player->SetState(MARIO_STATE_DIE);
						}
						else if (player->GetLevel() == MARIO_LEVEL_FIRE) {
							player->SetStartTransforming((DWORD)GetTickCount64());
							player->FireToBig();
							player->StartUntouchable();
						}
						else if (player->GetLevel() == MARIO_LEVEL_RACOON) {
							player->SetStartTransforming((DWORD)GetTickCount64());
							player->RacoonToBig();
							player->StartUntouchable();
						}
					}
					else {
						//kick
						int tmp = (player->GetVx() > 0) ? 1 : -1;
						koopas->GetKicked(tmp);
					}
				}

			}
			else if (dynamic_cast<CKoopas*>(obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(obj);
				int state = koopas->GetState();

				if ((player->GetBeingHoldedObj() && player->GetBeingHoldedObj() == obj)) {
					continue;
				}
				else if (player->hasJustKicked) {
					float _l, _t, _r, _b;
					float tmpX, tmpY;

					player->GetBoundingBox(_l, _t, _r, _b);
					koopas->GetPosition(tmpX, tmpY);
					if (player->GetNx() > 0) {
						koopas->SetPosition(_l - 1 - 16, tmpY);
					}
					else {
						koopas->SetPosition(_r + 1, tmpY);
					}
					player->hasJustKicked = 0;
				}
				else if (!player->GetUntouchable())
				{
					float tmpx, tmpy;

					koopas->GetPosition(tmpx, tmpy);

					if (!koopas->GetHarmless() && mb > tmpy)
					{
						if (player->GetLevel() == MARIO_LEVEL_BIG) {
							player->SetBackupLevel(MARIO_LEVEL_SMALL);
							player->SetBackupState(player->GetState());
							player->SetStartTransforming((DWORD)GetTickCount64());
							player->turnIntoSmall();
							player->StartUntouchable();
						}
						else if (player->GetLevel() == MARIO_LEVEL_SMALL) {
							player->SetState(MARIO_STATE_DIE);
						}
						else if (player->GetLevel() == MARIO_LEVEL_FIRE) {
							player->SetStartTransforming((DWORD)GetTickCount64());
							player->FireToBig();
							player->StartUntouchable();
						}
						else if (player->GetLevel() == MARIO_LEVEL_RACOON) {
							player->SetStartTransforming((DWORD)GetTickCount64());
							player->RacoonToBig();
							player->StartUntouchable();
						}
					}
					else {
						//kick
						int tmp = (player->GetVx() > 0) ? 1 : -1;
						koopas->GetKicked(tmp);
					}
				}
			}
			else if (dynamic_cast<Boomerang*>(obj)) {
				if (!player->GetUntouchable())
				{
					if (player->GetLevel() == MARIO_LEVEL_BIG) {
						player->SetBackupLevel(MARIO_LEVEL_SMALL);
						player->SetBackupState(player->GetState());
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->turnIntoSmall();
						player->StartUntouchable();
					}
					else if (player->GetLevel() == MARIO_LEVEL_SMALL) {
						player->SetState(MARIO_STATE_DIE);
					}
					else if (player->GetLevel() == MARIO_LEVEL_FIRE) {
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->FireToBig();
						player->StartUntouchable();
					}
					else if (player->GetLevel() == MARIO_LEVEL_RACOON) {
						player->SetStartTransforming((DWORD)GetTickCount64());
						player->RacoonToBig();
						player->StartUntouchable();
					}
				}
			}
		}
	}
}

void CPlayScene::handleCollisionsWithItemsAABB(vector<LPGAMEOBJECT>& collidable_objs)
{
	for (auto& obj : collidable_objs) {
		if (player->checkAABB(obj) && obj->GetIsActive()) {
			if (dynamic_cast<Mushroom*>(obj)) {
				Mushroom* mushroom = dynamic_cast<Mushroom*>(obj);
				float x, y;

				mushroom->GetPosition(x, y);

				LPGAMEOBJECT point = new Point(MUSHROOM_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(MUSHROOM_POINT);

				if (player->GetLevel() != MARIO_LEVEL_BIG) {
					mushroom->GotObsorbed(player);
					player->SetBackupLevel(MARIO_LEVEL_BIG);
					player->SetBackupState(player->GetState());
					player->SetStartTransforming((DWORD)GetTickCount64());
					player->turnIntoBig();
				}
			}
			else if (dynamic_cast<GreenMushroom*>(obj)) {
				GreenMushroom* mushroom = dynamic_cast<GreenMushroom*>(obj);
				float x, y;

				mushroom->GetPosition(x, y);

				LPGAMEOBJECT point = new Point(GREEN_MUSHROOM_POINT, x, y);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(MUSHROOM_POINT);
			}
			else if (dynamic_cast<Leaf*>(obj)) {
				Leaf* leaf = dynamic_cast<Leaf*>(obj);

				float pX, pY;

				player->GetPosition(pX, pY);

				leaf->GotObsorbed(player);
				LPGAMEOBJECT point = new Point(LEAF_POINT, pX, pY);
				Grid::GetInstance()->putObjectIntoGrid(point);
				Board::GetInstance()->GetPoint()->Add(MUSHROOM_POINT);

				if (player->GetLevel() != MARIO_LEVEL_RACOON && player->GetLevel() != MARIO_LEVEL_SMALL) {
					player->SetStartTransforming((DWORD)GetTickCount64());
					player->BigToRacoon();
				}
			}
		}
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	/*if (GetTickCount() - CGame::GetInstance()->player->GetUntouchableStart() < 700) {
		return;
	}*/

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if (mario->GetMadeItToNextScene() && mario->GetIsStanding()) {
		mario->SetVx(MARIO_WALKING_SPEED);
		return;
	}

	switch (KeyCode)
	{
	case DIK_S:
		if ((mario->GetLevel() == MARIO_LEVEL_RACOON && mario->GetIsSliding() && mario->GetIsRunning()) || mario->GetIsFlying()) {
			mario->SetState(MARIO_STATE_FLY);
			mario->flyUp = 1;
		}
		else if (!mario->GetIsStanding() && mario->GetLevel() == MARIO_LEVEL_RACOON && !mario->GetIsFlying() && !mario->GetIsGliding() && !mario->GetIsFalling()) {
			//mario->SetState(MARIO_STATE_FALL_TAIL);
			mario->StartFallingTail();
		}
		else {
			mario->SetState(MARIO_STATE_JUMP);
		}

		break;
	case DIK_UP:
		if (mario->touchPortalPipe) {
			mario->toExtraScene = 1;
			mario->vy = -MARIO_GET_INTO_PIPE_SPEED;
		}

		break;
	case DIK_DOWN:
		if (mario->touchPortalPipe) {
			mario->toExtraScene = 1;
			mario->vy = MARIO_GET_INTO_PIPE_SPEED;
		}

		break;
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_R:
		mario->Reset();
		break;
	case DIK_1:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			mario->ToBig();
		}
		mario->SetStartTransforming((DWORD)GetTickCount64());
		mario->BigToRacoon();
		//mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_2:
		if (mario->GetLevel() != MARIO_LEVEL_BIG) {
			mario->SetBackupLevel(MARIO_LEVEL_BIG);
			mario->SetBackupState(mario->GetState());
			mario->SetStartTransforming((DWORD)GetTickCount64());
			mario->turnIntoBig();
		}
		break;
	case DIK_3:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			mario->ToBig();
		}
		mario->SetStartTransforming((DWORD)GetTickCount64());
		mario->BigToFire();

		break;
	case DIK_5:
		mario->SetPosition(2560, 352);
		break;
	case DIK_6:
		mario->SetPosition(2256, 64);
		CGame::GetInstance()->SetCamPos(2096, 32);
		break;
	default:
		if (mario->GetIsFlying()) {
			//mario->SetState(MARIO_STATE_FALL);
		}

		break;
	}

}

//void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
//	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
//
//	switch (KeyCode) {
//	case DIK_RIGHT:
//		mario->SetState(MARIO_STATE_IDLE);
//		break;
//	case DIK_LEFT:
//		mario->SetState(MARIO_STATE_IDLE);
//		break;
//	}
//}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	/*if (GetTickCount() - CGame::GetInstance()->player->GetUntouchableStart() < 700) {
		return;
	}*/

	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE) return;

	if (mario->gettingOutPipe) {
		mario->SetVy((mario->exitDirect < 0) ? -MARIO_GET_INTO_PIPE_SPEED : MARIO_GET_INTO_PIPE_SPEED);
		return;
	}

	if (game->IsKeyDown(DIK_A)) {
		if (!mario->tailAttacked && mario->GetLevel() == MARIO_LEVEL_RACOON) {
			// start attack tail
			mario->tailAttacked = 1;
			mario->StartAttackingWithTail();
		}
		if (!mario->isAttackingFire && mario->GetLevel() == MARIO_LEVEL_FIRE) {
			mario->StartAttackingWithFire();
		}
		mario->SetIsRunning(1);
		mario->SetCanHold(1);
		if (abs(mario->GetVx()) >= MARIO_RUNNING_SPEED)
		{
			mario->SetState(MARIO_STATE_SLIDE);
			mario->SetIsSliding(1);
		}
		else {
			mario->SetIsSliding(0);
		}
	}
	else {
		if (mario->GetCanHold() && mario->GetBeingHoldedObj()) {
			// kick
			int tmp = (mario->GetNx() > 0) ? 1 : -1;
			float tmpX, tmpY;
			float l, t, r, b;

			mario->GetBoundingBox(l, t, r, b);
			mario->GetBeingHoldedObj()->GetPosition(tmpX, tmpY);
			//mario->GetBeingHoldedObj()->SetPosition(r + 0.4f, tmpY);

			if (mario->GetNx() > 0 || mario->GetVx() > 0) {
				mario->GetBeingHoldedObj()->SetPosition(r + 1, tmpY);
			}
			else {
				mario->GetBeingHoldedObj()->SetPosition(l - 1 - 16, tmpY);
			}

			mario->StartKicking();
			mario->GetBeingHoldedObj()->GetKicked(tmp);
			mario->hasJustKicked = 1;
			mario->SetBeingHoldedObj(NULL);
		}
		mario->SetCanHold(0);
		mario->SetIsRunning(0);
		mario->SetIsSliding(0);
		mario->tailAttacked = 0;
	}
	//DebugOut(L"%d\n", mario->GetNx());

	// disable control key when Mario die 

	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT)) {
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	/*else if (game->IsKeyDown(DIK_S)) {
		mario->SetState(MARIO_STATE_JUMP);
	}*/
	else if (mario->GetIsFlying()) {
		//mario->SetState(MARIO_STATE_FLY);
		//mario->SetState(MARIO_STATE_FALL);
		mario->vx = 0;
	}
	else if (mario->GetIsStanding()) {
		if (mario->GetMadeItToNextScene()) {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else {
			mario->SetState(MARIO_STATE_IDLE);
		}
	}
		
}