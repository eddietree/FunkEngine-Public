#include "ImguiWindow.h"

#include <string>

namespace funk
{

ImguiWindow::ImguiWindow()
{
	framesSinceUpdate = 0;
	titleBarMinWidth = 0;
	minimized = false;
	isNewWindow = true;
	autosize = false;
	locked = false;
	isSerializable = true;
	scrollPos = v2(0);
	pos = v2i(-1);
	dimenAutosize = v2i(0);
	bgAlpha = 0.5f;
}

void ImguiWindow::SetName( const char * a_name )
{
	strcpy( name, a_name );
}

}