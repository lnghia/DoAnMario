#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

void CScene::SetWorldNum(UINT worldNum)
{
	this->worldNum = worldNum;
}
