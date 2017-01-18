///
// Window class Implementation
//
///
#include "Window.h"
#include "Game.h"

Window::Window() :
	m_window(NULL),
	m_renderer(NULL),
	m_mouseFocus(false),
	m_keyboardFocus(false),
	m_FullScreen(false),
	m_minimized(false),
	m_height(0),
	m_width(0)
{
}

//Create Window
bool Window::Init()
{
	//Create window
	m_window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game::DEFAULT_SCREEN_WIDTH, Game::DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (m_window != NULL)
	{
		m_mouseFocus = true;
		m_keyboardFocus = true;
		m_width = Game::DEFAULT_SCREEN_WIDTH;
		m_height = Game::DEFAULT_SCREEN_HEIGHT;
	}

	return m_window != NULL;
}

//Creates renderer from internal window
SDL_Renderer* Window::CreateRenderer()
{
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return m_renderer;
}

//Handles window events
void Window::HandleEvent(SDL_Event& e)
{
	//Window event occured
	if (e.type == SDL_WINDOWEVENT)
	{
		//Caption update flag
		bool updateCaption = false;
		switch (e.window.event)
		{
			//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_width = e.window.data1;
			m_height = e.window.data2;
			//SDL_RenderPresent(gRenderer);
			break;
			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			//SDL_RenderPresent(gRenderer);
			break;
			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			m_mouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			m_mouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_keyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_keyboardFocus = false;
			updateCaption = true;
			break;
			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			m_minimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			m_minimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			m_minimized = false;
			break;
		}
	}
	//Enter exit full screen on return key
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		if (m_FullScreen)
		{
			SDL_SetWindowFullscreen(m_window, SDL_FALSE);
			m_FullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(m_window, SDL_TRUE);
			m_FullScreen = true;
			m_minimized = false;
		}
	}
}

//Deallocates internals
void Window::Free()
{

}