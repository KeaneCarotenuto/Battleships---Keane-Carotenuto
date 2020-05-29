#pragma once
#include <vector>
#include<iostream>
#include"CPlayer.h"

using namespace std;

class CGame
{
protected:
	bool m_isRunning = true;
	bool m_debug = false;
	int m_pTurn = 0;
	vector<CPlayer> m_players;
	
	

public:
	int state = 0;
	int m_AImoves = 0;

	void StartGame();
	void StopGame();
	void ToggleDebug();
	bool GetDebug();
	bool IsRunning();

	void SwapPlayerTurn();
	int GetPlayerTurn();

	void AddPlayer(CPlayer _newPlayer);
	CPlayer &GetPlayer(int _index);
	vector<CPlayer> &GetPlayers();
	void RemovePlayer(int _index);

	void UpdateBoards();
};