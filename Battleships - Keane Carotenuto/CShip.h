/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : CShip.h
Description : This class is used to store the position and segments of each player's ships, along with some other info.
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

#pragma once
#include <string>
#include <vector>

#include"CPosition.h"
#include"CSegment.h"

using namespace std;

class CShip
{
protected:
	wstring m_wstrName = L"Ship";
	wstring m_icon;
	CPosition m_startPos;
	int m_colour;
	int m_iDirection;
	vector<CSegment> m_segments;
	//CPlayer m_owner;
	

public:
	//Constructor for ship
	CShip(CPosition _newPos, int _length, int _iDirection, wstring _icon, int _colour);

	//Basic info of ship
	void SetName(wstring _wstrNewName);
	wstring GetName();

	void SetDirection(int _iDir);
	int GetDirection();

	void SetStartPos(CPosition _pos);

	int GetColour();
	wstring GetIcon();

	//Adds or changes segments
	void SetSegments(vector<CSegment> _newPos);
	void AddSegment(CSegment _segment);
	vector<CSegment> &GetSegments();
	CSegment &GetSegment(int _index );

	//Use for Manual ship placement
	void PlaceShip(CPosition _newPos, int _iDir);
};