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
	CPlayer(wstring _name);

	vector<vector<CSegment>> m_board;

	void CreateBoard();
	void ResetBoard();

	void SetName(wstring _newName);
	wstring GetName();

	void SetBoardPos(CPosition _pos);
	CPosition GetBoardPos();

	void SetShips(vector<CShip> _newShips);
	vector<CShip> &GetShips();
	void AddShip(CShip _newShip);
	CShip &GetShip(int _index);
	void RemoveShip(int _index);

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

	bool CheckSame(CPlayer& _player);
	
};

