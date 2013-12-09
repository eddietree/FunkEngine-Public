#include <core/App.h>
#include <core/CmdArgs.h>

#include <common/Memory.h>
#include <common/Debug.h>
#include <common/Log.h>

#include <GL/glew.h>
#include <il/il.h>
#include <il/ilu.h>

#include <stdlib.h>

// remove black console from windows build
#if defined(_WIN32)
	#include <Windows.h>
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

void initExtLibs()
{
	// glew
	GLenum err = glewInit();
	ASSERT(GLEW_OK == err );
	TRACE( "Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// devil
	ilInit();
	iluInit();

    // force same origin for all image files (pngs can be flipped on Y, etc)
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
    ilEnable(IL_ORIGIN_SET);
}

void LogException( const char * a_str )
{
	if ( !funk::Log::InstExists() ) funk::Log::CreateInst();
	LOG("Exception occurred: %s", a_str);
	funk::Log::DestroyInst();
}

int main(int argc, char *argv[])
{
	try
	{
		funk_memory_init();

		// command line arguments
		funk::CmdArgs args(argc, argv);
		int screen_width	= args.GetArgValueInt("w", 0);
		int screen_height	= args.GetArgValueInt("h", 0);
		bool fullscreen		= !args.IsFlagSet("windowed");

		// launch the app!
		funk::App app;
		app.InitWindow( screen_width, screen_height, fullscreen );
		initExtLibs();
		app.Run();
	}
	catch (std::exception& ex) 
	{
		LogException( ex.what() );
	}
	catch(...)
	{
		LogException( "Unknown exception" );
	}
	
	return 0;
}