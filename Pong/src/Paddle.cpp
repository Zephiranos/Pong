///
// Paddle class Implementation
//
///
#include "Paddle.h"
#include <cmath>

using namespace std;

Paddle::Paddle(int xPos, int yPos, int width, int height) :
	m_X(xPos),
	m_Y(yPos),
	m_Width(width),
	m_Height(height),
	m_direction(0),
	m_speed(7)
{
}

void Paddle::SetDirection(int direction)
{
	if (direction == 0) m_direction = 0;
	else
	{
		m_direction = abs(direction) / direction;
	}
}

void Paddle::Update()
{
	m_Y += m_direction * m_speed;
}

SDL_Rect Paddle::GetBoundingBox()
{
	SDL_Rect bbox = { m_X, m_Y, m_Width, m_Height };
	return bbox;
}

void Paddle::MoveAI(SDL_Rect ballBBox)
{
	int ballCenterY = ballBBox.y + ballBBox.h / 2;
	int padCenterY = m_Y + m_Height / 2;
	SetDirection(ballCenterY - padCenterY);
}