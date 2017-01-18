#pragma once
///
// Custom physics functions header
//
///
#ifndef _PHYSICS_H_
#include "SDL.h"

class Physics
{
public:
	static bool AABBCollision(SDL_Rect rect1, SDL_Rect rect2);
};

#endif // !_PHYSICS_H_
