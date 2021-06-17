#include "Board.h"

Board* Board::__instance = NULL;

Board::Board()
{
	point = new Displayer(7);
	money = new Displayer(2);
	time = new TimeTicker();
	lives = new Displayer(2);
	worldNum = new Displayer(1);
	//worldField = new Displayer();
	playerChar = new JustForShow();
	//clock = new JustForShow();
	//dollarSign = new JustForShow();
	cardStack = new EndGameItemStack();
	speedBar = new SpeedBar();

	CGame* game = CGame::GetInstance();

	x = game->GetCamX();
	y = game->GetCamY() + game->GetScreenHeight() - BOARD_BBOX_HEIGHT;
}

void Board::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + CGame::GetInstance()->GetScreenWidth();
	b = t + BOARD_BBOX_HEIGHT;
}

void Board::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();

	x = game->GetCamX();
	y = game->GetCamY() + game->GetScreenHeight() - BOARD_BBOX_HEIGHT;

	time->Update(dt);
	speedBar->Update(dt);

	UpdateComponentPos();
}

void Board::Render()
{
	RenderBackGround();

	animation_set->at(0)->Render(x + (CGame::GetInstance()->GetScreenWidth() - BOARD_BBOX_WIDTH) / 2, y);

	point->Render();
	money->Render();
	time->Render();
	lives->Render();
	worldNum->Render();
	//worldField->Render();
	//playerChar->Render();
	//clock->Render();
	//dollarSign->Render();
	cardStack->Render();
	speedBar->Render();
}

void Board::UpdatePos(float x, float y)
{
	this->x = x;
	this->y = y;

	float tempX = x + (CGame::GetInstance()->GetScreenWidth() - BOARD_BBOX_WIDTH) / 2;
	float tempY = y + (CGame::GetInstance()->GetScreenHeight() - BOARD_BBOX_HEIGHT) / 2;

	//worldField->UpdatePos(x + WORLDFIELD_X, y + WORLDFIELD_Y);
	worldNum->UpdatePos(tempX + WORLDNUM_X, y + WORLDNUM_Y);

	playerChar->SetPosition(tempX + PLAYERCHAR_X, y + PLAYERCHAR_Y);
	lives->UpdatePos(tempX + LIVES_X, y + LIVES_Y);

	point->UpdatePos(tempX + POINT_X, y + POINT_Y);

	time->UpdatePos(tempX + TIME_X, y + TIME_Y);
	//clock->SetPosition(tempX + CLOCK_tempX, y + CLOCK_Y);

	//dollarSign->SetPosition(tempX + DOLLAR_tempX, y + DOLLAR_Y);
	money->UpdatePos(tempX + MONEY_X, y + MONEY_Y);

	cardStack->UpdatePos(tempX + ITEMSTACK_X, y + ITEMSTACK_Y);

	speedBar->UpdatePos(tempX + SPEEDBAR_X, y + SPEEDBAR_Y);
}

void Board::UpdateComponentPos()
{
	float tempX = x + (CGame::GetInstance()->GetScreenWidth() - BOARD_BBOX_WIDTH) / 2;

	//worldField->UpdatePos(x + WORLDFIELD_X, y + WORLDFIELD_Y);
	worldNum->UpdatePos(tempX + WORLDNUM_X, y + WORLDNUM_Y);

	playerChar->SetPosition(tempX + PLAYERCHAR_X, y + PLAYERCHAR_Y);
	lives->UpdatePos(tempX + LIVES_X, y + LIVES_Y);

	point->UpdatePos(tempX + POINT_X, y + POINT_Y);

	time->UpdatePos(tempX + TIME_X, y + TIME_Y);
	//clock->SetPosition(tempX + CLOCK_tempX, y + CLOCK_Y);

	//dollarSign->SetPosition(tempX + DOLLAR_tempX, y + DOLLAR_Y);
	money->UpdatePos(tempX + MONEY_X, y + MONEY_Y);

	cardStack->UpdatePos(tempX + ITEMSTACK_X, y + ITEMSTACK_Y);

	speedBar->UpdatePos(tempX + SPEEDBAR_X, y + SPEEDBAR_Y);
}

void Board::RenderBackGround()
{
	D3DXVECTOR3 p(x, y - 10, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(BACKGOUND_TEX_ID);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 255);
}

Displayer* Board::GetPoint()
{
	return point;
}

Displayer* Board::GetMoney()
{
	return money;
}

TimeTicker* Board::GetTime()
{
	return time;
}

Displayer* Board::GetLives()
{
	return lives;
}

Displayer* Board::GetWorldNum()
{
	return worldNum;
}

Displayer* Board::GetWorldField()
{
	return worldField;
}

JustForShow* Board::GetPlayerChar()
{
	return playerChar;
}

JustForShow* Board::GetClock()
{
	return clock;
}

JustForShow* Board::GetDollarSign()
{
	return dollarSign;
}

EndGameItemStack* Board::GetCardStack()
{
	return cardStack;
}

SpeedBar* Board::GetSpeedBar()
{
	return speedBar;
}

void Board::SetAniSet(int ani_set)
{
	animation_set = CAnimationSets::GetInstance()->Get(ani_set);
}

Board* Board::GetInstance()
{
	if (!__instance) {
		__instance = new Board();
	}

	return __instance;
}
