#pragma once
///
// Custom rendering functions header
//
///
#ifndef _RENDERING_H_
#include "SDL.h"
#include <SDL_ttf.h>
#include <iostream>

class Rendering
{

public:
	static SDL_Texture* RenderText(const std::string text, const std::string fontFile, const SDL_Color color, int fontsize, SDL_Renderer *renderer);

};


#endif // !_RENDERING_H_
