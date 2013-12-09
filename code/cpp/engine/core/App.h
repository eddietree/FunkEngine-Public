
#pragma once

#include <core/Core.h>

// fwd decl
struct SDL_Window;

namespace funk
{
	class App
	{
	private:
		
		void* m_sdl_gl_context; // SDL_GLContext type
		SDL_Window * m_sdl_window;
		
		bool m_is_running;
		float m_dt;

		Core m_core;

	public:
		App();
		
		// Initializes window, if width or height is zero, will use screen dimensions
		void InitWindow( int a_width, int a_height, bool a_fullscreen );
		void Run();
	
	private:

		void HandleToggleFullscreen();
		void LogInfo();
		void CleanUp();
		
		void Update( float a_dt );
		void Draw( float a_dt );
		void DrawGui();
		void HandleInput();
	};
}