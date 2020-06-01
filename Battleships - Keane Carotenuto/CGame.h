#pragma once
#include <vector>
#include<iostream>
#include"CPlayer.h"

using namespace std;

class CGame																													// TF: Class
{
private:																													// TF: Access Specifier
	bool m_isRunning = true;
	bool m_debug = true;
	int m_pTurn = 0;
	vector<CPlayer> m_players;
	
	

public:																														// TF: Access Specifier
	~CGame();

	int state = 0;
	int m_AImoves = 0;
	vector<wstring> m_allSunk;

	void StartGame();
	void StopGame();
	void ToggleDebug();
	void SetDebug(bool _debug);
	bool GetDebug();
	bool IsRunning();

	void SwapPlayerTurn();
	void SetPlayerTurn(int _player);
	int GetPlayerTurn();

	void AddPlayer(CPlayer _newPlayer);
	CPlayer &GetPlayer(int _index);
	vector<CPlayer> &GetPlayers();
	void RemovePlayer(int _index);

	void UpdateBoards();
};