/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : Main.cpp
Description : This is the main cpp file resposible for running the game and managing other files.
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

#include <iostream>
#include <windows.h>
#include <time.h>
#include <string>
#include <vector>
#include <cmath>
#include <io.h>
#include <fcntl.h>
#include <algorithm>
#include <conio.h>

#include"CGame.h"
#include"CPosition.h"
#include"CSegment.h"
#include"CShip.h"
#include"CPlayer.h"


using namespace std;


void SetupGame();
void SetFont();

void DrawBoard(CPlayer& player);
void DrawCursor(CPlayer& player, int colour);

wstring Login();
void SetUpMenu(CGame* game);
void MenuControls(CGame* game);
void ShipPlacement(CGame* game, wstring _userName);
void PlayGame(CGame* game, bool &endAfter);
void EndGame(CGame* game, int _endVal);

void PlayerTurn(CGame *game, int _beingAttacked);
void AutomaticPlace(CGame& game, vector<CShip>& toPlace, int _player);
void ManualPlace(CGame& game, vector<CShip>& toPlace);
bool CheckTempShip(CGame& game, CPosition& _startPos, CShip& _ship, int& _direction, int _player);

void Print(CPosition pos, wstring str, int effect);
void SlowPrint(CPosition _pos, wstring _message, int effect = 15, int _wait = 20);
void GotoXY(CPosition pos, int multi = 1);

int main() {
	SetupGame();

	Sleep(1000);
	
	wstring userName = Login();

	Sleep(1000);

	system("CLS");

	SetFont();

	CGame *game = new CGame();
	
	SetUpMenu(game);
	
	bool endAfter = false;

	while (game->IsRunning()) {
		switch (game->state)
		{
		case 0:
			MenuControls(game);
			break;

		case 1: {
			ShipPlacement(game, userName);
			break;	
		}

		case 2:
			PlayGame(game, endAfter);
			break;

		case 3:
			//Loss
			EndGame(game, 1);
			break;

		case 4:
			//Win
			EndGame(game, 2);
			break;

		case 5:
			//Tie
			EndGame(game, 3);
			break;

		default:
			break;
		}
		
	}

	delete game;
	game = nullptr;

	Print({0,0},L"Press <ENTER> to Exit",12);
	getchar();

	return 0;
}

void SetFont() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 16;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DECORATIVE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void SetupGame()
{
	//Enable the use of Unicode
	(void)_setmode(_fileno(stdout), _O_U16TEXT);
	Print({ 0,0 }, L"BOOTING BATTLESHIP SYSTEM", 12);

	//Maximise the screen
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	//Set the buffer size to the size of the screen
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);
	csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
	csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
	SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

	//makes Cursor Invisible
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hstdout, &info);

	//Seed for Random
	srand(static_cast <unsigned> (time(0)));
}

wstring Login()
{
	wstring tempName;
	SlowPrint({ 0,0 }, L"[COMPLETE] BATTLESHIP SYSTEM ONLINE", 10, 30);

	SlowPrint({ 2,2 }, L"ENTER DEFENCE FORCE LOGIN", 12, 30);

	SlowPrint({ 4,4 }, L"ENTER NAME: ");
	getline(wcin, tempName);

	SlowPrint({ 4,6 }, L"ENTER Password: ");

	while (true) {
		if (_kbhit()) {
			wcout << L"*";
			_getch();

		}

		if (GetKeyState(VK_RETURN) & 0x8000) break;

	}

	Print({ 6,9 }, L"LOGGING IN", 15);
	Print({ 4,11 }, L"[                            ]", 15);
	SlowPrint({ 5,11 }, L"============================", 10, 40);
	Print({ 6,13 }, L"LOGGED IN", 10);

	return tempName;
}

void SetUpMenu(CGame *game)
{
	game->AddPlayer({ L"MENU" });
	game->GetPlayer(0).AddShip({ {2,3},1,0,L"P",240 });
	game->GetPlayer(0).AddShip({ {3,3},1,0,L"L",240 });
	game->GetPlayer(0).AddShip({ {4,3},1,0,L"A",240 });
	game->GetPlayer(0).AddShip({ {5,3},1,0,L"Y",240 });

	game->GetPlayer(0).AddShip({ {2,9},1,0,L"Q",240 });
	game->GetPlayer(0).AddShip({ {3,9},1,0,L"U",240 });
	game->GetPlayer(0).AddShip({ {4,9},1,0,L"I",240 });
	game->GetPlayer(0).AddShip({ {5,9},1,0,L"T",240 });


	game->GetPlayer(0).CreateBoard();
	game->GetPlayer(0).SetBoardPos({ 5,5 });

	game->UpdateBoards();

	DrawBoard(game->GetPlayer(0));
}

void MenuControls(CGame* game)
{
	if (GetKeyState(VK_LEFT) & 0x8000) { game->GetPlayer(0).MoveCursorRight(-1); DrawBoard(game->GetPlayer(0)); };
	if (GetKeyState(VK_RIGHT) & 0x8000) { game->GetPlayer(0).MoveCursorRight(1); DrawBoard(game->GetPlayer(0)); };
	if (GetKeyState(VK_UP) & 0x8000) { game->GetPlayer(0).MoveCursorDown(-1); DrawBoard(game->GetPlayer(0)); };
	if (GetKeyState(VK_DOWN) & 0x8000) { game->GetPlayer(0).MoveCursorDown(1); DrawBoard(game->GetPlayer(0)); };
	if (GetKeyState(VK_SPACE) & 0x8000) {
		bool theHit = game->GetPlayer(0).CalcShot(game->GetPlayer(0).GetCursor());
		game->UpdateBoards();
		DrawBoard(game->GetPlayer(0));
		Print({ game->GetPlayer(0).GetCursor().x * 3 + game->GetPlayer(0).GetBoardPos().x, game->GetPlayer(0).GetCursor().y * 3 - 1 + game->GetPlayer(0).GetBoardPos().y }, theHit ? L"HIT " : L"MISS", theHit ? 47 : 79);

		if (theHit && game->GetPlayer(0).GetCursor().y == 3) {
			game->GetPlayer(0).RemoveShip(0);

			game->RemovePlayer(0);

			Sleep(500);
			game->state = 1;
		}
	}
	else {
		DrawCursor(game->GetPlayer(0), 2);

		Sleep(100);
	}
}

void ShipPlacement(CGame* game, wstring _userName)
{
	vector<CShip> toPlace{
				{ {0,0},5,0,L"A",240 },
				{ {0,0},4,0,L"B",224 },
				{ {0,0},3,0,L"D",192 },
				{ {0,0},3,0,L"S",144 },
				{ {0,0},2,0,L"P",176 }
	};

	game->AddPlayer(_userName);

	game->GetPlayer(0).CreateBoard();
	game->GetPlayer(0).SetBoardPos({ 5,5 });

	game->AddPlayer({ L"CPU" });

	game->GetPlayer(1).CreateBoard();
	game->GetPlayer(1).SetBoardPos({ 40,5 });

	AutomaticPlace(*game, toPlace, 1);

	system("CLS");

	SlowPrint({ 2,2 }, L"Select Ship Deployment Mode:", 15, 20);

	vector<int> colours{ 10,15 };

	SlowPrint({ 3,4 }, L"Manual", colours[0]);
	SlowPrint({ 3,5 }, L"Automatic", colours[1]);

	while (true)
	{

		if (GetKeyState(VK_UP) & 0x8000) {
			colours.push_back(colours[0]);
			colours.erase(colours.begin());
		}
		if (GetKeyState(VK_DOWN) & 0x8000) {
			colours.push_back(colours[0]);
			colours.erase(colours.begin());
		}



		Print({ 3,4 }, L"Manual", colours[0]);
		Print({ 3,5 }, L"Automatic", colours[1]);

		if (GetKeyState(VK_RETURN) & 0x8000) {
			system("CLS");
			break;
		}



		Sleep(100);
	}

	if (colours[0] == 10) {
		ManualPlace(*game, toPlace);
	}
	else {
		AutomaticPlace(*game, toPlace, 0);
	}

	game->ToggleDebug();

	game->GetPlayer(1).ResetBoard(); game->UpdateBoards();

	DrawBoard(game->GetPlayer(1));

	game->state = 2;
}

void PlayGame(CGame* game, bool &endAfter)
{
	if (game->GetPlayer(1).GetHits().size() >= 17) {
		if (endAfter) {
			game->state = 5;
			return;
		}
		game->state = 4;
		return;
	}
	if (game->GetPlayer(0).GetHits().size() >= 17) {
		if (endAfter && game->GetPlayerTurn() == 0) {
			game->state = 3;
		}
		else {
			endAfter = true;
			game->SetPlayerTurn(1);
		}
	}

	if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('D') & 0x8000) { game->ToggleDebug(); game->GetPlayer(1).ResetBoard(); game->UpdateBoards(); DrawBoard(game->GetPlayer(1)); };
	PlayerTurn(game, game->GetPlayerTurn());
}

void EndGame(CGame* game, int _endVal)
{
	system("CLS");


	if (_endVal == 1) {
		//Loss
		SlowPrint({ 10,4 }, L"██████  ███████ ███████ ███████  █████  ████████ ", 10, 1);
		SlowPrint({ 10,5 }, L"██   ██ ██      ██      ██      ██   ██    ██    ", 10, 1);
		SlowPrint({ 10,6 }, L"██   ██ █████   █████   █████   ███████    ██    ", 10, 1);
		SlowPrint({ 10,7 }, L"██   ██ ██      ██      ██      ██   ██    ██    ", 10, 1);
		SlowPrint({ 10,8 }, L"██████  ███████ ██      ███████ ██   ██    ██    ", 10, 1);

	}
	else if (_endVal == 2) {
		//Win

		SlowPrint({ 10,4 }, L"██    ██ ██  ██████ ████████  ██████  ██████  ██    ██ ", 10, 1);
		SlowPrint({ 10,5 }, L"██    ██ ██ ██         ██    ██    ██ ██   ██  ██  ██  ", 10, 1);
		SlowPrint({ 10,6 }, L"██    ██ ██ ██         ██    ██    ██ ██████    ████   ", 10, 1);
		SlowPrint({ 10,7 }, L" ██  ██  ██ ██         ██    ██    ██ ██   ██    ██    ", 10, 1);
		SlowPrint({ 10,8 }, L"  ████   ██  ██████    ██     ██████  ██   ██    ██    ", 10, 1);

	}
	else {
		//Tie

		SlowPrint({ 10,4 }, L"██████  ██████   █████  ██     ██ ", 10, 1);
		SlowPrint({ 10,5 }, L"██   ██ ██   ██ ██   ██ ██     ██ ", 10, 1);
		SlowPrint({ 10,6 }, L"██   ██ ██████  ███████ ██  █  ██ ", 10, 1);
		SlowPrint({ 10,7 }, L"██   ██ ██   ██ ██   ██ ██ ███ ██ ", 10, 1);
		SlowPrint({ 10,8 }, L"██████  ██   ██ ██   ██  ███ ███  ", 10, 1);

	}

	SlowPrint({5,12}, game->GetPlayer(0).GetName());
	SlowPrint({ 5,14 }, L"Shots:  " + to_wstring(game->GetPlayer(1).GetShots().size()));
	SlowPrint({ 5,15 }, L"Hits:   " + to_wstring(game->GetPlayer(1).GetHits().size()));
	SlowPrint({ 5,16 }, L"Misses: " + to_wstring(game->GetPlayer(1).GetShots().size() - game->GetPlayer(1).GetHits().size()));

	SlowPrint({ 20,12 }, game->GetPlayer(1).GetName());
	SlowPrint({ 20,14 }, L"Shots:  " + to_wstring(game->GetPlayer(0).GetShots().size()));
	SlowPrint({ 20,15 }, L"Hits:   " + to_wstring(game->GetPlayer(0).GetHits().size()));
	SlowPrint({ 20,16 }, L"Misses: " + to_wstring(game->GetPlayer(0).GetShots().size() - game->GetPlayer(0).GetHits().size()));


	Print({ 10,20 }, L"Press <ENTER> to Return To Menu", 12);
	while (true) {
		if (GetKeyState(VK_RETURN) & 0x8000) {
			system("CLS");
			game->RemovePlayer(1);
			game->RemovePlayer(0);
			SetUpMenu(game);
			game->state = 0;
			return;
		}
	}
}

void PlayerTurn(CGame *game, int _beingAttacked)
{
	if (_beingAttacked == 1) {
		if (GetKeyState(VK_LEFT) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorRight(-1); DrawBoard(game->GetPlayer(_beingAttacked)); };
		if (GetKeyState(VK_RIGHT) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorRight(1); DrawBoard(game->GetPlayer(_beingAttacked)); };
		if (GetKeyState(VK_UP) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorDown(-1); DrawBoard(game->GetPlayer(_beingAttacked)); };
		if (GetKeyState(VK_DOWN) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorDown(1); DrawBoard(game->GetPlayer(_beingAttacked)); };
		if (GetKeyState(VK_SPACE) & 0x8000) {
			game->m_AImoves = 0;
			bool theHit = game->GetPlayer(_beingAttacked).CalcShot(game->GetPlayer(_beingAttacked).GetCursor());
			game->UpdateBoards();
			DrawBoard(game->GetPlayer(_beingAttacked));
			Print({ game->GetPlayer(_beingAttacked).GetCursor().x * 3 + game->GetPlayer(_beingAttacked).GetBoardPos().x, game->GetPlayer(_beingAttacked).GetCursor().y * 3 - 1 + game->GetPlayer(_beingAttacked).GetBoardPos().y }, theHit ? L"HIT " : L"MISS", theHit ? 47 : 79);
			game->SwapPlayerTurn();
		};
	}
	else {

		CPosition betterShot = { -1,-1 };
		for (CPosition _hit : game->GetPlayer(_beingAttacked).GetHits()) {
			
			vector<CPosition> spots { {0, -1}, { 0, 1 }, { -1, 0 }, { 1, 0 } };
			
			while (spots.size() > 0) {
				int randInt = (rand() % spots.size());
				int x = spots[randInt].x;
				int y = spots[randInt].y;

				bool shouldShoot = true;

				for (CPosition _shot : game->GetPlayer(_beingAttacked).GetShots()) {
					if ((_hit.x + x == _shot.x && _hit.y + y == _shot.y) || (_hit.x + x > 9 || _hit.y + y > 9) || (_hit.x + x < 0 || _hit.y + y < 0)) shouldShoot = false;
				}

				if (shouldShoot) {
					betterShot = { _hit.x + x, _hit.y + y };
					Print({ 40,40 }, to_wstring(_hit.x + x) + L", " + to_wstring(_hit.y + y), 15);
					goto stopCheck;
				}

				spots.erase(spots.begin() + randInt);
			}
		}
	stopCheck:


		if (betterShot.x != -1 && betterShot.y != -1) {
			game->m_AImoves = 10;
			game->GetPlayer(_beingAttacked).SetCursor(betterShot);
		}
		else {
			game->GetPlayer(_beingAttacked).SetCursor({ rand() % 10,rand() % 10 });
			while ((game->GetPlayer(_beingAttacked).GetCursor().x % 2 == 0 && game->GetPlayer(_beingAttacked).GetCursor().y % 2 != 0) || (game->GetPlayer(_beingAttacked).GetCursor().x % 2 != 0 && game->GetPlayer(_beingAttacked).GetCursor().y % 2 == 0))
			{
				game->GetPlayer(_beingAttacked).SetCursor({ rand() % 10, rand() % 10 });
			}


			DrawBoard(game->GetPlayer(_beingAttacked));
		}
		game->m_AImoves++;

		
		for (CPosition _shot : game->GetPlayer(_beingAttacked).GetShots()) {
			if (_shot.x == game->GetPlayer(_beingAttacked).GetCursor().x && _shot.y == game->GetPlayer(_beingAttacked).GetCursor().y) game->m_AImoves--;
		}

		if (game->m_AImoves > 1) {
			game->m_AImoves = 0;
			bool theHit = game->GetPlayer(_beingAttacked).CalcShot(game->GetPlayer(_beingAttacked).GetCursor());
			game->UpdateBoards();
			DrawBoard(game->GetPlayer(_beingAttacked));
			Print({ game->GetPlayer(_beingAttacked).GetCursor().x * 3 + game->GetPlayer(_beingAttacked).GetBoardPos().x, game->GetPlayer(_beingAttacked).GetCursor().y * 3 - 1 + game->GetPlayer(_beingAttacked).GetBoardPos().y }, theHit ? L"HIT " : L"MISS", theHit ? 47 : 79);
			game->SwapPlayerTurn();
		}
	}
	

	DrawCursor(game->GetPlayer(_beingAttacked), 4);

	Sleep(100);
}

void SlowPrint(CPosition _pos, wstring _message, int effect, int _wait) {
	GotoXY(_pos);
	for (wchar_t _char : _message) {
		Sleep(_wait);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), effect);
		wcout << _char;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
}

bool CheckTempShip(CGame& game, CPosition& _startPos, CShip& _ship, int& _direction, int _player) {
	if ((_startPos.x + _ship.GetSegments().size() - 1 > 9 && _direction == 0) || (_startPos.y + _ship.GetSegments().size() - 1 > 9 && _direction == 1)) return false;
	for (int i = 0; i < _ship.GetSegments().size(); i++) {

		CPosition _tempPos;
		if (_direction == 0) {
			_tempPos = { _startPos.x + i, _startPos.y };
		}
		else {
			_tempPos = { _startPos.x, _startPos.y + i };
		}
		if (game.GetPlayer(_player).CheckHit(_tempPos)) return false;

	}
	return true;
}

void ManualPlace(CGame& game, vector<CShip>& toPlace) {
	for (CShip _ship : toPlace) {
		CPosition _startPos = CPosition(game.GetPlayer(0).GetCursor());
		int _direction = 0;
		int _placed = false;
		int _colour = 31;
		bool _update = true;
		bool _canPlace = true;

		//add some controls on the side, as well as display the ship Infortmation

		SlowPrint({ 40,5 }, L"Ship: " + _ship.GetName(), 15, 20);
		SlowPrint({ 40,6 }, L"Length: " + to_wstring(_ship.GetSegments().size()), 15, 15);
		SlowPrint({ 40,7 }, L"Symbol: " + _ship.GetIcon(), 15, 10);
		SlowPrint({ 40,8 }, L"Colour: ", 15, 5);
		SlowPrint({ 48,8 }, L"╬", _ship.GetColour(), 5);

		while (!_placed)
		{
			if (GetKeyState(VK_LEFT) & 0x8000) { game.GetPlayer(0).MoveCursorRight(-1); DrawBoard(game.GetPlayer(0)); _update = true; };
			if (GetKeyState(VK_RIGHT) & 0x8000) { game.GetPlayer(0).MoveCursorRight(1); DrawBoard(game.GetPlayer(0)); _update = true; };
			if (GetKeyState(VK_UP) & 0x8000) { game.GetPlayer(0).MoveCursorDown(-1); DrawBoard(game.GetPlayer(0)); _update = true; };
			if (GetKeyState(VK_DOWN) & 0x8000) { game.GetPlayer(0).MoveCursorDown(1); DrawBoard(game.GetPlayer(0)); _update = true; };
			if (GetKeyState('R') & 0x8000) {
				_direction = abs(_direction - 1);
				_update = true;
			}
			_canPlace = false;

			if (_update) {
				_startPos.x = game.GetPlayer(0).GetCursor().x;
				_startPos.y = game.GetPlayer(0).GetCursor().y;
			}

			if (CheckTempShip(game, _startPos, _ship, _direction, 0)) {
				_colour = 26;
				_canPlace = true;
			}
			else {
				_colour = 28;
			}

			if (GetKeyState(VK_SPACE) & 0x8000 && _canPlace == true) {
				_placed = true;
				game.GetPlayer(0).AddShip({ _startPos,static_cast<int>(_ship.GetSegments().size()),_direction,_ship.GetIcon(),_ship.GetColour() });
				game.UpdateBoards();

				DrawBoard(game.GetPlayer(0));
				continue;
			}
			else {
				if (_update) {
					_update = false;

					DrawBoard(game.GetPlayer(0));

					//DrawCursor(game.GetPlayer(0), 2);

					for (int i = 0; i < _ship.GetSegments().size(); i++) {

						if (_direction == 0) {
							if ((_startPos.x + i) > 9)  continue;
							Print({ game.GetPlayer(0).GetBoardPos().x + (_startPos.x + i) * 3, game.GetPlayer(0).GetBoardPos().y + (_startPos.y * 3) + 0 }, L"╭ ╮", _colour);
							Print({ game.GetPlayer(0).GetBoardPos().x + (_startPos.x + i) * 3, game.GetPlayer(0).GetBoardPos().y + (_startPos.y * 3) + 2 }, L"╰ ╯", _colour);
						}
						if (_direction == 1) {
							if ((_startPos.y + i) > 9)  continue;
							Print({ game.GetPlayer(0).GetBoardPos().x + _startPos.x * 3, game.GetPlayer(0).GetBoardPos().y + (_startPos.y + i) * 3 + 0 }, L"╭ ╮", _colour);
							Print({ game.GetPlayer(0).GetBoardPos().x + _startPos.x * 3, game.GetPlayer(0).GetBoardPos().y + (_startPos.y + i) * 3 + 2 }, L"╰ ╯", _colour);
						}
					}
				}


				Sleep(100);
			}

		}

		SlowPrint({ 40,5 }, L"Ship: " + _ship.GetName(), 0, 0);
		SlowPrint({ 40,6 }, L"Length: " + to_wstring(_ship.GetSegments().size()), 0, 0);
		SlowPrint({ 40,7 }, L"Symbol: " + _ship.GetIcon(), 0, 0);
		SlowPrint({ 40,8 }, L"Colour: ", 0, 0);
		SlowPrint({ 48,8 }, L"╬", 0, 0);

	}
}

void AutomaticPlace(CGame& game, vector<CShip>& toPlace, int _player) {
	for (CShip _ship : toPlace) {
		CPosition _startPos{ 0,0 };
		int _direction = 0;
		int _placed = false;
		bool _canPlace = false;

		//add some controls on the side, as well as display the ship Infortmation

		while (!_placed)
		{
			CPosition _startPos{ rand() % 10,rand() % 10 };
			int _direction = rand() % 2;

			if (CheckTempShip(game, _startPos, _ship, _direction, _player)) {
				_canPlace = true;
			}

			if (_canPlace == true) {
				_placed = true;
				game.GetPlayer(_player).AddShip({ _startPos,static_cast<int>(_ship.GetSegments().size()),_direction,_ship.GetIcon(),_ship.GetColour() });
				game.UpdateBoards();
				continue;
			}


		}


	}
}

void DrawBoard(CPlayer& player) {
	int iy = 0;
	Print({ player.GetBoardPos().x, player.GetBoardPos().y - 2 }, player.GetName() + L"'s Ships", 10);
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			if (x > 0 && x < 31 && y >0 && y < 31) continue;
			Print({ x + player.GetBoardPos().x - 1,y + player.GetBoardPos().y - 1 }, L" ", 119);
		}
	}
	for (vector<CSegment> y : player.m_board) {
		int ix = 0;
		for (CSegment x : y) {

			wstring spacer = L" ";
			int _colour = 15;
			if (x.GetHitState() == -2) {
				_colour = 23;
				spacer = L"┼";
			}
			else {
				_colour = (x.GetHitState() == 1 ? 28 : x.GetColour());
				spacer = (x.GetHitState() == 1 ? L"╳" : L" ");
			}


			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y }, spacer + spacer + spacer, _colour);
			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y + 1 }, spacer + x.GetIcon() + spacer, _colour);
			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y + 2 }, spacer + spacer + spacer, _colour);
			ix++;
		}
		iy++;
	}
}

void DrawCursor(CPlayer& player, int colour)
{
	int posColour = player.m_board[player.GetCursor().y][player.GetCursor().x].GetColour();
	int newColour = posColour - (posColour % 16) + colour;

	int xpos = player.GetCursor().x * 3 + player.GetBoardPos().x;
	int ypos = player.GetCursor().y * 3 + player.GetBoardPos().y;

	Print({ xpos, ypos }, L"╭", newColour);
	Print({ xpos + 1, ypos }, L"┬", newColour);
	Print({ xpos + 2, ypos + 2 }, L"╯", newColour);
	Print({ xpos + 1, ypos + 2 }, L"┴", newColour);
	Print({ xpos + 2, ypos }, L"╮", newColour);
	Print({ xpos + 2, ypos + 1 }, L"┤", newColour);
	Print({ xpos, ypos + 2 }, L"╰", newColour);
	Print({ xpos, ypos + 1 }, L"├", newColour);
}

#pragma region "Printing Functions"
//Used to print out text at the specified coordinate, with the specified effect.
void Print(CPosition pos, wstring str, int effect) {
	GotoXY(pos, 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), effect);
	wcout << str;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
};

//Used to move the Console Cursor to a point on the screen for more accurate text management.
void GotoXY(CPosition pos, int multi) {
	COORD point;
	point.X = (pos.x) * multi;
	point.Y = pos.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
};
#pragma endregion

/*
text rainbow

wstring val = L"0";
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 16; x++) {
			while (val.length() < 3) {
				val = L"0" + val;
			}
			Print({ y*4,x }, val, stoi(val));
			val = to_wstring(stoi(val) +1);
		}
	}
*/