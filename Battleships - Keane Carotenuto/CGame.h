/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : CGame.h
Description : This is where the main game infortmation is stored and changed. Basically the owner of every other class.
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

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

	int m_AImoves = 0;

	//manages the state of the game
	int state = 0;
	vector<wstring> m_allSunk;

	void StartGame();
	void StopGame();
	void ToggleDebug();
	void SetDebug(bool _debug);
	bool GetDebug();
	bool IsRunning();

	//Changes who is shooting
	void SwapPlayerTurn();
	void SetPlayerTurn(int _player);
	int GetPlayerTurn();

	//Manages players
	void AddPlayer(CPlayer _newPlayer);
	CPlayer &GetPlayer(int _index);
	vector<CPlayer> &GetPlayers();
	void RemovePlayer(int _index);

	void UpdateBoards();
};