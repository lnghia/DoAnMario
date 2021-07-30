#include "WorldMapScene.h"
#include <fstream>

void WorldMapScene::_ParseSection_TEXTURES(string line)
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

void WorldMapScene::_ParseSection_SPRITES(string line)
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

void WorldMapScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < int(tokens.size()); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void WorldMapScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < int(tokens.size()); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void WorldMapScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = float(atof(tokens[1].c_str()));
	float y = float(atof(tokens[2].c_str()));
	int ani_set_id = atoi(tokens[3].c_str());
	/*int object_setting = 0;
	float xSwitch = 0;
	float ySwitch = 0;

	if (tokens.size() == 5)
		object_setting = atoi(tokens[4].c_str());
	else if (tokens.size() == 6)
	{
		xSwitch = float(atof(tokens[4].c_str()));
		ySwitch = float(atof(tokens[5].c_str()));
	}*/

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_JUST_FOR_SHOW:
		obj = new JustForShow();

		break;
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		player->SetIsInWorldMap(1);

		currCellX = (int)(x / 16);
		currCellY = (int)(y / 16);
		startX = currCellX;
		startY = currCellY;
		player->xInWorldMap = x;
		player->yInWorldMap = y;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GAMEOVER_MENU: {
		obj = new GameOverMenu();
		obj->SetInvisible(1);

		gameOverMenu = (GameOverMenu*)obj;

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

void WorldMapScene::_ParseSection_MAP(const string& line)
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

	if (tokens.size() > 8) {
		int tilesGap = stoi(tokens[8]);
		Map::getInstance()->SetSpaceBetweenTiles(tilesGap);
	}
	else {
		Map::getInstance()->SetSpaceBetweenTiles(1);
	}

	CGame* game = CGame::GetInstance();

	game->SetCamPos(0.0f, (float)(Map::getInstance()->getHeight() - game->GetScreenHeight() - 1));
}

void WorldMapScene::_ParseSection_Board(const string& line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	string obj = tokens[0];
	Board* board = Board::GetInstance();

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
}

void WorldMapScene::_ParseSection_Path(string& line)
{
	ifstream reader(line);
	string _line = "";

	while (getline(reader, _line)) {
		//vector<int> tmp = stringToVectorOfInt(line, " ");
		path.push_back(StringToVectorOfChar(_line, " "));
	}

	reader.close();
}

WorldMapScene::WorldMapScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	player = NULL;
	scene = NULL;
	currCellX = 0;
	currCellY = 0;
	key_handler = new WorldMapSceneKeyHandler(this);
}

void WorldMapScene::Load()
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
		if (line == "[BOARD]") {
			section = BOARD; continue;
		}
		if (line == "[PATH]") {
			section = PATH; continue;
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
		case BOARD: _ParseSection_Board(line); break;
		case PATH: _ParseSection_Path(line); break;
		}
	}

	f.close();

	scene = -1;

	CGame::GetInstance()->SetCamPos(0.0f, 0.0f);

	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(237, 28, 36));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void WorldMapScene::Update(DWORD dt)
{
	//CGame::GetInstance()->SetCamPos(0.0f, 0.0f); // set Cam when game start
	//Board::GetInstance()->Update(dt);

	Board::GetInstance()->Update(dt);

	if (!Board::GetInstance()->GetLives()->GetContent()) {
		gameOverMenu->SetInvisible(0);

		return;
	}

	pair<int, int> tmp = player->UpdateInWorldMap(dt);

	currCellY = tmp.second;
	currCellX = tmp.first;

	if (path[tmp.second][tmp.first] != '-' && (tmp.first != startX || tmp.second != startY)) {
		if (player->vy > 0) player->yInWorldMap = (float)(currCellY * 16);
		else if (player->vy < 0) player->yInWorldMap = (float)(currCellY * 16);
		else if (player->vx > 0) player->xInWorldMap = (float)(currCellX * 16);
		else if (player->vx < 0) player->xInWorldMap = (float)(currCellX * 16);

		player->x = player->xInWorldMap;

		player->SetVx(0);
		player->SetVy(0);
		startX = currCellX;
		startY = currCellY;

		if (path[tmp.second][tmp.first] != 'O' && path[tmp.second][tmp.first] != 'X') {
			scene = path[tmp.second][tmp.first] - '0';
		}
		else {
			scene = -1;
		}
	}
}

void WorldMapScene::Render()
{
	Map::getInstance()->Draw();
	Board::GetInstance()->Render();
	player->Render();

	for (auto& item : objects) {
		if(!item->GetInvisible())
			item->Render();
	}

	if (!gameOverMenu->GetInvisible()) {
		gameOverMenu->Render();
	}
	//gameOverMenu->Render();
}

void WorldMapScene::Unload()
{
	for (int i = 0; i < int(objects.size()); i++)
		delete objects[i];

	objects.clear();

	for (auto& tmp : path)
		tmp.clear();

	path.clear();

	player = NULL;
	gameOverMenu = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void WorldMapScene::SetMarioLevel(int val)
{
	marioLevel = val;
	player->SetLevel(val);
}

void WorldMapSceneKeyHandler::KeyState(BYTE* states)
{
}

void WorldMapSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CMario* mario = ((WorldMapScene*)scence)->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN: {
		if (!((WorldMapScene*)scence)->gameOverMenu->GetInvisible()) {
			int option = ((WorldMapScene*)scence)->gameOverMenu->option;

			((WorldMapScene*)scence)->gameOverMenu->option = (option + 1) % 2;

			break;
		}

		float mx, my;

		mx = mario->xInWorldMap;
		my = mario->yInWorldMap;

		int cellX = ((WorldMapScene*)scence)->currCellX;
		int cellY = ((WorldMapScene*)scence)->currCellY + 1;

		if (cellY < (int)((WorldMapScene*)scence)->path.size() && cellX < (int)((WorldMapScene*)scence)->path[0].size() && ((WorldMapScene*)scence)->path[cellY][cellX] != 'X') {
			mario->SetVy(MARIO_WALKING_SPEED);
			mario->SetVx(0);
		}
		else {
			mario->SetVx(0);
			mario->SetVy(0);
		}

		break;
	}
	case DIK_UP: {
		if (!((WorldMapScene*)scence)->gameOverMenu->GetInvisible()) {
			int option = ((WorldMapScene*)scence)->gameOverMenu->option;

			((WorldMapScene*)scence)->gameOverMenu->option = (option + 1) % 2;

			break;
		}

		float mx, my;

		mx = mario->xInWorldMap;
		my = mario->yInWorldMap;

		int cellX = ((WorldMapScene*)scence)->currCellX;
		int cellY = ((WorldMapScene*)scence)->currCellY - 1;

		if (cellY < (int)((WorldMapScene*)scence)->path.size() && cellX < (int)((WorldMapScene*)scence)->path[0].size() && ((WorldMapScene*)scence)->path[cellY][cellX] != 'X') {
			mario->SetVy(-MARIO_WALKING_SPEED);
			mario->SetVx(0);
		}
		else {
			mario->SetVx(0);
			mario->SetVy(0);
		}

		break;
	}
	case DIK_LEFT: {
		float mx, my;

		mx = mario->xInWorldMap;
		my = mario->yInWorldMap;

		int cellX = ((WorldMapScene*)scence)->currCellX - 1;
		int cellY = ((WorldMapScene*)scence)->currCellY;

		if (cellY < (int)((WorldMapScene*)scence)->path.size() && cellX < (int)((WorldMapScene*)scence)->path[0].size() && ((WorldMapScene*)scence)->path[cellY][cellX] != 'X') {
			mario->SetVy(0);
			mario->SetVx(-MARIO_WALKING_SPEED);
		}
		else {
			mario->SetVx(0);
			mario->SetVy(0);
		}

		break;
	}
	case DIK_RIGHT: {
		float mx, my;

		//mario->GetPosition(mx, my);
		mx = mario->xInWorldMap;
		my = mario->yInWorldMap;

		int cellX = ((WorldMapScene*)scence)->currCellX + 1;
		int cellY = ((WorldMapScene*)scence)->currCellY;

		if (cellY < (int)((WorldMapScene*)scence)->path.size() && cellX < (int)((WorldMapScene*)scence)->path[0].size() && ((WorldMapScene*)scence)->path[cellY][cellX] != 'X') {
			mario->SetVy(0);
			mario->SetVx(MARIO_WALKING_SPEED);
		}
		else {
			mario->SetVx(0);
			mario->SetVy(0);
		}

		break;
	}
	case DIK_S: {
		//Board* board = Board::GetInstance();

		//board->GetTime()->SetTime(0);
		//board->SetLives(3);
		if (!((WorldMapScene*)scence)->gameOverMenu->GetInvisible()) {
			int option = ((WorldMapScene*)scence)->gameOverMenu->option;

			if (option) {
				Map::getInstance()->unLoad();

				CGame::GetInstance()->SwitchScene(0);
				Board::GetInstance()->SetLives(3);
			}
			else {
				//Map::getInstance()->unLoad();

				((WorldMapScene*)scence)->gameOverMenu->SetInvisible(1);

				//CGame::GetInstance()->SwitchScene(4);
				Board::GetInstance()->SetLives(3);
				Board::GetInstance()->GetPoint()->SetContent(0);
				Board::GetInstance()->GetMoney()->SetContent(0);
			}

			break;
		}

		int sceneId = ((WorldMapScene*)scence)->scene;

		if (sceneId != 1 && sceneId != 7) {
			break;
		}

		Map::getInstance()->unLoad();

		CGame::GetInstance()->SwitchScene(sceneId);
		Board::GetInstance()->GetCardStack()->RefreshItemAni();
		Board::GetInstance()->GetTime()->SetStart(NULL);

		/*Board* board = Board::GetInstance();
		board->SetLives(3);*/

		break;
	}
	default:
		break;
	}
}

void WorldMapSceneKeyHandler::OnKeyUp(int KeyCode)
{
}
