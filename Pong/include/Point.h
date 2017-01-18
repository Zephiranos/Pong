#pragma once
///
// Point Class Header
//
///
#ifndef _POINT_H_

class Point
{

private:
	int m_x;
	int m_y;

public:
	Point(int x, int y);
	int GetX();
	int GetY();
};

#endif // !_POINT_H_