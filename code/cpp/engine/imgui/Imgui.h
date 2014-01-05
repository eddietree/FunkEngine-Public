#pragma once

#include "ImguiTilesheetChunks.h"

#include <math/v2i.h>
#include <math/v2.h>
#include <math/v3.h>
#include <common/StrongHandle.h>
#include <gfx/Texture2d.h>

#include <string>

namespace funk
{
	class LineGraph;
	class CubicSpline2d;
    class Texture2d;

	struct Imgui
	{
		static const int AUTOPOS = -1;
		static const int AUTOSIZE = -1;

		static void		Begin( const char* name, v2i pos = v2i(AUTOPOS), v2i dimen = v2i(Imgui::AUTOSIZE) );
		static void		End();

		static void		Header( const char* name );
		static void		SameLine();
		static void		Separator();
		static void		Tab();
		static void		Untab();
		static void		Minimize();
		static bool		IsMinimized();
		static void		SetSerializable(bool is_serializable); // does this window serialize to the GM file

		static bool		Button( const char* name );
		static bool		ButtonDown( const char* name );
		static bool		ButtonHover( const char* name );
		static bool		ButtonNoPadding( const char* name );
		static bool		ButtonDownNoPadding( const char* name );
		static bool		ButtonHoverNoPadding( const char* name );

		static bool		CheckBox( const char* name, bool &val );
		static int		Select( const char* names[], int &val, int numChoices ); // returns the index of the choices
		static int		SelectCustom( const char* names[], int* values, int &val, int numChoices ); // returns val inside "values"
		static int		DropDown( const char* name, bool &val );

		// image
		static void		Image( StrongHandle<Texture2d> tex, v2i dimen, v2 uv0, v2 uv1 );
		static bool		ImageButton( StrongHandle<Texture2d> tex, v2i dimen, v2 uv0, v2 uv1 );
		
		// tilesheet
		static v2i		TilesheetSelector( StrongHandle<Texture2d> tex, int tile_size, v2i selected_grid_pos = v2i(-1), float scale = 1.0f );
		static int		TilesheetChunkSelector( StrongHandle<ImguiTilesheetChunks> tilesheet_chunks, StrongHandle<Texture2d> tex, int tile_size, int selected_chunk_id = -1, float scale = 1.0f ); // returns chunk id

		static float	SliderFloat( const char* name, float & val, float min, float max );
		static int		SliderInt( const char* name, int & val, int min, int max );
		static v2		SliderV2( const char* name, v2 & val, v2 min, v2 max );
		static v2i		SliderV2i( const char* name, v2i & val, v2i min, v2i max );
		static v3		SliderV3( const char* name, v3 & val, v3 min, v3 max );
		static v3		SliderRGB( const char* name, v3 & val);
		static v3		SliderHSV( const char* name, v3 & rgb);

		static void		FillBarFloat( const char* name, float val, float min, float max );
		static void		FillBarInt( const char* name, int val, int min, int max );
		static void		FillBarV2( const char* name, v2 val, v2 min, v2 max );
		static void		FillBarV3( const char* name, v3 val, v3 min, v3 max );
		static void		ColorBlock( const char* name, v3 rgb, v2i dimen = v2i(64) );

		static int		TextInput( const char* name, std::string& val, int width = 150 );
		static void		Print( const char * text );
		static void		PrintParagraph( const char * text );

		static void		BarGraph( float * data, int num_bins, float min_val = 0.0f, float max_val = 1.0f, int highlight_bins_index = -1 );
		static void		LineGraph( LineGraph & lineGraph );
		static void		CubicSpline2d( StrongHandle<CubicSpline2d> lineGraph, v2 minPos, v2 maxPos, v2i dimen=v2i(300) );

		// advanced features
		static v2i		GetDrawPos();
		static void		SetDrawPos( v2i pos );
		static void		MoveDrawPosBy( v2i dimen );
		static void		MoveDrawPosNextLine( v2i dimen );
		static v2i		GetWindowDimen();
		static v2i		GetWindowDimenAutoSize();
		static void		SetScrollX( float x /*[0,1]*/);
		static void		SetScrollY( float y /*[0,1]*/);
		static void		SetScrollToPosY( int y );
		static void		SetScrollToPosYIfNotInView( int y, int pad_y );
		static float	GetScrollY();
		static bool		IsWindowActive();
		static bool		IsWidgetActive(int id);
		static bool		IsWorkingWindowNew();
		static int		GetPrevWidgetId();
		static void		SetActiveWidgetId(int id); // dangerous
		static void		Lock( const char * gui );
		static void		Unlock( const char * gui );

		static int FONT_WIDTH;
		static int FONT_HEIGHT;
		static int TITLE_BAR_HEIGHT;
	};
}
