///
// Ball class Implementation
//
///
#include "Ball.h"
#include <cmath>

Ball::Ball(int x, int y, int width, int height):
	m_Width(width),
	m_Height(height),
	m_X(x - width/2),
	m_Y(y - height/2),
	m_resetX(x - width / 2),
	m_resetY(y - height / 2),
	m_dX(0),
	m_dY(0),
	m_speedLaunch(4),
	m_speedMax(10)
{
}

void Ball::Reset()
{
	m_X = m_resetX;
	m_Y = m_resetY;
	m_dX = 0;
	m_dY = 0;
}

void Ball::Launch(int side)
{
	m_dY = m_speedLaunch-2;
	m_dX = copysign(m_speedLaunch, side);
}

void Ball::Update()
{
	m_X += m_dX;
	m_Y += m_dY;
}

SDL_Rect Ball::GetBoundingBox()
{
	//SDL_Rect bbox = { m_X, m_Y, m_Width, m_Height };
	return{ m_X, m_Y, m_Width, m_Height };
}

bool Ball::Collide(const SDL_Rect obj)
{
	SDL_Rect bbox = {m_X , m_Y, m_Width, m_Height};

	if (bbox.y < obj.y + obj.h && bbox.y > obj.y
		|| bbox.y + bbox.h < obj.y + obj.h && bbox.y + bbox.h > obj.y)
	{
		m_dY = -m_dY;
	}
	if (bbox.x < obj.x + obj.w && bbox.x > obj.x
		|| bbox.x + bbox.w < obj.x + obj.w && bbox.x + bbox.w > obj.x)
	{
		m_dX = -m_dX;
	}

	return false;
}

void Ball::Accelerate()
{
	m_dX = copysign(std::fmin(abs(m_dX) + 1, m_speedMax), m_dX);
	m_dY = copysign(std::fmin(abs(m_dY) + 1, m_speedMax), m_dY);
}

///
// Easy solution, might change later
///
void Ball::BounceOffWall()
{
	m_dY = -m_dY;
	Accelerate();
}

void Ball::BounceOffPaddle()
{
	m_dX = -m_dX;
	Accelerate();
}
///