#include "CShip.h"
#include "CPosition.h"

#include <vector>

using namespace std;

CShip::CShip(CPosition _startPos, int _length, int _iDirection, wstring _icon, int _colour): m_iDirection(_iDirection), m_startPos(_startPos), m_icon(_icon), m_colour(_colour)
{
	if (m_icon == L"A") { m_wstrName = L"Aircraft Carrier"; }
	else if (m_icon == L"B") { m_wstrName = L"Battleship"; }
	else if (m_icon == L"D") { m_wstrName = L"Destroyer"; }
	else if (m_icon == L"S") { m_wstrName = L"Submarine"; }
	else if (m_icon == L"P") { m_wstrName = L"Patrol Boat"; }

	for (int i = 0; i < _length; i++) {
		
		if (m_iDirection == 0) {
			AddSegment({ {_startPos.x + i, _startPos.y}, _icon, _colour });
		}
		if (m_iDirection == 1) {
			AddSegment({ {_startPos.x, _startPos.y + i}, _icon, _colour });
		}
	}
}

void CShip::SetName(wstring newName) {
	m_wstrName = newName;
}

wstring CShip::GetName() {
	return m_wstrName;
}

void CShip::SetDirection(int _iDir) {
	m_iDirection = _iDir;
}

int CShip::GetDirection() {
	return m_iDirection;
}

void CShip::SetStartPos(CPosition _pos)
{
	m_startPos = _pos;
}

int CShip::GetColour()
{
	return m_colour;
}

wstring CShip::GetIcon()
{
	return m_icon;
}

void CShip::SetSegments(vector<CSegment> _newSegment) {
	m_segments = _newSegment;
}

void CShip::AddSegment(CSegment _segment) {
	m_segments.push_back(_segment);
}

vector<CSegment> &CShip::GetSegments() {
	return m_segments;
}

CSegment &CShip::GetSegment(int _index)
{
	return m_segments[_index];
}

void CShip::Rebuild()
{
	for (int i = 0; i < m_segments.size(); i++) {
		if (m_iDirection == 0) {
			AddSegment({ {m_startPos.x + i, m_startPos.y}, m_icon, m_colour });
		}
		if (m_iDirection == 1) {
			AddSegment({ {m_startPos.x, m_startPos.y + i}, m_icon, m_colour });
		}
		m_segments.erase(m_segments.begin());
	}
}

void CShip::PlaceShip(CPosition _newPos, int _iDir) {


}
