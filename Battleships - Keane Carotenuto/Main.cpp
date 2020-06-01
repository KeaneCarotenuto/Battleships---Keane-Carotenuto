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

#pragma region "Classes and Libraries"
//I enclude all of the required classes and libraries

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

#include"CGame.h"																											// TF: Class
																															// TF: Access Specifier (Inside header File)
#include"CPosition.h"
#include"CSegment.h"
#include"CShip.h"
#include"CPlayer.h"

using namespace std;
#pragma endregion

#pragma region "Declaring Functions"
//Declaring all of my Functions

//Initialisation Funcs
void SetupGame();
void SetFont();

//Main Game Loop Funcs
wstring Login();																											// TF: Variable Type
void SetUpMenu(CGame* game);
void MenuControls(CGame* game);
void ShipPlacement(CGame* game, wstring _userName);
void PlayGame(CGame* game, bool &endAfter);
void EndGame(CGame* game, int _endVal);

//Sub Game Loop Funcs
void PlayerTurn(CGame *game, int _beingAttacked);
void AutomaticPlace(CGame& game, const CShip(&toPlace)[5], int _player);													// TF: Constant
void ManualPlace(CGame& game, const CShip(&toPlace)[5]);																	// TF: Constant
bool CheckTempShip(CGame& game, CPosition& _startPos, CShip& _ship, int& _direction, int _player);							// TF: Variable Type

//Drawing Funcs
void DrawBoard(CPlayer& player);
void DrawCursor(CPlayer& player, int colour);
void DisplayControls(CPosition _pos, int _type);
void Print(CPosition pos, wstring str, int effect = 15);																	// TF: Default Parameter
void SlowPrint(CPosition _pos, wstring _message, int effect = 15, int _wait = 20);											// TF: Default Parameter
void GotoXY(CPosition pos);

#pragma endregion

#pragma region "Main Loop"
//The Main game loop

int main() {
	SetupGame();

	//Wait for Game to Boot
	Sleep(1000);
	
	//Get User's Name
	wstring userName = Login();

	Sleep(1000);

	system("CLS");

	SetFont();

	//Create Main Game Class Instance
	CGame *game = new CGame();																								// TF: Pointer Initialised
																															// TF: Dynamic Memory
																															// TF: Class Instance

	SetUpMenu(game);
	
	bool endAfter = false;																									 // TF: Variable Type

	while (game->IsRunning()) {
		switch (game->state)																								// TF: Pointer Dereferenced
		{
		case 0:
			//Displays the Menu
			MenuControls(game);
			break;

		case 1: {
			//Starts the Ship Placement Section
			ShipPlacement(game, userName);
			break;	
		}

		case 2:
			//The actual Shooting Part
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

	//Removes game From Dynamic Memory and Waits for user to press Enter to Exit
	delete game;																											// TF: Dynamic Memory
																															// TF: Destructor
	game = nullptr;

	Print({0,0},L"Press <ENTER> to Exit",12);
	getchar();

	return 0;
}

#pragma endregion

#pragma region "Setup Functions"
//These Functions are used to set up the game before it starts

//Changes the font of the Console
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

//Changes The console size etc
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
	srand(static_cast <unsigned> (time(0)));																				// TF: Pseudo Random Number
}

#pragma endregion

#pragma region "Main Game"
//The Functions that Actually Control most of the gameplay

//A simple Login screen with a theme, really to just get name of player
wstring Login()
{
	wstring tempName;																										// TF: Variable Type
	SlowPrint({ 0,0 }, L"[COMPLETE] BATTLESHIP SYSTEM ONLINE", 10, 30);

	SlowPrint({ 2,2 }, L"ENTER DEFENCE FORCE LOGIN", 12, 30);

	SlowPrint({ 4,4 }, L"ENTER NAME: ");
	getline(wcin, tempName);

	SlowPrint({ 4,6 }, L"ENTER Password: ");

	while (true) {
		if (_kbhit()) {																										// TF: Conditional Statement
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

//Creates the Board for the interactive Menu
void SetUpMenu(CGame* game)
{
	//Creates temporary Menu Player
	game->AddPlayer({ L"MENU" });
	
	//Adds Ships as Buttons
	game->GetPlayer(0).AddShip({ {2,2},1,0,L"B",240 });																		// TF: Constructor
	game->GetPlayer(0).AddShip({ {3,2},1,0,L"A",240 });
	game->GetPlayer(0).AddShip({ {4,2},1,0,L"T",240 });
	game->GetPlayer(0).AddShip({ {5,2},1,0,L"T",240 });
	game->GetPlayer(0).AddShip({ {6,2},1,0,L"L",240 });
	game->GetPlayer(0).AddShip({ {7,2},1,0,L"E",240 });

	game->GetPlayer(0).AddShip({ {3,8},1,0,L"Q",240 });
	game->GetPlayer(0).AddShip({ {4,8},1,0,L"U",240 });
	game->GetPlayer(0).AddShip({ {5,8},1,0,L"I",240 });
	game->GetPlayer(0).AddShip({ {6,8},1,0,L"T",240 });

	//Creates Board With Ships
	game->GetPlayer(0).CreateBoard();
	game->GetPlayer(0).SetBoardPos({ 5,5 });
	game->UpdateBoards();

	DrawBoard(game->GetPlayer(0));																							// TF: Reference

	DisplayControls({ 40, 5 }, 3);
}

//Controls how the user may interact with the Menu
void MenuControls(CGame* game)
{
	//Moves Cursor
	if (GetKeyState(VK_LEFT) & 0x8000) { game->GetPlayer(0).MoveCursorRight(-1); DrawBoard(game->GetPlayer(0)); };
	if (GetKeyState(VK_RIGHT) & 0x8000) { game->GetPlayer(0).MoveCursorRight(1); DrawBoard(game->GetPlayer(0)); };
	if (GetKeyState(VK_UP) & 0x8000) { game->GetPlayer(0).MoveCursorDown(-1); DrawBoard(game->GetPlayer(0)); };
	if (GetKeyState(VK_DOWN) & 0x8000) { game->GetPlayer(0).MoveCursorDown(1); DrawBoard(game->GetPlayer(0)); };

	//Checks if player Selects Battle Or Quit
	if (GetKeyState(VK_SPACE) & 0x8000) {
		bool theHit = game->GetPlayer(0).CalcShot(game->GetPlayer(0).GetCursor());
		game->UpdateBoards();
		DrawBoard(game->GetPlayer(0));
		Print({ game->GetPlayer(0).GetCursor().x * 3 + game->GetPlayer(0).GetBoardPos().x,									// TF: Arithmetic Operator
		game->GetPlayer(0).GetCursor().y * 3 - 1 + game->GetPlayer(0).GetBoardPos().y },									// TF: Arithmetic Operator
			theHit ? L"HIT " : L"MISS",
			theHit ? 47 : 79);

		if (theHit && game->GetPlayer(0).GetCursor().y == 2) {																// TF: Logical Operator
			game->GetPlayer(0).RemoveShip(0);

			game->RemovePlayer(0);

			Sleep(500);
			game->state = 1;
		}
		else if (theHit && game->GetPlayer(0).GetCursor().y == 8) {															// TF: Logical Operator
			game->StopGame();
			return;
		}
	}
	else {
		DrawCursor(game->GetPlayer(0), 2);

		Sleep(100);
	}
}

//Manages The Placements of the Ships
void ShipPlacement(CGame* game, wstring _userName)
{
	//Creates an array of the Ships that need to be placed
	const CShip toPlace[5]{																							// TF: Constant
				{ {0,0},5,0,L"A",240 },
				{ {0,0},4,0,L"B",224 },
				{ {0,0},3,0,L"D",192 },
				{ {0,0},3,0,L"S",144 },
				{ {0,0},2,0,L"P",176 }
	};																												// TF: Array

	//Creates Players
	game->AddPlayer(_userName);

	game->GetPlayer(0).CreateBoard();
	game->GetPlayer(0).SetBoardPos({ 5,5 });

	game->AddPlayer({ L"CPU" });																					// TF: Constructor

	game->GetPlayer(1).CreateBoard();
	game->GetPlayer(1).SetBoardPos({ 40,5 });

	//Places Enemy Ships
	AutomaticPlace(*game, toPlace, 1);

	system("CLS");

	//Asks user what Placement Method they want to use
	SlowPrint({ 2,2 }, L"Select Ship Deployment Mode:", 15, 20);

	vector<int> colours{ 10,15 };

	SlowPrint({ 3,4 }, L"Manual", colours[0]);
	SlowPrint({ 3,5 }, L"Automatic", colours[1]);

	DisplayControls({ 40, 5 }, 2);

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

	//Checks which method the player chose and goes to that func
	if (colours[0] == 10) {
		DisplayControls({ 40, 15 }, 4);
		ManualPlace(*game, toPlace);
	}
	else {
		AutomaticPlace(*game, toPlace, 0);
	}

	system("CLS");

	DisplayControls({ 5, 40 }, 5);

	//Hides Enemy Ships
	game->SetDebug(false);

	game->GetPlayer(1).ResetBoard(); game->UpdateBoards();

	DrawBoard(game->GetPlayer(1));

	game->state = 2;
}

//Manages Users shooting at eachother
void PlayGame(CGame* game, bool &endAfter)
{
	//Checks for Win/Loss/Tie
	if (game->GetPlayer(1).GetHits().size() >= 17) {																		// TF: Relational Operator
		if (endAfter) {
			game->state = 5;
			return;
		}
		game->state = 4;
		return;
	}
	if (game->GetPlayer(0).GetHits().size() >= 17) {
		if (endAfter && game->GetPlayerTurn() == 0) {																		// TF: Relational Operator
			game->state = 3;
		}
		else {
			endAfter = true;
			game->SetPlayerTurn(1);
		}
	}

	//Manages users shooting and turns
	if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('D') & 0x8000) { game->ToggleDebug(); game->GetPlayer(1).ResetBoard(); game->UpdateBoards(); DrawBoard(game->GetPlayer(1)); };
	PlayerTurn(game, game->GetPlayerTurn());
}

//Manages What happens when the game ends
void EndGame(CGame* game, int _endVal)
{
	system("CLS");


	if (_endVal == 1) {
		//Loss
		SlowPrint({ 10,4 }, L"██████  ███████ ███████ ███████  █████  ████████ ", 12, 1);
		SlowPrint({ 10,5 }, L"██   ██ ██      ██      ██      ██   ██    ██    ", 12, 1);
		SlowPrint({ 10,6 }, L"██   ██ █████   █████   █████   ███████    ██    ", 12, 1);
		SlowPrint({ 10,7 }, L"██   ██ ██      ██      ██      ██   ██    ██    ", 12, 1);
		SlowPrint({ 10,8 }, L"██████  ███████ ██      ███████ ██   ██    ██    ", 12, 1);

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

		SlowPrint({ 10,4 }, L"██████  ██████   █████  ██     ██ ", 15, 1);
		SlowPrint({ 10,5 }, L"██   ██ ██   ██ ██   ██ ██     ██ ", 15, 1);
		SlowPrint({ 10,6 }, L"██   ██ ██████  ███████ ██  █  ██ ", 15, 1);
		SlowPrint({ 10,7 }, L"██   ██ ██   ██ ██   ██ ██ ███ ██ ", 15, 1);
		SlowPrint({ 10,8 }, L"██████  ██   ██ ██   ██  ███ ███  ", 15, 1);

	}


	//Displays stats
	SlowPrint({5,12}, game->GetPlayer(0).GetName());
	SlowPrint({ 5,14 }, L"Shots:  " + to_wstring(game->GetPlayer(1).GetShots().size()));
	SlowPrint({ 5,15 }, L"Hits:   " + to_wstring(game->GetPlayer(1).GetHits().size()));
	SlowPrint({ 5,16 }, L"Misses: " + to_wstring(game->GetPlayer(1).GetShots().size() - game->GetPlayer(1).GetHits().size()));

	SlowPrint({ 20,12 }, game->GetPlayer(1).GetName());
	SlowPrint({ 20,14 }, L"Shots:  " + to_wstring(game->GetPlayer(0).GetShots().size()));
	SlowPrint({ 20,15 }, L"Hits:   " + to_wstring(game->GetPlayer(0).GetHits().size()));
	SlowPrint({ 20,16 }, L"Misses: " + to_wstring(game->GetPlayer(0).GetShots().size() - game->GetPlayer(0).GetHits().size()));

	//Waits for user to press enter, then resets the whole gameand goes to menu
	Print({ 10,20 }, L"Press <ENTER> to Return To Menu", 12);
	while (true) {
		if (GetKeyState(VK_RETURN) & 0x8000) {
			system("CLS");
			game->RemovePlayer(1);
			game->RemovePlayer(0);
			game->SetDebug(true);
			SetUpMenu(game);
			game->state = 0;
			return;
		}
	}
}

#pragma endregion

#pragma region "Sub Game"
//The Functions that the main game loop uses to for some tasks

//Lets the players shoot at eachother
void PlayerTurn(CGame *game, int _beingAttacked)
{
	//If The User is Attacking the CPU
	if (_beingAttacked == 1) {
		//Draws Indicator
		Print({ 4,36 },  L"╙██████████████████████████████╜",0);
		Print({ 39,36 }, L"╙██████████████████████████████╜", 10);

		//Manages Cursor Movement
		if (GetKeyState(VK_LEFT) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorRight(-1); DrawBoard(game->GetPlayer(_beingAttacked)); };
		if (GetKeyState(VK_RIGHT) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorRight(1); DrawBoard(game->GetPlayer(_beingAttacked)); };
		if (GetKeyState(VK_UP) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorDown(-1); DrawBoard(game->GetPlayer(_beingAttacked)); };
		if (GetKeyState(VK_DOWN) & 0x8000) { game->GetPlayer(_beingAttacked).MoveCursorDown(1); DrawBoard(game->GetPlayer(_beingAttacked)); };

		//Shoots and manages the shot result
		if (GetKeyState(VK_SPACE) & 0x8000) {
			game->m_AImoves = 0;
			bool theHit = game->GetPlayer(_beingAttacked).CalcShot(game->GetPlayer(_beingAttacked).GetCursor());
			game->UpdateBoards();
			DrawBoard(game->GetPlayer(_beingAttacked));
			Print({ game->GetPlayer(_beingAttacked).GetCursor().x * 3 + game->GetPlayer(_beingAttacked).GetBoardPos().x, game->GetPlayer(_beingAttacked).GetCursor().y * 3 - 1 + game->GetPlayer(_beingAttacked).GetBoardPos().y }, theHit ? L"HIT " : L"MISS", theHit ? 47 : 79);
			
			int tempi = 5;
			for (wstring _wstr : game->m_allSunk) {
				Print({ 75, tempi }, _wstr, 0);
					tempi++;
			}
			game->m_allSunk.clear();
			for (CPlayer _player : game->GetPlayers()) {
				for (CShip _ship : _player.GetShips()) {
					bool sunk = true;
					for (CSegment _segment : _ship.GetSegments()) {
						if (_segment.GetHitState() == 0) {
							sunk = false;
							break;
						}
					}
					if (sunk) {
						game->m_allSunk.push_back(_player.GetName() + L"'s " + _ship.GetName() + L" SUNK");
					}
				}
			}
			tempi = 5;
			
			for (wstring _wstr : game->m_allSunk) {
				Print({75, tempi}, _wstr, (_wstr.find(L"CPU's") ? 12 : 10));
				tempi++;
			}
			game->SwapPlayerTurn();
		};
	}
	//If the CPU is attacking the User
	else {
		//Draws Indicator
		Print({ 4,36 },  L"╙██████████████████████████████╜", 12);
		Print({ 39,36 }, L"╙██████████████████████████████╜", 0);

		//Basically the CPU AI
		CPosition betterShot = { -1,-1 };
		
		//For each successful hit
		for (CPosition _hit : game->GetPlayer(_beingAttacked).GetHits()) {
			
			//Create array of Spots to check around the hit
			vector<CPosition> spots { {0, -1}, { 0, 1 }, { -1, 0 }, { 1, 0 } };
			
			//Loops through each spot
			while (spots.size() > 0) {																						// TF: Conditional Statement
				int randInt = (rand() % spots.size());																		// TF: Arithmetic Operator
																															// TF: Pseudo Random Number
				int x = spots[randInt].x;
				int y = spots[randInt].y;

				bool shouldShoot = true;

				//Checks if the spot is off the board, or if there has already been a shot there
				//This is 
				for (CPosition _shot : game->GetPlayer(_beingAttacked).GetShots()) {
					if ((_hit.x + x == _shot.x && _hit.y + y == _shot.y) || (_hit.x + x > 9 || _hit.y + y > 9) || (_hit.x + x < 0 || _hit.y + y < 0)) {
						shouldShoot = false;
						break;
					}
				}

				//If there is a shot to be taken that is adjacent to the hit, it will take it
				if (shouldShoot) {
					betterShot = { _hit.x + x, _hit.y + y };
					Print({ 40,40 }, to_wstring(_hit.x + x) + L", " + to_wstring(_hit.y + y), 15);
					goto stopCheck;
				}

				//removes the spot used form the list
				spots.erase(spots.begin() + randInt);
			}
		}
	stopCheck:

		//If there is a better shot, it takes it
		if (betterShot.x != -1 && betterShot.y != -1) {																		// TF: Relational Operator
			game->m_AImoves = 10;
			game->GetPlayer(_beingAttacked).SetCursor(betterShot);
		}
		//Otherwise It chooses a random spot
		else {
			//The code only fires in every second spot, as this reduces the total shots, and you can never miss a ship, making it optimal
			game->GetPlayer(_beingAttacked).SetCursor({ rand() % 10,rand() % 10 });											// TF: Arithmetic Operator
			while ((game->GetPlayer(_beingAttacked).GetCursor().x % 2 == 0 && game->GetPlayer(_beingAttacked).GetCursor().y % 2 != 0) || (game->GetPlayer(_beingAttacked).GetCursor().x % 2 != 0 && game->GetPlayer(_beingAttacked).GetCursor().y % 2 == 0))
			{
				game->GetPlayer(_beingAttacked).SetCursor({ rand() % 10, rand() % 10 });									// TF: Pseudo Random Number
			}


			DrawBoard(game->GetPlayer(_beingAttacked));
		}
		game->m_AImoves++;

		//Checks to see if the random spot has already been shot at, if so, makes the CPU choose a new spot (can be quite long end game)
		for (CPosition _shot : game->GetPlayer(_beingAttacked).GetShots()) {												// TF: Iteration Structure
			if (_shot.x == game->GetPlayer(_beingAttacked).GetCursor().x && _shot.y == game->GetPlayer(_beingAttacked).GetCursor().y) game->m_AImoves--;
		}

		//If the spot chosen is good, it fires there
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

//Manages the Automatic Placement of the Ships
void AutomaticPlace(CGame& game, const CShip(&toPlace)[5], int _player) {													// TF: Array
	//For each ship that needs to be placed
	for (CShip _ship : toPlace) {
		CPosition _startPos{ 0,0 };
		int _direction = 0;
		int _placed = false;
		bool _canPlace = false;

		//add some controls on the side, as well as display the ship Infortmation

		//keeps looking for a good spot to place
		while (!_placed)
		{
			CPosition _startPos{ rand() % 10,rand() % 10 };																	// TF: Arithmetic Operator
			int _direction = rand() % 2;																					// TF: Pseudo Random Number

			//Checks if spot is bad
			if (CheckTempShip(game, _startPos, _ship, _direction, _player)) {
				_canPlace = true;
			}

			//If spot is good, places it and moves on to next ship to place
			if (_canPlace == true) {
				_placed = true;
				game.GetPlayer(_player).AddShip({ _startPos,static_cast<int>(_ship.GetSegments().size()),_direction,_ship.GetIcon(),_ship.GetColour() });
				game.UpdateBoards();
				continue;
			}


		}


	}
}

//Manages the user placing thier own ships
void ManualPlace(CGame& game, const CShip (&toPlace)[5]) {																	// TF: Array
	//For each ship that needs to be placed
	for (CShip _ship : toPlace) {
		CPosition _startPos = CPosition(game.GetPlayer(0).GetCursor());														// TF: Copy Constructor
		int _direction = 0;
		int _placed = false;
		int _colour = 31;
		bool _update = true;
		bool _canPlace = true;

		//Shows infortmation of ship being placed
		SlowPrint({ 40,5 }, L"Ship: " + _ship.GetName(), 15, 20);
		SlowPrint({ 40,6 }, L"Length: " + to_wstring(_ship.GetSegments().size()), 15, 15);
		SlowPrint({ 40,7 }, L"Symbol: " + _ship.GetIcon(), 15, 10);
		SlowPrint({ 40,8 }, L"Colour: ", 15, 5);
		SlowPrint({ 48,8 }, L"╬", _ship.GetColour(), 5);

		//While the ship hasnt been placed
		while (!_placed)																									// TF: Logical Operator
		{
			//manages movement of the target position
			if (GetKeyState(VK_LEFT) & 0x8000) { game.GetPlayer(0).MoveCursorRight(-1); DrawBoard(game.GetPlayer(0)); _update = true; };
			if (GetKeyState(VK_RIGHT) & 0x8000) { game.GetPlayer(0).MoveCursorRight(1); DrawBoard(game.GetPlayer(0)); _update = true; };
			if (GetKeyState(VK_UP) & 0x8000) { game.GetPlayer(0).MoveCursorDown(-1); DrawBoard(game.GetPlayer(0)); _update = true; };
			if (GetKeyState(VK_DOWN) & 0x8000) { game.GetPlayer(0).MoveCursorDown(1); DrawBoard(game.GetPlayer(0)); _update = true; };

			//Rotates targe position
			if (GetKeyState('R') & 0x8000) {
				_direction = abs(_direction - 1);
				_update = true;
			}
			_canPlace = false;

			//Updates taget pos
			if (_update) {
				_startPos.x = game.GetPlayer(0).GetCursor().x;
				_startPos.y = game.GetPlayer(0).GetCursor().y;
			}

			//Checks if the ship can be placed in that spot
			if (CheckTempShip(game, _startPos, _ship, _direction, 0)) {
				_colour = 26;
				_canPlace = true;
			}
			else {
				_colour = 28;
			}

			//Places Target Ship if it can be then moves on
			if (GetKeyState(VK_SPACE) & 0x8000 && _canPlace == true) {
				_placed = true;
				//Adds ship to player
				game.GetPlayer(0).AddShip({ _startPos,static_cast<int>(_ship.GetSegments().size()),_direction,_ship.GetIcon(),_ship.GetColour() });
				game.UpdateBoards();

				DrawBoard(game.GetPlayer(0));
				continue;
			}

			//if the user isnt placing the ship, it updates the position and colour of the taget position outline
			else {
				if (_update) {
					_update = false;

					DrawBoard(game.GetPlayer(0));

					//DrawCursor(game.GetPlayer(0), 2);

					for (int i = 0; i < static_cast<int>(_ship.GetSegments().size()); i++) {

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

		//Removes the Ship Info for the next info
		SlowPrint({ 40,5 }, L"Ship: " + _ship.GetName(), 0, 0);
		SlowPrint({ 40,6 }, L"Length: " + to_wstring(_ship.GetSegments().size()), 0, 0);
		SlowPrint({ 40,7 }, L"Symbol: " + _ship.GetIcon(), 0, 0);
		SlowPrint({ 40,8 }, L"Colour: ", 0, 0);
		SlowPrint({ 48,8 }, L"╬", 0, 0);

	}
}

//Checks if the target position and length given is open space or is occupied
bool CheckTempShip(CGame& game, CPosition& _startPos, CShip& _ship, int& _direction, int _player) {
	//if out of bounds
	if ((_startPos.x + _ship.GetSegments().size() - 1 > 9 && _direction == 0) || (_startPos.y + _ship.GetSegments().size() - 1 > 9 && _direction == 1)) return false;

	//If Colliding with another ship
	for (int i = 0; i < static_cast<int>(_ship.GetSegments().size()); i++) {																	// TF: Iteration Structure

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

#pragma endregion

#pragma region "Drawing Functions"
//Functions responsible for Drawing up things on the screen

//Drawing the Player's Boards on screen
void DrawBoard(CPlayer& player) {																							// TF: Reference
	int iy = 0;
	//Print Name
	Print({ player.GetBoardPos().x, player.GetBoardPos().y - 2 }, player.GetName() + L"'s Ships", 10);						// TF: Reference
	
																															//Print Boarder around board
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			if (x > 0 && x < 31 && y >0 && y < 31) continue;
			Print({ x + player.GetBoardPos().x - 1,y + player.GetBoardPos().y - 1 }, L" ", 119);
		}
	}
	
	//Prints Ships and water
	for (vector<CSegment> y : player.m_board) {
		int ix = 0;
		for (CSegment x : y) {

			//Prints Appropriate symbols and colours of board spaces
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

			//prints each segment
			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y }, spacer + spacer + spacer, _colour);
			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y + 1 }, spacer + x.GetIcon() + spacer, _colour);
			Print({ ix * 3 + player.GetBoardPos().x ,iy * 3 + player.GetBoardPos().y + 2 }, spacer + spacer + spacer, _colour);
			ix++;
		}
		iy++;
	}
}

//Draws the specified player's Cursor
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

//Manages which Controls to display
void DisplayControls(CPosition _pos, int _type)
{
	int x = _pos.x;																											// TF: Variable Type
	int y = _pos.y;																											// TF: Variable Type

	Print({ x + 0, y + 0 }, L"CONTROLS:", 15);																				// TF: Arithmetic Operator

	if (_type == 1) {																										// TF: Relational Operator
		Print({ x + 1, y + 2 }, L"Text Menus:", 7);
		Print({ x + 2, y + 3 }, L"Use <UP> and <DOWN> arrow keys to change Selection", 8);
		Print({ x + 2, y + 4 }, L"Press <ENTER> to Confirm Selection", 8);

		Print({ x + 1, y + 6 }, L"On Game Board:", 7);
		Print({ x + 2, y + 7 }, L"Use <ARROW KEYS> to Move Aim Reticle", 8);
		Print({ x + 2, y + 8 }, L"Press <SPACE> to fire at Targeted Position", 8);
	}

	if (_type == 2) {
		Print({ x + 1, y + 2 }, L"Text Menus:", 7);
		Print({ x + 2, y + 3 }, L"Use <UP> and <DOWN> arrow keys to change Selection", 8);
		Print({ x + 2, y + 4 }, L"Press <ENTER> to Confirm Selection", 8);
	}

	if (_type == 3 || _type == 5) {																							// TF: Logical Operator
		Print({ x + 1, y + 2 }, L"On Game Board:", 7);
		Print({ x + 2, y + 3 }, L"Use <ARROW KEYS> to Move Aim Reticle", 8);
		Print({ x + 2, y + 4 }, L"Press <SPACE> to fire at Targeted Position", 8);
	}

	if (_type == 4) {
		Print({ x + 1, y + 2 }, L"Ship Placement:", 7);
		Print({ x + 2, y + 3 }, L"Use <ARROW KEYS> to Move Target Location", 8);
		Print({ x + 2, y + 4 }, L"Use <R> to Rotate Target Location", 8);
		Print({ x + 2, y + 5 }, L"Press <SPACE> to Place Ship at Target Location", 8);
	}

	if (_type == 5) {
		Print({ x + 2, y + 5 }, L"Press <CTRL>+<D> to Enable Counter Intelligence*", 8);
		Print({ x + 2, y + 6 }, L"                          *(Debug Mode)", 8);
	}

}

//Used to print out text at the specified coordinate, with the specified effect.
void Print(CPosition pos, wstring str, int effect) {
	GotoXY(pos);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), effect);
	wcout << str;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
};

//Prints text at coord with colour one letter at a time
void SlowPrint(CPosition _pos, wstring _message, int effect, int _wait) {
	GotoXY(_pos);
	for (wchar_t _char : _message) {																						// TF: Iteration Structure
		Sleep(_wait);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), effect);
		wcout << _char;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
}

//Used to move the Console Cursor to a point on the screen for more accurate text management.
void GotoXY(CPosition pos) {
	COORD point;
	point.X = pos.x;
	point.Y = pos.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
};
#pragma endregion