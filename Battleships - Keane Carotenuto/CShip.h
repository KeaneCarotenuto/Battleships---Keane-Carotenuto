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
	int m_colour;
	int m_iDirection;
	vector<CSegment> m_segments;

public:
	CShip(CPosition _newPos, int _length, int _iDirection, wstring _icon, int _colour);

	void SetName(wstring _wstrNewName);
	wstring GetName();

	void SetDirection(int _iDir);
	int GetDirection();

	void SetSegments(vector<CSegment> _newPos);
	void AddSegment(CSegment _segment);
	vector<CSegment> &GetSegments();
	CSegment &GetSegment(int _index );

	void PlaceShip(CPosition _newPos, int _iDir);
};