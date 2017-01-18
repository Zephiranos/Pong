///
// Custom Rendering Functions Implementation
//
///
#include "Rendering.h"
#include <exception>
#include <iostream>


SDL_Texture* Rendering::RenderText(const std::string text, const std::string fontFile, const SDL_Color color, int fontsize, SDL_Renderer *renderer)
{

		TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontsize);

		if (font == NULL) return NULL;

		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);

		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		SDL_FreeSurface(textSurface);

		TTF_CloseFont(font);

		return  textTexture;
	
}