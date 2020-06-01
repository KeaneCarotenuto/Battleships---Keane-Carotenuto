/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2018 Media Design School
-
Project: Battleships
File Name : CSegment.cpp
Description : The functions for the Segment class.
-
Author : Keane Carotenuto
Mail : Keane.Car8958@mediadesign.school.nz
**************************************************************************/

#include "CSegment.h"
#include <string>

using namespace std;

CSegment::CSegment(CPosition _position, wstring _icon, int _colour):
	m_iHitState(0),
	m_icon(_icon),
	m_colour(_colour),
	m_position(_position)
{
}

CSegment::CSegment(int _hitState):m_iHitState(_hitState),m_icon(L"~"),m_colour(25)
{
}

void CSegment::SetHitState(int _iState) {
	m_iHitState = _iState;
}

int CSegment::GetHitState() {
	return m_iHitState;
}

void CSegment::SetPosition(CPosition _newPos) {
	m_position = _newPos;
}

CPosition CSegment::GetPosition() {
	return m_position;
}

void CSegment::SetIcon(wstring _icon)
{
	m_icon = _icon;
}

wstring CSegment::GetIcon()
{
	return m_icon;
}

void CSegment::SetColour(int _colour)
{
	m_colour = _colour;
}

int CSegment::GetColour()
{
	return m_colour;
}
