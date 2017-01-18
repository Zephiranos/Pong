///
// Window Class Header
//
///
#ifndef _WINDOW_H_
#include "SDL.h"

class Window
{

private:
	//Window data
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	//Window dimensions
	int m_width;
	int m_height;

	//Window focus
	bool m_mouseFocus;
	bool m_keyboardFocus;
	bool m_FullScreen;
	bool m_minimized;

public:
	//Constructor
	Window();

	//Create Window
	bool Init();

	//Creates renderer from internal window
	SDL_Renderer* CreateRenderer();

	//Handles window events
	void HandleEvent(SDL_Event& e);

	//Deallocates internals
	void Free();

	//Window dimensions
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	//Window focii
	bool HasMouseFocus() const { return m_mouseFocus; }
	bool HasKeyboardFocus() const { return m_keyboardFocus; }
	bool IsMinimized() const { return m_minimized; }


};

#endif // !_WINDOW_H_