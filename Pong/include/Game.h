#pragma once
///
// Game Class Header
//
///

#ifndef _GAME_H_
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Paddle.h"
#include "Ball.h"
#include <iostream>

enum GameState
{
	Ready,
	Launch,
	Running
};

enum Winner
{
	None,
	LeftPlayer,
	RightPlayer
};

class Game
{
private:

	bool m_isRunning;

	GameState m_gameState;

	// Window & renderer
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	int m_screenWidth;
	int m_screenHeight;

	//Assets
	std::string m_fontFile;
	SDL_Color m_fontColor;
	SDL_Texture* m_textureScoreL;
	SDL_Texture* m_textureScoreR;
	SDL_Texture* m_textureReadyTxt1;
	SDL_Texture* m_textureReadyTxt2;
	SDL_Texture* m_textureWin;
	Mix_Chunk* m_audioPad;
	Mix_Chunk* m_audioCol;
	Mix_Chunk* m_audioLaunch;

	//Game Entities
	SDL_Rect m_wallTop;
	SDL_Rect m_wallBot;
	SDL_Rect m_goalL;
	SDL_Rect m_goalR;
	Ball* m_ball;
	Paddle* m_padLeft;
	Paddle* m_padRight;

	//Stuff
	SDL_Event m_event;
	Winner m_winner;
	int m_mouseX;
	int m_mouseY;

	//Gameplay
	int m_scoreL;
	int m_scoreR;


	//Methods
	int Initialize();
	int Events();
	int Update();
	int Render();
	int Cleanup();

public:
	// Screen resolution 
	static const int DEFAULT_SCREEN_WIDTH = 800;
	static const int DEFAULT_SCREEN_HEIGHT = 600;

	Game();
	int Execute();
};

#endif // !_GAME_H_


