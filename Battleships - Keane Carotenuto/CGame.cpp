#include "CGame.h"
#include <string>

using namespace std;

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

void CGame::UpdateBoards()
{
	for (CPlayer& _player : GetPlayers()) {
		for (CShip _ship : _player.GetShips()) {
			for (CSegment _segment : _ship.GetSegments()) {
				if (_segment.GetHitState() == 0 && _player.checkSame(m_players[m_players.size()-1]) && !GetDebug()) continue;
				_player.m_board[_segment.GetPosition().y][_segment.GetPosition().x] = _segment;
			}
		}
		for (CPosition _miss : _player.GetMisses()) {
			_player.m_board[_miss.y][_miss.x].SetHitState(-2);
		}
	}
}
