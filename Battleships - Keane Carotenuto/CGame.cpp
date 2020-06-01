/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : CGame.cpp
Description : The functions for the game class.
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

#include "CGame.h"
#include <string>

using namespace std;

CGame::~CGame()
{
}

void CGame::StartGame()
{
	m_isRunning = true;
}

void CGame::StopGame()
{
	m_isRunning = false;
}

void CGame::ToggleDebug()
{
	m_debug = !m_debug;
}

void CGame::SetDebug(bool _debug)
{
	m_debug = _debug;
}

bool CGame::GetDebug()
{
	return m_debug;
}

bool CGame::IsRunning()
{
	return m_isRunning;
}

void CGame::SwapPlayerTurn()
{
	if (m_pTurn == 0) m_pTurn = 1;
	else m_pTurn = 0;
}

void CGame::SetPlayerTurn(int _player)
{
	m_pTurn = _player;
}

int CGame::GetPlayerTurn()
{
	return m_pTurn;
}

void CGame::AddPlayer(CPlayer _newPlayer)
{
	m_players.push_back(_newPlayer);
}

CPlayer &CGame::GetPlayer(int _index)
{
	return m_players[_index];
}

vector<CPlayer> &CGame::GetPlayers()
{
	return m_players;
}

void CGame::RemovePlayer(int _index)
{
	m_players.erase(m_players.begin() + _index);
}

//Loops through each player, each ship, each segment, and misses then updates thier respective board with the info (Not Drawing)
void CGame::UpdateBoards()
{
	for (CPlayer& _player : GetPlayers()) {
		for (CShip _ship : _player.GetShips()) {
			for (CSegment _segment : _ship.GetSegments()) {
				if (_segment.GetHitState() == 0 && _player.CheckSame(m_players[m_players.size()-1]) && !GetDebug()) continue;
				_player.m_board[_segment.GetPosition().y][_segment.GetPosition().x] = _segment;
				if (_player.CheckSame(m_players[m_players.size() - 1]) && !GetDebug()) {
					_player.m_board[_segment.GetPosition().y][_segment.GetPosition().x].SetIcon(L"╳");
				}
			}
		}
		for (CPosition _miss : _player.GetMisses()) {
			_player.m_board[_miss.y][_miss.x].SetHitState(-2);
		}
	}
}
