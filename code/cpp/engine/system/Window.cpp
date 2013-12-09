#include "Window.h"

#include <system/Config.h>
#include <common/Debug.h>

#if !PLATFORM_MOBILE
#include <SDL2/SDL.h>
#endif

namespace funk
{
	Window::Window() : m_dimen(0)
	{
		
	}

	Window::~Window()
	{
		
	}

	void Window::OnRotateOrientation( DeviceOrientation a_orientation, int a_screen_width, int a_screen_height )
	{
		LOG("Rotate device: dimen(%d,%d), orientation: %d", a_screen_width, a_screen_height, a_orientation );
		m_orientation = a_orientation;
		SetDimen( a_screen_width, a_screen_height );
		
		// TODO: CALL GM FUNC
	}

    bool Window::IsFullScreen() const 
	{ 
		// TODO
		return false; 
	}

	void Window::SetTitle(const char* title) 
	{ 
		// TODO
		return; 
	}

	void Window::ShowMouseCursor( bool a_enable )
	{
		#if defined(_SDL_H)				
		SDL_ShowCursor( a_enable ? SDL_ENABLE : SDL_DISABLE); 
		#endif
	}

	void Window::Exit()
	{
		#if defined(_SDL_H)				
		SDL_Quit();
		exit(0);
		#endif
	}
}