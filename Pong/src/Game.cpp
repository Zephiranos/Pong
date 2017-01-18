///
// Game class Implementation
//
///
#include "Game.h"
#include "Rendering.h"
#include "Physics.h"
#include <string>

#define LEFT_PLAYER 1
#define RIGHT_PLAYER 2

using namespace std;

Game::Game():
	m_isRunning(true),
	m_screenWidth(DEFAULT_SCREEN_WIDTH),
	m_screenHeight(DEFAULT_SCREEN_HEIGHT),
	m_gameState(Ready)
{
}

///
// Runs game
///
int Game::Execute()
{
	Initialize();

	while (m_isRunning) {
		Events();
		Update();
		Render();
	}

	Cleanup();
	return 0;
}

///
// Initialize game
///
int Game::Initialize()
{
	//SDL Initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return -1;
	}
	
	m_window = SDL_CreateWindow("Pong", 100, 100, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		Cleanup();
		return -1;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr)
	{
		Cleanup();
		return -1;
	}

	//Game Initialization
	//Score
	m_scoreL = 0;
	m_scoreR = 0;
	//Paddle
	int padWidth = m_screenWidth / 50;
	int padHeight = m_screenHeight / 8;
	m_padLeft = new Paddle(m_screenWidth/40, (m_screenHeight - padHeight)/2, padWidth, padHeight);
	m_padRight = new Paddle(m_screenWidth - m_screenWidth / 40 - padWidth, (m_screenHeight - padHeight) / 2, padWidth, padHeight);
	//Walls
	m_wallTop = { 0, 0, m_screenWidth , (int)(m_screenHeight * 0.01) };
	m_wallBot = { 0, m_screenHeight - (int)(m_screenHeight * 0.01), m_screenWidth, (int)(m_screenHeight * 0.01) };
	//Ball
	m_ball = new Ball(m_screenWidth / 2, m_screenHeight / 2, padWidth, padWidth);
	//Goals
	m_goalL = {0, 0, 2, m_screenHeight};
	m_goalR = {m_screenWidth - 2, 0, 2, m_screenHeight};

	//Font Initialization
	m_fontColor = {255, 255, 255, 255}; //White
	m_fontFile = "Assets\\Fonts\\Gameplay.ttf";
	if (TTF_Init() != 0)
	{
		Cleanup();
		return -1;
	}
	// Audio Initialization
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
	{
		Cleanup();
		return -1;
	}

	///
	// Assets 
	///
	// textures
	m_textureScoreL = Rendering::RenderText(to_string(m_scoreL), m_fontFile, m_fontColor, 26, m_renderer);
	m_textureScoreR = Rendering::RenderText(to_string(m_scoreR), m_fontFile, m_fontColor, 26, m_renderer);
	m_textureReadyTxt1 = Rendering::RenderText("Press SPACE", m_fontFile, m_fontColor, 26, m_renderer);
	m_textureReadyTxt2 = Rendering::RenderText("To Start", m_fontFile, m_fontColor, 26, m_renderer);
	m_textureWin = Rendering::RenderText("Winner", m_fontFile, m_fontColor, 26, m_renderer);
	// audio
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
	m_audioCol = Mix_LoadWAV("Assets\\Sound\\ping_pong_8bit_plop.ogg");
	m_audioLaunch = Mix_LoadWAV("Assets\\Sound\\ping_pong_8bit_peeeeeep.ogg");
	m_audioPad = Mix_LoadWAV("Assets\\Sound\\ping_pong_8bit_beeep.ogg");
	Mix_VolumeChunk(m_audioCol, MIX_MAX_VOLUME / 2);
	Mix_VolumeChunk(m_audioLaunch, MIX_MAX_VOLUME / 2);
	Mix_VolumeChunk(m_audioPad, MIX_MAX_VOLUME / 2);

	return 0;
}

///
// Handle Input
///
int Game::Events()
{
	while (SDL_PollEvent(&m_event))
	{
		//closes window
		if (m_event.type == SDL_QUIT)
		{
			m_isRunning = false;
		}
		//// Track mouse movement
		//if (m_event.type == SDL_MOUSEMOTION)
		//	SDL_GetMouseState(&m_mouseX, &m_mouseY);
		if (m_event.type == SDL_KEYDOWN)
		{
			switch (m_event.key.keysym.sym) {

			// Pressing ESC exits from the game
			case SDLK_ESCAPE:
				m_isRunning = false;
				break;
			// Pressing space will launch the ball if it isn't already launched
			case SDLK_SPACE:
				if (m_gameState == Ready)
					m_gameState = Launch;
				break;
			// Pad move up
			case SDLK_UP:
				m_padRight->SetDirection(-1);
				break;
			// Pad move down
			case SDLK_DOWN:
				m_padRight->SetDirection(1);
				break;
			}
		}
		if (m_event.type == SDL_KEYUP)
		{
			// Pad stops moving
			if (m_event.key.keysym.sym == SDLK_UP && m_padRight->GetDirection() == -1 
				|| m_event.key.keysym.sym == SDLK_DOWN && m_padRight->GetDirection() == 1)
				m_padRight->SetDirection(0);
		}
	}
	return 0;
}

///
// Update game objects and physics
///
int Game::Update()
{
	//Launching Ball
	if (m_gameState == Launch)
	{
		Mix_PlayChannel(1, m_audioLaunch, 0);
		m_ball->Launch(1);
		m_gameState = Running;
		m_winner = None;
		m_scoreL = m_scoreR = 0;
		m_textureScoreL = Rendering::RenderText(to_string(m_scoreL), m_fontFile, m_fontColor, 26, m_renderer);
		m_textureScoreR = Rendering::RenderText(to_string(m_scoreR), m_fontFile, m_fontColor, 26, m_renderer);
	}

	if (m_gameState == Running)
	{
		///
		// AI
		///
		m_padLeft->MoveAI(m_ball->GetBoundingBox());

		///
		// Collision detection
		///
		// Left Pad vs Walls
		if (Physics::AABBCollision(m_wallTop, m_padLeft->GetBoundingBox()) && m_padLeft->GetDirection() == -1 //collide with top wall
			|| Physics::AABBCollision(m_wallBot, m_padLeft->GetBoundingBox()) && m_padLeft->GetDirection() == 1) //collide with bottom wall
			m_padLeft->SetDirection(0);
		// Right Pad vs Walls
		if (Physics::AABBCollision(m_wallTop, m_padRight->GetBoundingBox()) && m_padRight->GetDirection() == -1 //collide with top wall
			|| Physics::AABBCollision(m_wallBot, m_padRight->GetBoundingBox()) && m_padRight->GetDirection() == 1) //collide with bottom wall
			m_padRight->SetDirection(0);
		///
		// Easy solution
		// Might change
		///
		// Ball vs Walls
		if (Physics::AABBCollision(m_ball->GetBoundingBox(), m_wallTop)
			|| Physics::AABBCollision(m_ball->GetBoundingBox(), m_wallBot))
		{
			m_ball->BounceOffWall();
			Mix_PlayChannel(1, m_audioCol, 0);
		}
		//Ball vs Paddles
		if (Physics::AABBCollision(m_ball->GetBoundingBox(), m_padRight->GetBoundingBox())
			|| Physics::AABBCollision(m_ball->GetBoundingBox(), m_padLeft->GetBoundingBox()))
		{
			m_ball->BounceOffPaddle();
			Mix_PlayChannel(1, m_audioPad, 0);
		}

		///
		// Update objects positions
		///
		//Paddles
		m_padRight->Update();
		m_padLeft->Update();
		//Ball
		m_ball->Update();
		///

		///
		//Game 
		///
		// goal scored
		if (Physics::AABBCollision(m_ball->GetBoundingBox(), m_goalL))
		{
			Mix_PlayChannel(1, m_audioLaunch, 0);
			++m_scoreR;
			m_textureScoreR = Rendering::RenderText(to_string(m_scoreR), m_fontFile, m_fontColor, 26, m_renderer);
			m_ball->Reset();
			m_ball->Launch(1);
		}
		if (Physics::AABBCollision(m_ball->GetBoundingBox(), m_goalR))
		{
			Mix_PlayChannel(1, m_audioLaunch, 0);
			++m_scoreL;
			m_textureScoreL = Rendering::RenderText(to_string(m_scoreL), m_fontFile, m_fontColor, 26, m_renderer);
			m_ball->Reset();
			m_ball->Launch(-1);
		}
		// Win
		if (m_scoreL == 10 || m_scoreR == 10)
		{
			m_winner = (m_scoreL == 10) ? LeftPlayer : RightPlayer;
			m_gameState = Ready;
		}
	}
	return 0;
}

///
// Render game objects on screen
///
int Game::Render()
{
	// Clear screen (background color)
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);     
	SDL_RenderClear(m_renderer);

	/// Draw Pong terrain
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	//net
	SDL_Rect net[20];
	int netX = m_screenWidth / 2 - m_screenWidth * 0.005;
	int netWidth = m_screenWidth * 0.01;
	int netHeight = (m_screenHeight - m_screenHeight * 0.01 * 19) / 20;
	for (int i = 0; i < 20; ++i)
	{
		net[i] = {netX, i * (netWidth + netHeight), netWidth, netHeight };
	} 
	// draw
	SDL_RenderFillRect(m_renderer, &m_wallTop);
	SDL_RenderFillRect(m_renderer, &m_wallBot);
	SDL_RenderFillRects(m_renderer, net, 20);

	///
	// UI
	///
	//Left score
	SDL_Rect rectScoreL = { m_screenWidth / 3 - m_screenWidth / 40, m_screenHeight / 40, m_screenWidth/20, m_screenHeight/20 };
	SDL_RenderCopy(m_renderer, m_textureScoreL, NULL, &rectScoreL);
	//Right score
	SDL_Rect rectScoreR = { 2 * m_screenWidth / 3 - m_screenWidth / 40, m_screenHeight / 40, m_screenWidth / 20, m_screenHeight / 20 };
	SDL_RenderCopy(m_renderer, m_textureScoreR, NULL, &rectScoreR);
	//Start Text
	if (m_gameState == Ready)
	{
		SDL_Rect rectReadyTxt = { 2*m_screenWidth / 8 - m_screenWidth / 6, 3 * m_screenHeight / 8, m_screenWidth / 3, m_screenHeight / 8 };
		SDL_RenderCopy(m_renderer, m_textureReadyTxt1, NULL, &rectReadyTxt);
		SDL_Rect rectReadyTxt2 = { m_screenWidth / 8, m_screenHeight / 2, m_screenWidth / 4, m_screenHeight / 8 };
		SDL_RenderCopy(m_renderer, m_textureReadyTxt2, NULL, &rectReadyTxt2);
	}
	if (m_winner != None)
	{
		int xpos = (m_winner == LeftPlayer) ? m_screenWidth / 8 : 5 * m_screenWidth / 8;
		SDL_Rect rectWinner = { xpos, m_screenHeight / 8, m_screenWidth / 4, m_screenHeight / 8 };
		SDL_RenderCopy(m_renderer, m_textureWin, NULL, &rectWinner);
	}
	///

	///
	// Game entities
	///
	//Paddles
	SDL_Rect paddleL = { m_padLeft->GetX(), m_padLeft->GetY(), m_padLeft->GetWidth(), m_padLeft->GetHeight() };
	SDL_Rect paddleR = { m_padRight->GetX(), m_padRight->GetY(), m_padRight->GetWidth(), m_padRight->GetHeight() };
	//Ball
	SDL_Rect ball = {m_ball->GetX(), m_ball->GetY(), m_ball->GetWidth(), m_ball->GetHeight()};
	// Draw
	SDL_RenderFillRect(m_renderer, &paddleL);
	SDL_RenderFillRect(m_renderer, &paddleR);
	SDL_RenderFillRect(m_renderer, &ball);

	// Swap buffers
	SDL_RenderPresent(m_renderer);
	return 0;
}

///
// Destroy assets and pointers
///
int Game::Cleanup()
{
	///
	// Destroy Asets
	///
	// Textures
	SDL_DestroyTexture(m_textureScoreL);
	SDL_DestroyTexture(m_textureScoreR);
	SDL_DestroyTexture(m_textureReadyTxt1);
	SDL_DestroyTexture(m_textureReadyTxt2);
	SDL_DestroyTexture(m_textureWin);
	// Audio
	Mix_FreeChunk(m_audioCol);
	Mix_FreeChunk(m_audioLaunch);
	Mix_FreeChunk(m_audioPad);
	Mix_CloseAudio();
	Mix_Quit();

	// Destroy Window and renderer
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);

	SDL_Quit();

	return 0;
}
	