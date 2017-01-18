#pragma once
///
//Paddle class header
//
///
#ifndef _PADDLE_H_
#include "SDL.h"

class Paddle
{
private:
	//position
	int m_X;
	int m_Y;
	//size
	int m_Width;
	int m_Height;

	int m_direction;
	int m_speed;
	
	//Control class copy
	Paddle(const Paddle & paddle);
	Paddle & operator= (const Paddle & paddle);
public:
	Paddle(int xPos, int yPos, int width, int height);
	const int GetX() { return m_X; }
	const int GetY() { return m_Y; }
	const int GetWidth() { return m_Width; }
	const int GetHeight() { return m_Height; }
	SDL_Rect GetBoundingBox();
	const int GetDirection() { return m_direction; }
	void SetDirection(int direction);
	void Update();
	void MoveAI(SDL_Rect ballBBox);


};

#endif