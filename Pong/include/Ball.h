#pragma once
///
// Ball class header
//
///
#ifndef _BALL_H_

#include "Point.h"
#include "SDL.h"

class Ball
{
private:

	int m_resetX;
	int m_resetY;

	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

	int m_dX;
	int m_dY;

	int m_speedLaunch;
	int m_speedMax;

	//Control class copy
	Ball(const Ball & ball);
	Ball & operator= (const Ball & ball);

	void Accelerate();

public:
	Ball(int x, int y, int width, int height);

	const int GetX() { return m_X; }
	const int GetY() { return m_Y; }
	const int GetWidth() { return m_Width; }
	const int GetHeight() { return m_Height; }
	SDL_Rect GetBoundingBox();

	void Reset();
	void Launch(int side);
	void Update();
	bool Collide(const SDL_Rect);
	void BounceOffWall();
	void BounceOffPaddle();
};

#endif // !_BALL_H_
