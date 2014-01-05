#pragma once

#include <math/v2i.h>
#include <math/v2.h>
#include <common/HandledObj.h>

namespace funk
{
	struct ImguiWindow : public HandledObj<ImguiWindow>
	{
		// name
		char name[128];

		v2i		pos; // top-left position. rendered towards bottom-left
		v2i		dimen;
		v2i		dimenAutosize;		// autosize calculated
		v2i		dimenAutosizePrev;	// from last frame
		v2i		scrollPos;			// in pixels
		int		titleBarMinWidth;	// minimum title bar width
		float	bgAlpha;
		
		bool	autosize;
		bool	minimized;
		bool	isNewWindow; // first time
		bool	locked; // locked and cannot move
		bool	isSerializable; // does this window serialize to the GM file

		int	framesSinceUpdate; // >=1 means non-active

		ImguiWindow();
		void SetName( const char * a_name );
	};
}
