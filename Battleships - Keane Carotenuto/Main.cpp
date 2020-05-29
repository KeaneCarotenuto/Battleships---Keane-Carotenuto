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

#include"CGame.h"
#include"CPosition.h"
#include"CSegment.h"
#include"CShip.h"
#include"CPlayer.h"


using namespace std;

void SetFont();

void Print(CPosition pos, wstring str, int effect);
void GotoXY(CPosition pos, int multi = 1);
void DrawBoard(CPlayer &player);
void DrawCursor(CPlayer& player, int colour);

void PlayerTurn(CGame *game, int _beingAttacked);

void BasicStuff(CGame &game) {
	game.AddPlayer({ L"James" });
	game.GetPlayer(0).AddShip({ {2,3},5,0,L"C",240 });
	game.GetPlayer(0).AddShip({ {4,5},3,1,L"D",192 });

	game.GetPlayer(0).CreateBoard();
	game.GetPlayer(0).SetBoardPos({ 5,5 });

	game.AddPlayer({ L"CPU" });
	game.GetPlayer(1).AddShip({ {1,1},3,1,L"S",144 });
	game.GetPlayer(1).AddShip({ {6,6},3,0,L"B",224 });

	game.GetPlayer(1).CreateBoard();
	game.GetPlayer(1).SetBoardPos({ 40,5 });

	
	game.UpdateBoards();

	DrawBoard(game.GetPlayer(0));
	DrawBoard(game.GetPlayer(1));
}


int main() {
	//Enable the use of Unicode
	(void)_setmode(_fileno(stdout), _O_U16TEXT);
	Print({ 0,0 }, L"BOOTING", 12);

	//Maximise the screen
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	
	//Set the buffer size to the size of the screen
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);
	csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
	csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
	SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hstdout, &info);

	srand(static_cast <unsigned> (time(0)));

	Sleep(1000);
	Print({ 0,0 }, L"COMPLETE", 10);


	SetFont();

	CGame game;

	bool created = false;
	
	game.AddPlayer({ L"MENU" });
	game.GetPlayer(0).AddShip({ {2,3},5,0,L"PLAY",240 });

	game.GetPlayer(0).CreateBoard();
	game.GetPlayer(0).SetBoardPos({ 5,5 });

	game.UpdateBoards();

	DrawBoard(game.GetPlayer(0));
	
	
	
	//game.GetPlayer(1).CheckHit({ 6,7 });


	//Need to create a better menu and what not. Make Ship placement too!
	while (game.IsRunning()) {
		switch (game.state)
		{
		case 0:
			if (GetKeyState(VK_LEFT) & 0x8000) { game.GetPlayer(0).MoveCursorRight(-1); DrawBoard(game.GetPlayer(0)); };
			if (GetKeyState(VK_RIGHT) & 0x8000) { game.GetPlayer(0).MoveCursorRight(1); DrawBoard(game.GetPlayer(0)); };
			if (GetKeyState(VK_UP) & 0x8000) { game.GetPlayer(0).MoveCursorDown(-1); DrawBoard(game.GetPlayer(0)); };
			if (GetKeyState(VK_DOWN) & 0x8000) { game.GetPlayer(0).MoveCursorDown(1); DrawBoard(game.GetPlayer(0)); };
			if (GetKeyState(VK_SPACE) & 0x8000) { 
				bool theHit = game.GetPlayer(0).CalcShot(game.GetPlayer(0).GetCursor());
				game.UpdateBoards();
				DrawBoard(game.GetPlayer(0));
				Print({ game.GetPlayer(0).GetCursor().x * 3 + game.GetPlayer(0).GetBoardPos().x, game.GetPlayer(0).GetCursor().y * 3 - 1 + game.GetPlayer(0).GetBoardPos().y }, theHit ? L"HIT " : L"MISS", theHit ? 47 : 79);
				
				if (theHit) {
					game.RemovePlayer(0);
					Sleep(1000);
					game.state = 1;
				}
			}
			else {
				DrawCursor(game.GetPlayer(0), 2);

				Sleep(100);
			}
			
			break;

		case 1:
			if (!created) BasicStuff(game); created = true;
			game.state = 2;
			break;

		case 2:
			if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('D') & 0x8000) { game.ToggleDebug(); game.GetPlayer(1).ResetBoard(); game.UpdateBoards(); DrawBoard(game.GetPlayer(1)); };
			PlayerTurn(&game, game.GetPlayerTurn());
			break;

		case 3:
			break;

		case 4:
			break;

		default:
			break;
		}
		
	}

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

void DrawBoard(CPlayer &player) {
	int iy = 0;
	Print({ player.GetBoardPos().x, player.GetBoardPos().y -2 }, player.GetName() + L"'s Ships", 10);
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			if (x > 0 && x < 31 && y >0 && y < 31) continue;
			Print({x + player.GetBoardPos().x -1,y + player.GetBoardPos().y -1}, L" ", 119);
		}
	}
	for (vector<CSegment> y : player.m_board) {
		int ix = 0;
		for (CSegment x : y) {
			//if (x.GetHitState() == 0) continue;

			//Do some stuff to make it show missed hits in the water (hit state -2? store hits in vector?)
			//Also clean up code a lot
			//Also make AI not stupid and fire around hit targets
			//add boarder around ships
			//

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
			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y +1 }, spacer + x.GetIcon() + spacer, _colour);
			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y +2 }, spacer + spacer + spacer, _colour);
			ix++;
		}
		iy++;
	}
}

void DrawCursor(CPlayer& player, int colour)
{
	int posColour = player.m_board[player.GetCursor().y][player.GetCursor().x].GetColour();
	int newColour = posColour - (posColour % 16) + colour;

	int xpos = player.GetCursor().x*3 + player.GetBoardPos().x;
	int ypos = player.GetCursor().y*3 + player.GetBoardPos().y;

	Print({ xpos, ypos }, L"╭", newColour);
	Print({ xpos+1, ypos }, L"┬", newColour);
	Print({ xpos+2, ypos+2 }, L"╯", newColour);
	Print({ xpos+1, ypos+2 }, L"┴", newColour);
	Print({ xpos+2, ypos }, L"╮", newColour);
	Print({ xpos+2, ypos + 1 }, L"┤", newColour);
	Print({ xpos, ypos + 2 }, L"╰", newColour);
	Print({ xpos, ypos + 1 }, L"├", newColour);
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
					if ((_hit.x + x == _shot.x && _hit.y + y == _shot.y) || abs(x) == abs(y)) shouldShoot = false;
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
			game->GetPlayer(_beingAttacked).SetCursor({ rand() % 9,rand() % 9 });
			while ((game->GetPlayer(_beingAttacked).GetCursor().x % 2 == 0 && game->GetPlayer(_beingAttacked).GetCursor().y % 2 != 0) || (game->GetPlayer(_beingAttacked).GetCursor().x % 2 != 0 && game->GetPlayer(_beingAttacked).GetCursor().y % 2 == 0))
			{
				game->GetPlayer(_beingAttacked).SetCursor({ rand() % 9, rand() % 9 });
			}


			DrawBoard(game->GetPlayer(_beingAttacked));
		}
		game->m_AImoves++;

		
		for (CPosition _shot : game->GetPlayer(_beingAttacked).GetShots()) {
			if (_shot.x == game->GetPlayer(_beingAttacked).GetCursor().x && _shot.y == game->GetPlayer(_beingAttacked).GetCursor().y) game->m_AImoves--;
		}

		if (game->m_AImoves > 5) {
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