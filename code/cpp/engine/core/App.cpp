#include "App.h"

#include <SDL2/SDL.h>

#include <system/Input.h>
#include <system/Window.h>
#include <system/Timer.h>
#include <core/Core.h>
#include <common/Debug.h>
#include <common/Log.h>
#include <common/Profiler.h>
#include <gfx/GLIncludes.h>
#include <gfx/ShaderManager.h>
#include <gfx/RenderState.h>
#include <imgui/ImguiManager.h>
#include <vm/VirtualMachine.h>

// NOTE: used by extern access in places far away from App.cpp
// TODO: our renderer needs to support this somehow
bool g_swap_freeze = false;

void checkSDLError(int line = -1)
{
	#ifndef FUNK_FINAL
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
        TRACE("SDL Error: %s\n", error);
        if (line != -1)
            TRACE(" + line: %i\n", line);
        SDL_ClearError();
    }
	#endif
}

#define ENABLE_VSYNC

#define VALIDATE_SDL( _CALL ) { int val = (_CALL); CHECK(val==0, "SDL Error"); }

const char *DEFAULT_WINDOW_TITLE = "FunkEngine";

namespace funk
{	
	App::App()
		: m_sdl_gl_context( nullptr )
		, m_sdl_window( nullptr )
		, m_is_running( true )
		, m_dt( 0.0f )
	{
	}

	void App::InitWindow( int a_width, int a_height, bool a_fullscreen )
	{		
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { ERROR_MSG("Error loading SDL"); }
		else { TRACE("SDL initialized successfully!\n"); }
		
		/*
		// Uncomment this to enable OpenGL 3.2
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		*/

        // TODO: can we enable debug ARB somehow?
        VALIDATE_SDL( SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG) );
		
		// attributes
		VALIDATE_SDL( SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) );
		VALIDATE_SDL( SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) );
		VALIDATE_SDL( SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) );
		

		// multisampling!
		//VALIDATE_SDL( SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) );
		//VALIDATE_SDL( SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,16) );
		
		checkSDLError();
		
		// monitor size
		int display_index = 0;
		SDL_DisplayMode display_mode;
		VALIDATE_SDL( SDL_GetDesktopDisplayMode( display_index, &display_mode ) );
		int monitor_width = display_mode.w;
		int monitor_height = display_mode.h;

		// if width or height is zero, will use monitor dimensions
		a_width = a_width == 0 ? monitor_width : a_width;
		a_height = a_height == 0 ? monitor_height : a_height;

#if defined(_WIN32)
		m_sdl_window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
									  a_width, a_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | (a_fullscreen & SDL_WINDOW_FULLSCREEN ));
        m_sdl_gl_context = SDL_GL_CreateContext(m_sdl_window);
#else
		// there is a weird bug on macbook retina displays which causes the window to be only
		// 1/4 of the bottom left screen. fixed it by spawning window then flipping to fullscreen
		m_sdl_window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
									  a_width, a_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN );
        
        m_sdl_gl_context = SDL_GL_CreateContext(m_sdl_window);
        
		//if ( a_fullscreen ) VALIDATE_SDL( SDL_SetWindowFullscreen(m_sdl_window, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP ) );

        SDL_GL_MakeCurrent( m_sdl_window, m_sdl_gl_context );
#endif // _WIN32
		
		checkSDLError();

		bool vert_sync = false;
		#if defined(ENABLE_VSYNC)
		vert_sync = true;
		#endif

		VALIDATE_SDL( SDL_GL_SetSwapInterval(vert_sync?1:0) );
		SDL_DisableScreenSaver();
		checkSDLError();
		
		funk::Window::CreateInst();
		funk::Window::Ref().SetDimen( a_width, a_height );
		funk::Window::Ref().SetDimenMonitor( monitor_width, monitor_height );
	}

	void App::LogInfo()
	{
		if ( !Log::InstExists() ) Log::CreateInst();

		// SDL version
		SDL_version version;
		SDL_GetVersion(&version);
		LOG("SDL Version: %d.%d.%d", version.major, version.minor, version.patch);
		LOG("SDL Video Driver: %s", SDL_GetVideoDriver(0));
		LOG("SDL Vertical Sync: %d", SDL_GL_GetSwapInterval());

		// print attributes
		#define LOG_SDL_GL_ATTRIBUTE( _attrib ) { int val; SDL_GL_GetAttribute(_attrib, &val); LOG("\t%s: %d", #_attrib, val); }
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_CONTEXT_MAJOR_VERSION );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_CONTEXT_MINOR_VERSION );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_MULTISAMPLEBUFFERS );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_MULTISAMPLESAMPLES );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_DOUBLEBUFFER );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_BUFFER_SIZE );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_DEPTH_SIZE );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_STENCIL_SIZE );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_RED_SIZE );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_GREEN_SIZE );
		LOG_SDL_GL_ATTRIBUTE( SDL_GL_BLUE_SIZE );
		#undef LOG_SDL_GL_ATTRIBUTE		

		// OpenGL attribs
		const char * gl_vendor		=  (char*)glGetString(GL_VENDOR);
		const char * gl_renderer	=  (char*)glGetString(GL_RENDERER);
		const char * gl_version		=  (char*)glGetString(GL_VERSION);
		const char * gl_glsl_ver	=  (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

		// crash on outdated graphics driver
		if ( gl_glsl_ver == nullptr )
		{
			MESSAGE_BOX("Error", "Your graphics driver is out of date, yo! Please update your graphics driver!\n(GLSL Version not recognized)" );
			HALT_ERROR();
		}

		// get version number
		LOG("OpenGL Properties\n\tVender: %s\n\tRenderer: %s\n\tVersion: %s\n\tGLSL version: %s",
			   gl_vendor, gl_renderer, gl_version, gl_glsl_ver );
	}
	
	void App::Run()
	{		
		#if !defined(FUNK_FINAL)
		Profiler::CreateInst();
		#endif

		m_is_running = true;
		
		const int fps = 60;
		const float dt = 1.0f/fps;

		LogInfo();
		m_core.Init(fps);

		funk::Timer frame_time;
		
		while( m_is_running )
		{
			frame_time.Start();

			#if !defined(FUNK_FINAL)
			Profiler::Ref().Begin();
			#endif

			// key and mouse events
			PROFILE_BEGIN( "Main Loop" );
			HandleInput();
			Update(dt);
			Draw(dt);
			PROFILE_END( "Main Loop" );

			// Gui
			PROFILE_BEGIN( "Gui Root" );
			DrawGui();
			PROFILE_END( "Gui Root" );

			#if !defined(FUNK_FINAL)
			Profiler::Ref().End();
			#endif

			// reload shaders
			if ( Input::Ref().DidKeyJustGoDown("F6") )
				ShaderManager::Ref().ReloadAll();

			if ( g_swap_freeze == false )
			{
				SDL_GL_SwapWindow(m_sdl_window);
			}

			// should just sleep exact time to next frame
			// do we have an accurate sleep?

			// busy wait until finished
			while( frame_time.GetTimeSecs() < dt )
			{
				// TODO: should wait properly till vsync, for now just take some busy-wait pressure off
				// TODO: will have to reconsider this timing for vita too
				::Sleep( 1 );
			}
		}
		
		CleanUp();
	}

	void App::HandleToggleFullscreen()
	{
		Input &input = Input::Ref();

		const char* modifier_key = "LALT";
        const char* activate_key = "RETURN";

		// make uses apple-key-F to fullscreen
		#if !defined(_WIN32)
		modifier_key = "LGUI";
        activate_key = "f";
		#endif

		if ( input.IsKeyDown(modifier_key) && input.DidKeyJustGoDown(activate_key) )
		{
			// toggle fullscreen
			Uint32 flags = SDL_GetWindowFlags(m_sdl_window);
			VALIDATE_SDL( SDL_SetWindowFullscreen(m_sdl_window, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (( flags & SDL_WINDOW_FULLSCREEN ) ?  0 : SDL_WINDOW_FULLSCREEN_DESKTOP) ) );
		}
	}
	
	void App::HandleInput()
	{
		PROFILE_BEGIN("App::HandleInput");

		Input &input = Input::Ref();
		input.Update();

		SDL_Event event;

		HandleToggleFullscreen();
		
		// handle events
		while( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
				m_is_running = false;

			if ( event.type == SDL_WINDOWEVENT )
			{
				switch ( event.window.event )
				{
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					{
						// grab current window dimen
						v2i new_window_dimen;
						SDL_GetWindowSize( m_sdl_window, &new_window_dimen.x, &new_window_dimen.y );

						// catch when user resizes
						if ( new_window_dimen != Window::Ref().Sizei() )
						{
							v2i prev_window_dimen = Window::Ref().Sizei();
							Window::Ref().SetDimen( new_window_dimen.x, new_window_dimen.y );
							RenderState::Ref().SetViewport( 0, 0, new_window_dimen.x, new_window_dimen.y );
							ImguiManager::Ref().OnResizeScreen( prev_window_dimen, new_window_dimen );
						}

						break;
					}

					case SDL_WINDOWEVENT_MINIMIZED:
						if ( VirtualMachine::InstExists() )	VirtualMachine::Ref().CallCoreFunc("OnWindowMinimized");
						break;

					case SDL_WINDOWEVENT_RESTORED:
						if ( VirtualMachine::InstExists() ) VirtualMachine::Ref().CallCoreFunc("OnWindowRestored");
						break;

					case SDL_WINDOWEVENT_FOCUS_GAINED:
						if ( VirtualMachine::InstExists() ) VirtualMachine::Ref().CallCoreFunc("OnWindowFocusGained");
						break;

					case SDL_WINDOWEVENT_FOCUS_LOST:
						if ( VirtualMachine::InstExists() ) VirtualMachine::Ref().CallCoreFunc("OnWindowFocusLost");
						break;
				}
			}
			else if ( event.type == SDL_DROPFILE )
			{
				// can has drop file support?
				const char * filename = event.drop.file;
				LOG( "Dropped file: '%s'", filename );
			}
			
			input.HandleEvent(&event);
		}

		if ( input.IsKeyDown("LALT") && input.DidKeyJustGoDown("F4") )
		{
			m_is_running = false;
		}

		// here we chick to see if any events should be pre-grabbed by imgui
		ImguiManager::Ref().DetectConsumeInputs();

		PROFILE_END("App::HandleInput");
	}

	void App::Update( float a_dt )
	{
		PROFILE_BEGIN("App::Update");
		m_core.Update(a_dt);
		PROFILE_END("App::Update");
	}
	
	void App::Draw( float a_dt )
	{
		PROFILE_BEGIN("App::Draw");
		m_core.Draw(a_dt);
		PROFILE_END("App::Draw");
	}

	void App::DrawGui()
	{
		m_core.Gui();
	}
	
	void App::CleanUp()
	{
		LOG("Destroying Window...");
		funk::Window::DestroyInst();
		
		#if !defined(FUNK_FINAL)
		Profiler::DestroyInst();
		#endif

		LOG("Quitting SDL..");
		SDL_GL_DeleteContext(m_sdl_gl_context);
		//SDL_DestroyWindow(m_sdl_window); // this crashes on macosx when quitting fullscreen
		SDL_Quit();
	}
}