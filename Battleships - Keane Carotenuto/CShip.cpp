#include "CShip.h"
#include "CPosition.h"

#include <vector>

using namespace std;

CShip::CShip(CPosition _startPos, int _length, int _iDirection, wstring _icon, int _colour): m_iDirection(_iDirection), m_icon(_icon), m_colour(_colour)
{
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

void CShip::PlaceShip(CPosition _newPos, int _iDir) {


}
