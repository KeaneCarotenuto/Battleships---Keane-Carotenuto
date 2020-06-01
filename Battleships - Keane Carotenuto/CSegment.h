/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : CSegment.h
Description : Each ship is made of segments, which have a position and hit state so that the game may be played.
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

#pragma once
#include"CPosition.h"
#include <string>

using namespace std;

class CSegment
{
protected:
	CPosition m_position;
	wstring m_icon;
	int m_colour;
	int m_iHitState = 0;

public:
	//Basic Constructor
	CSegment(CPosition _position, wstring _icon, int _colour);
	CSegment(int _hitState);

	//Management of values
	void SetPosition(CPosition _newPos);
	CPosition GetPosition();

	void SetIcon(wstring _icon);
	wstring GetIcon();

	void SetColour(int _colour);
	int GetColour();

	void SetHitState(int _iState);
	int GetHitState();
};

