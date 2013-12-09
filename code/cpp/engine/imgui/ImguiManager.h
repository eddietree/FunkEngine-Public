#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <stdint.h>
#include <set>

#include <common/Singleton.h>
#include <common/StrongHandle.h>

#include <gfx/Font.h>
#include <gfx/ShaderProgram.h>

#include <math/Cam2d.h>
#include "ImguiWindow.h"

namespace funk
{
	class ImguiManager : public Singleton<ImguiManager>
	{
	private:

		// all possible windows
		std::unordered_map< uint32_t, StrongHandle<ImguiWindow> > m_window_map;
		std::vector< StrongHandle<ImguiWindow> > m_visible_windows;

		Cam2d m_cam;
		Font m_font;
		StrongHandle<ShaderProgram> m_font_shader;

		// consume inputs
		std::set<int> m_consume_mouse_btns; // mouse buttons to consume
		std::set<uint32_t> m_consume_key_press; // key presses to consume

	public:

		struct State
		{
			union
			{
				float			f;
				int				i;
				unsigned int	uint;
				struct { float fvec[4]; };
				struct { int   ivec[4]; };
			} data;

			v2i	drawPos;
			v2i	drawPosPrev;
			v2i scrollOffset;
			int numTabs;	// tabs spacing for drawing

			// where the mouse can interact
			v2i mouseRegionStart;
			v2i mouseRegionDimen;

			// widget
			static const int WIDGET_NULL = -1;	// no widget
			static const int WIDGET_WINDOW = 1;	 // widget corresponding to titlebar/window

			int		activeWidgetId;
			int		widgetCount;
			bool	doesWidgetConsumeTextInput;
			
			StrongHandle<ImguiWindow> activeWindow;
			StrongHandle<ImguiWindow> workingWindow;

			void Reset();

		} state;

		void PreUpdate(); // clean up unused windows
		void PostUpdate(); // clean up unused windows
		void ClearActiveWindow();
		void ClearAllWindows();
		void OnResizeScreen( v2i a_prev_dimen, v2i a_new_dimen );

		// ordering
		void SetActiveWindow( StrongHandle<ImguiWindow> window );
		void SetActiveWidgetId( int id );
		int GetWindowOrderIndex( const char* name ) const; // 0 is top, -1 is invalid
		void BringWindowToFront( StrongHandle<ImguiWindow> & window );
		bool IsPosInWindow( v2i pos, StrongHandle<ImguiWindow> window ) const;

		// consuming input events
		void DetectConsumeInputs();
		bool DidMouseButtonGoDown( int btn ) const;
		bool DidKeyJustGoDown( const char* key ) const;

		// drawing
		Font & GetFont() { return m_font; }
		StrongHandle<ShaderProgram> &GetFontShader() { return m_font_shader; }
		Cam2d & GetCam() { return m_cam; }

		StrongHandle<ImguiWindow> GetWindow( const char* name );
		std::unordered_map< uint32_t, StrongHandle<ImguiWindow> >& GetWindowList() { return m_window_map; }

	private:
		friend Singleton<ImguiManager>;
		ImguiManager();
		~ImguiManager();
		
		// handling visible windows
		void PruneVisibleWindows(); // remove any hidden windows from visible list
		void UpdateWindowMap();
		void DebugPrintVisibleWindows();
	};
}
