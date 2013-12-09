#include "ImguiManager.h"
#include "Imgui.h"

#include <common/Util.h>
#include <system/Window.h>
#include <system/Input.h>
#include <gfx/ShaderManager.h>


namespace funk
{

// how many frames until dead
const int DEAD_WINDOW_NUM_FRAMES = 100000;

ImguiManager::ImguiManager()
{	
	m_font.Init( "common/textures/font/droid_10.png", 10 );
	Imgui::FONT_WIDTH = m_font.CalcCharWidth('M');
	Imgui::FONT_HEIGHT = m_font.CalcDimen("M").y;
	m_font_shader = ShaderManager::Ref().GetProgram("common/shaders/fontDefault.vp", "common/shaders/fontDefault.fp" );

	m_cam.InitScreenSpace();

	state.Reset();

	const v2i windowSize = Window::Get()->Sizei();
	state.mouseRegionStart = v2i(0, windowSize.y);
	state.mouseRegionDimen = windowSize;

	LOG("ImguiManager initialized");
}

ImguiManager::~ImguiManager()
{
}

StrongHandle<ImguiWindow> ImguiManager::GetWindow( const char* name )
{
	assert(name);

	uint32_t hash_key = HashString(name);
	auto it = m_window_map.find(hash_key);

	if ( it == m_window_map.end() )
	{
		StrongHandle<ImguiWindow> result = new ImguiWindow();
		strcpy( result->name, name );
		m_window_map[hash_key] = result;

		return result;
	}

	return it->second;
}

void ImguiManager::PruneVisibleWindows()
{
	// remove all windows that are no longer visible
	for ( size_t i = 0; i < m_visible_windows.size(); ++i )
	{
		auto window = m_visible_windows[i];

		// remove if no longer visible
		if ( window->framesSinceUpdate > 0 )
		{
			m_visible_windows[i] = m_visible_windows.back();
			m_visible_windows.resize( m_visible_windows.size()- 1 );
		}
	}
}

void ImguiManager::UpdateWindowMap()
{
	auto it = m_window_map.begin();

	while( it != m_window_map.end() )
	{
		auto window = it->second;

		// if not updated, kill it!
		if ( window->framesSinceUpdate > 1 )
		{
			// make sure it is not same window
			if ( state.activeWindow == window )
			{
				ClearActiveWindow();
			}

			if ( window->framesSinceUpdate > DEAD_WINDOW_NUM_FRAMES )
			{
				m_window_map.erase(it++);
				continue;
			}
		}

		// is visible
		else
		{
			bool is_in_visible_list = false;

			// if already in m_visible_windows, ignore
			for ( auto visible_it : m_visible_windows )
			{
				// already in list
				if ( visible_it == window ) 
				{
					is_in_visible_list = true;
					break;
				}
			}

			// if not in m_visible_windows, add to back of the list
			if ( !is_in_visible_list )
			{
				m_visible_windows.push_back( window );
			}
		}

		++( window->framesSinceUpdate );
		++it;
	}

	//DebugPrintVisibleWindows();
}

void ImguiManager::SetActiveWindow( StrongHandle<ImguiWindow> window )
{
	if ( window == NULL )
	{
		ClearActiveWindow();
	}

	if ( state.activeWindow != window )
	{
		SetActiveWidgetId( State::WIDGET_NULL );
	}

	// bring window to front if exists
	if ( window != NULL && window != state.activeWindow )
	{
		BringWindowToFront( window );
	}

	state.activeWindow = window;
}

void ImguiManager::SetActiveWidgetId( int id )
{
	if( id == State::WIDGET_NULL )
	{
		// nothing
	}

	state.activeWidgetId = id;
	state.doesWidgetConsumeTextInput = false;
}

int ImguiManager::GetWindowOrderIndex( const char* name ) const
{
	// try to grab the window
	uint32_t hash_key = HashString( name );
	const auto it = m_window_map.find( hash_key );
	if ( it == m_window_map.end() ) return -1;

	// see if in list!
	for ( size_t i = 0; i < m_visible_windows.size(); ++i )
	{
		const auto window = m_visible_windows[i];

		// already in list
		if ( window == it->second ) 
		{
			return (int)i;
		}
	}

	return -1;
}

void ImguiManager::BringWindowToFront( StrongHandle<ImguiWindow> & window )
{
	for ( size_t i = 0; i < m_visible_windows.size(); ++i )
	{
		auto curr_window = m_visible_windows[i];

		if ( window == curr_window )
		{
			for ( size_t j = i; j >= 1; --j )
			{
				m_visible_windows[j] = m_visible_windows[j-1];
			}

			break;
		}
	}

	m_visible_windows[0] = window;
}

void ImguiManager::DebugPrintVisibleWindows()
{
	static int frame = 0;
	TRACE( "----- [%d] -----\n", frame++ );

	for ( size_t i = 0; i < m_visible_windows.size(); ++i )
	{
		auto window = m_visible_windows[i];
		TRACE( "[%d] %s - framesSinceUpdate: %d\n", i, window->name, window->framesSinceUpdate );
	}
}

void ImguiManager::PreUpdate()
{
	GetCam().InitScreenSpace();

	PruneVisibleWindows();
	UpdateWindowMap();	
}

void ImguiManager::PostUpdate()
{
	// clear up all buttons consumed
	m_consume_key_press.clear();
	m_consume_mouse_btns.clear();
}

void ImguiManager::ClearActiveWindow()
{
	state.activeWindow  = NULL;
	state.widgetCount = 0;
	state.doesWidgetConsumeTextInput = false;
}

void ImguiManager::ClearAllWindows()
{
	ClearActiveWindow();
	State().Reset();
	m_window_map.clear();
	m_visible_windows.clear();
}

void ImguiManager::OnResizeScreen( v2i a_prev_dimen, v2i a_new_dimen )
{
	for( auto window : GetWindowList() )
	{
		int prev_dist_from_top = a_prev_dimen.y - window.second->pos.y;
		int new_y = a_new_dimen.y - prev_dist_from_top;
		window.second->pos.y = new_y;
	}
}

bool ImguiManager::IsPosInWindow( v2i pos, StrongHandle<ImguiWindow> window ) const
{
	if ( window == nullptr ) return false;

	v2i window_pos = window->pos;
	v2i window_dimen = window->dimen;

	if ( window->minimized || window->locked )
	{
		window_dimen.y = Imgui::TITLE_BAR_HEIGHT;
	}

	const bool pos_in_window = pos.x >= window_pos.x && pos.x <= window_pos.x + window_dimen.x
			&& pos.y <= window_pos.y && pos.y >= window_pos.y - window_dimen.y;

	return pos_in_window;
}

void ImguiManager::DetectConsumeInputs()
{
	const int min_mouse_btn = 1;
	const int max_mouse_btn = 3;
	const v2i mouse_pos = Input::Get()->GetMousePos();

	bool mouse_clicked = false;
	bool mouse_over_any_window = false;

	for( auto window : m_visible_windows )
	{
		// is mouse in 
		const bool mouse_in_window = IsPosInWindow( mouse_pos, window );
		mouse_over_any_window |= mouse_in_window;

		// check all mouse button presses if is in
		for ( int i = min_mouse_btn; i <= max_mouse_btn; i+=1 )
		{
			if ( Input::Ref().DidMouseJustGoDown( i ) )
			{
				mouse_clicked = true;

				// if did not click on active window
				auto active_window = state.activeWindow;
				if ( active_window == nullptr || (mouse_in_window && window != active_window && !IsPosInWindow(mouse_pos, active_window) ) )
				{
					SetActiveWindow( window );
				}

				if ( mouse_in_window )
				{
					Input::Ref().ConsumeMouseClick( i );
					m_consume_mouse_btns.insert( i );
				}
			}
		}
	}

	// if clicked but missed all windows, clear the active window
	if ( mouse_clicked && !mouse_over_any_window ) 
	{
		ClearActiveWindow();
	}

	// if the widget consumes input, check to see if user is typing in widget
	if ( state.doesWidgetConsumeTextInput )
	{
		char c = Input::Ref().GetKeyPressed();

		// did use press on a valid key?
		if ( c != '\0' )
		{
			// grab the scancode
			uint32_t scan_code = Input::Ref().GetCharScanCode( c );

			// consume keys
			Input::Ref().ConsumeKeyPress( scan_code );
			m_consume_key_press.insert( scan_code );
		}
	}
}

bool ImguiManager::DidMouseButtonGoDown( int btn ) const
{
	return m_consume_mouse_btns.find( btn ) != m_consume_mouse_btns.end();
}

bool ImguiManager::DidKeyJustGoDown( const char* key ) const
{
	uint32_t key_hashed = Input::Ref().GetScanCode( key );
	return m_consume_key_press.find( key_hashed ) != m_consume_key_press.end();
}

void ImguiManager::State::Reset()
{
	activeWindow = NULL;
	workingWindow = NULL;
	
	memset( &data, 0, sizeof(data) );
	
	activeWidgetId = State::WIDGET_NULL;
	widgetCount = 0;
	doesWidgetConsumeTextInput = false;
}

}