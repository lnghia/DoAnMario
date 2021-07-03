#include "Grid.h"
#include "Map.h"
#include "FireBall.h"
#include <math.h>
#include "Mario.h"

Grid* Grid::instance = NULL;

Grid::Grid()
{
	colNum = 0;
	rowNum = 0;
}

Grid::Grid(int cellWidth, int cellHeight)
{
	CGame* cgameInstance = CGame::GetInstance();

	this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;

	rowNum = cgameInstance->GetScreenHeight() / cellHeight;
	colNum = cgameInstance->GetScreenWidth() / cellWidth;

	grid.resize(rowNum, vector<unordered_set<LPGAMEOBJECT>>(colNum, unordered_set<LPGAMEOBJECT>()));
}

Grid::Grid(int cellWidth, int cellHeight, vector<LPGAMEOBJECT>& objects)
{
}

Grid::Grid(vector<LPGAMEOBJECT>& objects)
{
}

void Grid::moveObj(LPGAMEOBJECT obj, RECT oldBoundingBox)
{
	//clearObjFromGrid(obj);

	vector<int> beforeMovingCells = getOverLapCells(oldBoundingBox);
	//vector<int> afterMovingCells = getOverLapCells(obj);

	for (int r = beforeMovingCells[2]; r <= beforeMovingCells[3]; ++r) {
		if (r<0 || r>rowNum) break;
		for (int c = beforeMovingCells[0]; r <= beforeMovingCells[1]; ++c) {
			if (c<0 || c>colNum) break;
			grid[r][c].erase(obj);
		}
	}

	putObjectIntoGrid(obj);
}

void Grid::clearObjFromGrid(LPGAMEOBJECT obj) {
	float x, y;

	obj->GetPosition(x, y);

	vector<int> overlapseCells = getOverLapCells(obj);

	//DebugOut(L"a\n");

	for (int r = overlapseCells[TOP]-1; r <= overlapseCells[BOTTOM]+1; ++r) {
		if (r < 0 || r >= rowNum) continue;
		for (int c = overlapseCells[LEFT]-1; c <= overlapseCells[RIGHT]+1; ++c) {
			if (c < 0 || c >= colNum) continue;
			if (grid[r][c].find(obj) != grid[r][c].end()) {
				grid[r][c].erase(obj);
			}
		}
	}
	//DebugOut(L"b\n");
}

void Grid::clearObjFromCell(LPGAMEOBJECT obj, int& row, int& col) {
	if (row >= rowNum || col >= colNum) {
		return;
	}

	if (grid[row][col].find(obj) != grid[row][col].end()) {
		grid[row][col].erase(obj);
	}
}

void Grid::ObjIntoTrash(LPGAMEOBJECT obj)
{
	//clearObjFromGrid(obj);
	noMoreNeeded.push_back(obj);
}

void Grid::cleanObjTrashBin()
{
	for (auto& obj : noMoreNeeded) {
		clearObjFromGrid(obj);
		delete obj;
	}

	noMoreNeeded.clear();
}

void Grid::load(int cellWidth, int cellHeight)
{
	this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;

	colNum = (int)ceil((float)Map::getInstance()->getWidth() / cellWidth);
	rowNum = (int)ceil((float)Map::getInstance()->getHeight() / cellHeight);

	grid.resize(rowNum, vector<unordered_set<LPGAMEOBJECT>>(colNum, unordered_set<LPGAMEOBJECT>()));
}

vector<int> Grid::getOverLapCells(LPGAMEOBJECT obj)
{
	float left, top, right, bottom;

	obj->GetBoundingBox(left, top, right, bottom);

	int col1 = (int)(left / cellWidth);
	int col2 = (int)(right / cellWidth);
	int row1 = (int)(top / cellHeight);
	int row2 = (int)(bottom / cellHeight);

	if (row1 >= rowNum || row2 >= rowNum) {
		int t = 1;
	}

	return { col1, col2, row1, row2 };
}

vector<int> Grid::getOverLapCells(RECT boundingBox)
{
	int col1 = (int)boundingBox.left / cellWidth;
	int col2 = (int)boundingBox.right / cellWidth;
	int row1 = (int)boundingBox.top / cellHeight;
	int row2 = (int)boundingBox.bottom / cellHeight;

	return { col1, col2, row1, row2 };
}

vector<int> Grid::getOverLapCells(const float& left, const float& top, const float& right, const float& bottom) {
	int col1 = (int)floor(left / cellWidth);
	int col2 = (int)floor(right / cellWidth);
	int row1 = (int)floor(top / cellHeight);
	int row2 = (int)floor(bottom / cellHeight);

	return { col1, col2, row1, row2 };
}

void Grid::unload()
{
	for (int r = 0; r < (int)grid.size(); ++r) {
		for (int c = 0; c < (int)grid[r].size(); ++c) {
			for (auto& obj : grid[r][c]) {
				delete obj;
			}
			grid[r][c].clear();
		}
		grid[r].clear();
	}

	grid.resize(rowNum, vector<unordered_set<LPGAMEOBJECT>>(colNum, unordered_set<LPGAMEOBJECT>()));

	DebugOut(L"[INFO] Grid unloaded!\n");
}

void Grid::putObjectsInToGrid(vector<LPGAMEOBJECT>& objects)
{
	for (auto obj : objects) {
		putObjectIntoGrid(obj);
	}
}

void Grid::putObjectIntoGrid(LPGAMEOBJECT obj)
{
	vector<int> cords = getOverLapCells(obj);

	//DebugOut(L"c\n");
	//DebugOut(L"%d %d %d %d\n", cords[TOP], cords[BOTTOM], cords[LEFT], cords[RIGHT]);

	if (cords[TOP] >= rowNum || cords[BOTTOM] >= rowNum) {
		int tmp = 1;
	}

	for (int row = cords[TOP]; row <= cords[BOTTOM]; ++row) {
		if (row < 0 || row >= rowNum) break;
		for (int col = cords[LEFT]; col <= cords[RIGHT]; ++col) {
			if (col < 0 || col >= colNum) break;
			grid[row][col].insert(obj);
		}
	}
	//DebugOut(L"d\n");
}

//pair<int, int> Grid::getObjectCell(LPGAMEOBJECT object)
//{
//	float left, top, right, bottom;
//
//	object->GetBoundingBox(left, top, right, bottom);
//
//	vector<int> cords = getOverLapCells(object);
//
//	return { (int)x % cellWidth, (int)y / cellWidth };
//}

vector<LPGAMEOBJECT> Grid::getObjectsInCell(int x, int y)
{
	vector<LPGAMEOBJECT> rs;

	if (x < 0 || x >= colNum || y < 0 || y >= rowNum) {
		return {};
	}

	rs.assign(grid[y][x].begin(), grid[y][x].end());

	return rs;
}

vector<LPGAMEOBJECT> Grid::getObjectsIn9CellsAroundPlayer(LPGAMEOBJECT player)
{
	/*vector<pair<int, int>> directions{
		{1, 1},
		{-1, -1},
		{1, 0},
		{0, 1},
		{0, 0},
		{-1, 0},
		{0, -1},
		{1, -1},
		{-1, 1}
	};*/

	vector<LPGAMEOBJECT> rs;

	// col1, col2, row1, row2
	vector<int> overLapCells = getOverLapCells(player);

	for (int r = overLapCells[2]; r <= overLapCells[3]; ++r) {
		for (int c = overLapCells[0]; c <= overLapCells[1]; ++c) {
			for (auto& obj : grid[r][c]) {
				rs.push_back(obj);
			}
		}
	}

	/*for (auto& dir : directions) {
		for (auto& obj : grid[row + dir.first][col + dir.second]) {
			rs.push_back(obj);
		}
	}*/

	return rs;
}

void Grid::GetBroadPhaseBox(LPGAMEOBJECT obj, float& left, float& top, float& right, float& bottom) {
	float vx, vy;
	float l, t, r, b;
	DWORD dt;

	obj->GetSpeed(vx, vy);
	obj->GetBoundingBox(l, t, r, b);
	dt = obj->GetDeltaTime();

	float dx = vx * dt;
	float dy = vy * dt;

	left = dx > 0 ? l : l + dx;
	top = dy > 0 ? t : t + dy;
	right = dx > 0 ? r + dx : r;
	bottom = dy > 0 ? b + dy : b;
}

vector<LPGAMEOBJECT> Grid::GetPotentialCollidableObjects(LPGAMEOBJECT obj)
{
	float bpLeft, bpTop, bpRight, bpBottom;
	vector<LPGAMEOBJECT> result;
	unordered_set<LPGAMEOBJECT> uniqueChecker; 

	GetBroadPhaseBox(obj, bpLeft, bpTop, bpRight, bpBottom);

	vector<int> overLapCells = getOverLapCells(bpLeft, bpTop, bpRight, bpBottom);

	//DebugOut(L"e\n");

	if (overLapCells[TOP] < 0 || overLapCells[TOP] >= rowNum || overLapCells[BOTTOM] < 0 || overLapCells[BOTTOM] >= rowNum ||
		overLapCells[LEFT] < 0 || overLapCells[LEFT] >= colNum || overLapCells[RIGHT] < 0 || overLapCells[RIGHT] >= colNum) {
		int tmp = 1;
	}

	float vx, vy;

	obj->GetSpeed(vx, vy);

	/*if (!vx && !vy) {
		return {};
	}*/

	overLapCells[TOP] -= (overLapCells[TOP] > 0);
	overLapCells[BOTTOM] += (overLapCells[BOTTOM] + 1 < rowNum);
	overLapCells[LEFT] -= (overLapCells[LEFT] > 0);
	overLapCells[RIGHT] += (overLapCells[RIGHT] + 1 < colNum);

	if (overLapCells[TOP] < 0 || overLapCells[TOP] >= rowNum || overLapCells[BOTTOM] < 0 || overLapCells[BOTTOM] >= rowNum ||
		overLapCells[LEFT] < 0 || overLapCells[LEFT] >= colNum || overLapCells[RIGHT] < 0 || overLapCells[RIGHT] >= colNum) {
		int tmp = 1;
	}

	for (int r = overLapCells[TOP]; r <= overLapCells[BOTTOM]; ++r) {
		if (r < 0 || r >= (int)grid.size()) {
			break;
		}
		for (int c = overLapCells[LEFT]; c <= overLapCells[RIGHT]; ++c) {
			if (c < 0 || c >= (int)grid[r].size()) {
				break;
			}
			for (auto& _obj : grid[r][c]) {
				float x, y;

				if (obj != _obj && _obj->GetInteractivable() && _obj->GetIsActive() && uniqueChecker.find(_obj)==uniqueChecker.end() && !dynamic_cast<CMario*>(_obj)) {
					result.push_back(_obj);
					uniqueChecker.insert(_obj);
				}
			}
		}
	}

	//DebugOut(L"f\n");

	return result;
}

vector<LPGAMEOBJECT> Grid::GetObjectsInCamera()
{
	vector<LPGAMEOBJECT> result;
	unordered_set<LPGAMEOBJECT> uniqueChecker;

	CGame* game = CGame::GetInstance();

	float camX = game->GetCamX();
	float camY = game->GetCamY();

	int scrW = game->GetScreenWidth();
	int scrH = game->GetScreenHeight();

	vector<int> overlapCells = getOverLapCells(camX, camY, camX + scrW, camY + scrH);

	for (int r = overlapCells[TOP]; r <= overlapCells[BOTTOM]; ++r) {
		for (int c = overlapCells[LEFT]; c <= overlapCells[RIGHT]; ++c) {
			for (auto& _obj : grid[r][c]) {
				if (uniqueChecker.find(_obj) == uniqueChecker.end()) {
					result.push_back(_obj);
					uniqueChecker.insert(_obj);
				}
			}
		}
	}

	return result;
}

Grid* Grid::GetInstance()
{
	if (!instance) {
		instance = new Grid();
	}

	return instance;
}
