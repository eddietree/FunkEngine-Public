#include <core/App.h>
#include <core/CmdArgs.h>

int main(int argc, char *argv[])
{
	// command line arguments
	funk::CmdArgs args(argc, argv);
	int screen_width	= args.GetArgValueInt("w", 0);
	int screen_height	= args.GetArgValueInt("h", 0);
	bool fullscreen		= !args.IsFlagSet("windowed");
	//bool show_console	= args.IsFlagSet("showconsole");
	
	funk::App app;
	app.InitWindow( screen_width, screen_height, fullscreen );
	app.Run();
	
	return 0;
}

