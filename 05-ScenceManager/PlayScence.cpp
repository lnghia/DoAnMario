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

#include "ObjectCheatSheet.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
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
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
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

	for (int i = 1; i < tokens.size(); i++)
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

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	if (object_type == 10) {
		int tmp = 1;
	}

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
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_QBRICK: {
		int hiddenItemType = atoi(tokens[4].c_str());
		int	hiddenItemAni = atoi(tokens[5].c_str());

		obj = new QBrick(hiddenItemType, hiddenItemAni);

		break;
	}
	case OBJECT_TYPE_BRICK:
		obj = new CBrick();
		break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_JUST_FOR_SHOW:
		obj = new JustForShow(); break;
	case OBJECT_TYPE_INVISIBLE: {
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());

		obj = new InteractivableTransObject(width, height);

		break;
	}
	case OBJECT_TYPE_PIPE_HITBOX: {
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());

		obj = new PipeHitBox(width, height);

		break;
	}
	case OBJECT_TYPE_PIPE_FOR_SHOW: {
		obj = new JustForShow();

		obj->SetRenderPriority(30);

		break;
	}
	case OBJECT_TYPE_COLOR_BRICK_HITBOX: {
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());

		obj = new ColorBrickHitBox(width, height);

		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT: {
		float pipeX = atof(tokens[4].c_str());
		float pipeY = atof(tokens[5].c_str());
		float pipeWidth = atof(tokens[6].c_str());
		float pipeHeight = atof(tokens[7].c_str());

		if (!player) {
			DebugOut(L"[Error] Player is not ready for initiating Piranha Plant");
		}

		obj = new PiranhaPlant(pipeX, pipeY, pipeWidth, pipeHeight, player);
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
	Grid::GetInstance()->putObjectIntoGrid(obj);
}

void CPlayScene::_ParseSection_GRID(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	// 1: cellWidth, 2: cellHeight
	Grid::GetInstance()->load(stoi(tokens[0]), stoi(tokens[1]));
}

void CPlayScene::_ParseSection_MAP(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 8) return;

	Map::getInstance()->loadFromFile(tokens[0],
		stoi(tokens[1]),
		stoi(tokens[2]),
		stoi(tokens[3]),
		stoi(tokens[4]),
		stoi(tokens[5]),
		stoi(tokens[6]),
		stoi(tokens[7]));

	CGame* game = CGame::GetInstance();

	game->SetCamPos(0.0f, Map::getInstance()->getHeight() - game->GetScreenHeight() - 1);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

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
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case MAP: _ParseSection_MAP(line); break;
		case GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

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

	//while (state == MARIO_STATE_BIG_TO_SMALL && untouchable && GetTickCount() - untouchable_start < 700) {
	//	/*coObjects = Grid::GetInstance()->GetPotentialCollidableObjects(player);
	//	player->Update(dt, &coObjects);*/

	//	if (level == MARIO_LEVEL_BIG) {
	//		level = MARIO_LEVEL_SMALL;
	//	}
	//	else {
	//		level = MARIO_LEVEL_BIG;
	//	}
	//	Render();



	//}

	if (player->GetUntouchable() && GetTickCount() - player->GetUntouchableStart() < 700) {
		/*coObjects = Grid::GetInstance()->GetPotentialCollidableObjects(player);
		player->Update(dt, &coObjects);*/


		/*if (player->GetLevel() == MARIO_LEVEL_BIG) {
			player->SetLevel(MARIO_LEVEL_SMALL);
		}
		else {
			player->SetLevel(MARIO_LEVEL_BIG);
		}*/

		//player->Render();

		return;
	}

	for (auto& obj : objectsInCamera) {
		coObjects.clear();
		if (obj->GetInteractivable())
			coObjects = Grid::GetInstance()->GetPotentialCollidableObjects(obj);
		obj->Update(dt, &coObjects);
		/*if (dynamic_cast<Coin*>(obj)) {
			int tmp = 1;
		}*/
		if (!obj->GetIsActive()) {
			Grid::GetInstance()->clearObjFromGrid(obj);
			/*delete obj;
			obj = NULL;*/
		}
		/*if (!obj->GetIsActive() && dynamic_cast<FireBall*>(obj)) {
			int tmp = 1;
		}*/
	}

	//Grid::GetInstance()->cleanObjTrashBin();

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

	cx -= game->GetScreenWidth() / 2;

	if (player->GetState() == MARIO_STATE_FLY) {
		cy -= game->GetScreenHeight() / 2;
		if (cy + game->GetScreenHeight() > Map::getInstance()->getHeight()) {
			cy = Map::getInstance()->getHeight() - game->GetScreenHeight() - 1;
		}
	}
	else {
		cy = _cy;
	}

	cx = (cx < 0) ? 0 : cx;
	cy = (cy < 0) ? 0 : cy;


	if (cx + game->GetScreenWidth() > Map::getInstance()->getWidth()) {
		cx = Map::getInstance()->getWidth() - game->GetScreenWidth() - 1;
	}

	//DebugOut(L"[CAM POS]: %f %f\n", cx, cy);

	CGame::GetInstance()->SetCamPos(round(cx), /*100.0f*/ round(cy));
	//CGame::GetInstance()->SetCamPos(cx, /*100.0f*/ cy);
}

void CPlayScene::Render()
{
	Map::getInstance()->Draw();

	/*for (int i = 0; i < objects.size(); i++)
		if (objects[i] != player && !objects[i]->GetInvisible()) {
			objects[i]->Render();
		}*/

	vector<LPGAMEOBJECT> objectsInCamera = Grid::GetInstance()->GetObjectsInCamera();

	sort(objectsInCamera.begin(), objectsInCamera.end(), cmp);

	for (auto& obj : objectsInCamera) {
		/*if (dynamic_cast<Point*>(obj)) {
			int tmp = 1;
		}*/
		if (obj != player && !obj->GetInvisible()) {
			obj->Render();
		}
	}

	/*for (auto& obj : objects) {
		if (obj != player && !obj->GetInvisible()) {
			obj->Render();
		}
	}*/

	player->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	/*if (GetTickCount() - CGame::GetInstance()->player->GetUntouchableStart() < 700) {
		return;
	}*/

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
		mario->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	/*if (GetTickCount() - CGame::GetInstance()->player->GetUntouchableStart() < 700) {
		return;
	}*/

	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else
		mario->SetState(MARIO_STATE_IDLE);
}