/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : CPlayer.cpp
Description : The functions for the Player class.
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

#include "CPlayer.h"
#include <algorithm>
#include <cmath>

using namespace std;

CPlayer::CPlayer(wstring _name) :m_name(_name), m_cursor({1,1})
{
}

void CPlayer::SetName(wstring _newName)
{
	m_name = _newName;
}

wstring CPlayer::GetName()
{
	return m_name;
}

void CPlayer::SetBoardPos(CPosition _pos)
{
	m_boardPos = _pos;
}

CPosition CPlayer::GetBoardPos()
{
	return m_boardPos;
}

//Resets the board to be just water
void CPlayer::ResetBoard()
{
	for (int y = 0; y < static_cast<int>(m_board.size()); y++) {
		for (int x = 0; x < static_cast<int>(m_board[0].size()); x++) {
			m_board[y][x] = { -1 };
		}
	}
}

void CPlayer::CreateBoard()
{
	m_board.assign(10, vector < CSegment >(10, { -1 }));
}

void CPlayer::SetShips(vector<CShip> _newShips)
{
	m_ships = _newShips;
}

vector<CShip> &CPlayer::GetShips()
{
	return m_ships;
}

void CPlayer::AddShip(CShip _newShip)
{
	m_ships.push_back(_newShip);
}

CShip &CPlayer::GetShip(int _index)
{
	return m_ships[_index];
}

void CPlayer::RemoveShip(int _index)
{
	m_ships.erase(m_ships.begin() + _index);
}

void CPlayer::AddShot(CPosition _pos)
{
	m_shotsAt.push_back(_pos);
}

vector<CPosition> CPlayer::GetShots()
{
	return m_shotsAt;
}

void CPlayer::AddMiss(CPosition _pos)
{
	m_missesAt.push_back(_pos);
}

vector<CPosition> CPlayer::GetMisses()
{
	return m_missesAt;
}

void CPlayer::AddHit(CPosition _pos)
{
	m_hitsAt.push_back(_pos);
}

vector<CPosition> CPlayer::GetHits()
{
	return m_hitsAt;
}

//Checks if a given position contains a ship segment
bool CPlayer::CheckHit(CPosition _pos)
{
	for (CShip& _ship : GetShips()) {
		for (CSegment& _segment : _ship.GetSegments()) {
			if (_pos.x == _segment.GetPosition().x && _pos.y == _segment.GetPosition().y) {
				return true;
			}
		}
	}
	return false;
}

//Calculates if a shot has hit or not, then changes the appropriate values
bool CPlayer::CalcShot(CPosition _pos)
{
	AddShot(_pos);
	for (CShip &_ship : GetShips()) {
		for (CSegment &_segment : _ship.GetSegments()) {
			if (_pos.x == _segment.GetPosition().x && _pos.y == _segment.GetPosition().y && _segment.GetHitState() != 1) {
				AddHit(_pos);
				_segment.SetColour(26);
				_segment.SetHitState(1);
				return true;
			}
		}
	}
	if (!CheckHit(_pos)) AddMiss(_pos);
	return false;
}

void CPlayer::SetCursor(CPosition _pos)
{
	m_cursor = _pos;
}

CPosition CPlayer::GetCursor()
{
	return m_cursor;
}

void CPlayer::MoveCursorRight(int _x)
{
	m_cursor.x = clamp(m_cursor.x+_x,0,9);
	
}

void CPlayer::MoveCursorDown(int _y)
{
	m_cursor.y = clamp(m_cursor.y + _y, 0, 9);
}

bool CPlayer::CheckSame(CPlayer& _player)
{
	return this == &_player;
}
