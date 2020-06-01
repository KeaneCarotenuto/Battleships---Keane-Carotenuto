/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : CPlayer.h
Description : This class is used to store the players ships and boards, as well as manage shots
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

#pragma once
#include <string>
#include <vector>
#include"CShip.h"

using namespace std;

class CPlayer
{
protected:
	wstring m_name;
	vector<CShip> m_ships;
	vector<CPosition> m_shotsAt;
	vector<CPosition> m_missesAt;
	vector<CPosition> m_hitsAt;
	CPosition m_boardPos;
	CPosition m_cursor;

public:
	//Basic Constructor
	CPlayer(wstring _name);

	//The player's board of ships
	vector<vector<CSegment>> m_board;

	void CreateBoard();
	void ResetBoard();

	void SetName(wstring _newName);
	wstring GetName();

	void SetBoardPos(CPosition _pos);
	CPosition GetBoardPos();

	//Manages player ships
	void SetShips(vector<CShip> _newShips);
	vector<CShip> &GetShips();
	void AddShip(CShip _newShip);
	CShip &GetShip(int _index);
	void RemoveShip(int _index);

	//Manages shots/shooting against this player
	void AddShot(CPosition _pos);
	vector<CPosition> GetShots();

	void AddMiss(CPosition _pos);
	vector<CPosition> GetMisses();

	void AddHit(CPosition _pos);
	vector<CPosition> GetHits();

	bool CheckHit(CPosition _pos);
	bool CalcShot(CPosition _pos);

	void SetCursor(CPosition _pos);
	CPosition GetCursor();
	void MoveCursorRight(int _x);
	void MoveCursorDown(int _y);

	//Checks if two players are equal
	bool CheckSame(CPlayer& _player);
	
};

