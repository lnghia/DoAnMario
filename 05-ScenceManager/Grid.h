#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "Utils.h"

#include "GameObject.h"
#include "Game.h"

using namespace std;

#define ROW second
#define COLUMN first

#define LEFT 0
#define RIGHT 1
#define TOP 2
#define BOTTOM 3

class Grid
{
	//vector<vector<LPGAMEOBJECT>> grid;

	static Grid* instance;

	// first dimension is the row index, second dimension is column index
	vector<vector<unordered_set<LPGAMEOBJECT>>> grid;

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

	void unload();

	//pair<int, int> getObjectCell(LPGAMEOBJECT object);

	void putObjectsInToGrid(vector<LPGAMEOBJECT>& objects);

	void putObjectIntoGrid(LPGAMEOBJECT obj);

	vector<LPGAMEOBJECT> getObjectsInCell(int x, int y);

	vector<LPGAMEOBJECT> getObjectsIn9CellsAroundPlayer(LPGAMEOBJECT player);

	void GetBroadPhaseBox(LPGAMEOBJECT obj, float& left, float& top, float& right, float& bottom);

	vector<LPGAMEOBJECT> GetPotentialCollidableObjects(LPGAMEOBJECT obj);


	vector<LPGAMEOBJECT> GetObjectsInCamera();

	static Grid* GetInstance();
};

// implement playscene
// load the grid along with loading the playscene
// unload the grid along with unloading the playscene
// infact, the grid might have to be unloaded before unloading the playscene 
// or maybe we can just get rid of the vector of objects and use the grid instead.
// before doing sweptAABB, get the surrounding objects and check with them, no need to check any more than that.
// this will be done in checking collision step in update() of mario
// complete the map along with the objects and start testing
// update the cell of the moving objects.
// use vector to store the objects in each cell instead of unordered_set
// object should store a list of cells that contains it 