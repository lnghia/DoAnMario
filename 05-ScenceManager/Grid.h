#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "Utils.h"

#include "GameObject.h"
#include "Game.h"

#include <fstream>

using namespace std;

#define ROW second
#define COLUMN first

#define LEFT 0
#define RIGHT 1
#define TOP 2
#define BOTTOM 3

#define MAX_GRID_LINE	1024

class Grid
{
	//vector<vector<LPGAMEOBJECT>> grid;

	static Grid* instance;

	// first dimension is the row index, second dimension is column index
	vector<vector<unordered_set<LPGAMEOBJECT>>> grid;

	vector<LPGAMEOBJECT> noMoreNeeded;

	int rowNum;
	int colNum;

	int cellWidth;
	int cellHeight;
public:
	Grid();
	Grid(int cellWidth, int cellHeight);
	Grid(int cellWidth, int cellHeight, vector<LPGAMEOBJECT>& objects);
	Grid(vector<LPGAMEOBJECT>& objects);

	//static int generateObjId();

	void moveObj(LPGAMEOBJECT obj, RECT oldBoundingBox);

	void load(int cellWidth, int cellHeight);
	void loadFromFile(string& filePath, unordered_map<int, LPGAMEOBJECT>& objs_with_id);

	// returned result will be a vector of 4 integers
	// 0: from column
	// 1: to column
	// 2: from row
	// 3: to row
	vector<int> getOverLapCells(LPGAMEOBJECT obj);

	vector<int> getOverLapCells(RECT boundingBox);

	vector<int> getOverLapCells(const float& left, const float& top, const float& right, const float& bottom);

	void clearObjFromGrid(LPGAMEOBJECT obj);

	void clearObjFromCell(LPGAMEOBJECT obj, int& row, int& col);

	void ObjIntoTrash(LPGAMEOBJECT obj);
	
	void cleanObjTrashBin();

	void unload();

	//pair<int, int> getObjectCell(LPGAMEOBJECT object);

	void putObjectsInToGrid(vector<LPGAMEOBJECT>& objects);

	void putObjectIntoGrid(LPGAMEOBJECT obj);

	void putObjectIntoGrid(string& line, unordered_map<int, LPGAMEOBJECT>& objs_with_id);

	vector<LPGAMEOBJECT> getObjectsInCell(int x, int y);

	vector<LPGAMEOBJECT> getObjectsIn9CellsAroundPlayer(LPGAMEOBJECT player);

	void GetBroadPhaseBox(LPGAMEOBJECT obj, float& left, float& top, float& right, float& bottom);

	vector<LPGAMEOBJECT> GetPotentialCollidableObjects(LPGAMEOBJECT obj);


	vector<LPGAMEOBJECT> GetObjectsInCamera();

	static Grid* GetInstance();
};

