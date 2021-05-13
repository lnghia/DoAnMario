#include "Grid.h"
#include "Map.h"

#include <math.h>

Grid* Grid::instance = NULL;

Grid::Grid()
{
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
	vector<int> beforeMovingCells = getOverLapCells(oldBoundingBox);
	vector<int> afterMovingCells = getOverLapCells(obj);


}

void Grid::clearObjFromGrid(LPGAMEOBJECT obj) {
	float x, y;

	obj->GetPosition(x, y);

	int row = (int)y % rowNum;
	int col = (int)x % colNum;

	if ((col + 1 < colNum && grid[row][col + 1].find(obj) != grid[row][col + 1].end()) ||
		(row + 1 < rowNum && grid[row + 1][col].find(obj) != grid[row + 1][col].end())) {

		for (int r = row; r < grid.size(); ++r) {
			for (int c = col; c < grid.size(); ++c) {
				if (grid[row][col].find(obj) != grid[row][col].end()) {
					grid[row][col].erase(obj);
				}
				else {
					break;
				}
			}
		}

	}
}

void Grid::clearObjFromCell(LPGAMEOBJECT obj, int& row, int& col) {
	if (row >= rowNum || col >= colNum) {
		return;
	}

	if (grid[row][col].find(obj) != grid[row][col].end()) {
		grid[row][col].erase(obj);
	}
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

	int col1 = (int)left % colNum;
	int col2 = (int)right % colNum;
	int row1 = (int)top / colNum;
	int row2 = (int)bottom / colNum;

	return { col1, col2, row1, row2 };
}

vector<int> Grid::getOverLapCells(RECT boundingBox)
{
	int col1 = (int)boundingBox.left % colNum;
	int col2 = (int)boundingBox.right % colNum;
	int row1 = (int)boundingBox.top / colNum;
	int row2 = (int)boundingBox.bottom / colNum;

	return { col1, col2, row1, row2 };
}

vector<int> Grid::getOverLapCells(const float& left, const float& top, const float& right, const float& bottom) {
	int col1 = (int)left % colNum;
	int col2 = (int)right % colNum;
	int row1 = (int)top / colNum;
	int row2 = (int)bottom / colNum;

	return { col1, col2, row1, row2 };
}

void Grid::unload()
{
	for (int r = 0; r < grid.size(); ++r) {
		for (int c = 0; c < grid[r].size(); ++c) {
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

	for (int row = cords[TOP]; row <= cords[BOTTOM]; ++row) {
		for (int col = cords[LEFT]; col <= cords[RIGHT]; ++col) {
			grid[row][col].insert(obj);
		}
	}
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

	GetBroadPhaseBox(obj, bpLeft, bpTop, bpRight, bpBottom);

	vector<int> overLapCells = getOverLapCells(bpLeft, bpTop, bpRight, bpBottom);

	for (int r = overLapCells[2]; r <= overLapCells[3]; ++r) {
		for (int c = overLapCells[0]; c <= overLapCells[1]; ++c) {
			for (auto& obj : grid[r][c]) {
				result.push_back(obj);
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
