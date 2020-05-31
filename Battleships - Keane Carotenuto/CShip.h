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
	

	CShip(CPosition _newPos, int _length, int _iDirection, wstring _icon, int _colour);

	void SetName(wstring _wstrNewName);
	wstring GetName();

	void SetDirection(int _iDir);
	int GetDirection();

	void SetStartPos(CPosition _pos);

	int GetColour();
	wstring GetIcon();

	void SetSegments(vector<CSegment> _newPos);
	void AddSegment(CSegment _segment);
	vector<CSegment> &GetSegments();
	CSegment &GetSegment(int _index );

	void Rebuild();

	void PlaceShip(CPosition _newPos, int _iDir);
};