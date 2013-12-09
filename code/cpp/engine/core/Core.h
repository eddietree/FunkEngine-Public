
#pragma once

namespace funk
{
	class Core
	{
	private:
		
		int m_static_fps; // the fps determined by iphone
		
		float m_ms_update;
		float m_ms_render;

		bool m_check_gl; // checks for gl errors

	public:
		
		Core();
		~Core();
		
		void Init( int a_static_fps );
		void Update( float timeSinceLastUpdate );
		void Draw( float timeSinceLastDraw );
		
		void OnResume();
		void OnPause();
		void OnQuit();
		
		void Gui();
	
	private:
	
		void ClearScreen();
	
	};
	
}

