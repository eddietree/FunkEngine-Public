
#pragma once

#include <common/Singleton.h>
#include <math/v2.h>
#include <math/v2i.h>

namespace funk
{
	enum DeviceOrientation
	{
		DEVICE_ORIENTATION_PORTRAIT,
		DEVICE_ORIENTATION_LANDSCAPE,
	};
	
	class Window : public Singleton<Window>
	{
	private:
		v2i m_dimen;
		v2i m_dimen_monitor;
		DeviceOrientation m_orientation;
		
	public:
		v2i Sizei() const { return m_dimen; }
		v2 Sizef() const { return funk::v2((float)m_dimen.x, (float)m_dimen.y); }
		DeviceOrientation getOrientation() const { return m_orientation; }
		
		// monitor
		v2i MonitorSizei() const { return m_dimen_monitor; }
		v2 MonitorSizef() const { return funk::v2((float)m_dimen_monitor.x, (float)m_dimen_monitor.y); }
		
		void SetDimen( int a_screen_width, int a_screen_height ) { m_dimen = v2i(a_screen_width, a_screen_height); }
		void SetDimenMonitor( int a_screen_width, int a_screen_height ) { m_dimen_monitor = v2i(a_screen_width, a_screen_height); }
		
		void OnRotateOrientation( DeviceOrientation a_orientation, int a_screen_width, int a_screen_height );

        bool IsFullScreen() const;
        void SetTitle(const char* title);
		void ShowMouseCursor( bool a_enable );

		void Exit();
		
	private:
		friend Singleton<Window>;
		Window();
		~Window();
	};
}
