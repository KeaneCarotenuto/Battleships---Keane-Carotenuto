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
	CSegment(CPosition _position, wstring _icon, int _colour);
	CSegment(int _hitState);

	//void Set(CSegment _segment);

	void SetPosition(CPosition _newPos);
	CPosition GetPosition();

	void SetIcon(wstring _icon);
	wstring GetIcon();

	void SetColour(int _colour);
	int GetColour();

	void SetHitState(int _iState);
	int GetHitState();
};

