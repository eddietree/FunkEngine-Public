#include "Core.h"

#include <math/MVP.h>
#include <vm/VirtualMachine.h>
#include <common/Debug.h>
#include <common/Log.h>
#include <gfx/RenderState.h>
#include <gfx/ImmGfx.h>
#include <gfx/ShaderManager.h>
#include <gfx/GLDebug.h>
#include <system/TouchInputManager.h>
#include <system/Config.h>
#include <system/Window.h>
#include <system/Timer.h>
#include <system/Input.h>
#include <imgui/ImguiManager.h>
#include <imgui/ImgUI.h>
#include <audio/SoundManager.h>
#include <audio/fmod/FmodSystem.h>

#include <hardware/leap/LeapController.h>

namespace funk
{
	Core::Core()
	{
		m_check_gl = false;
	}

	Core::~Core()
	{	
		Timer timer;

		VirtualMachine::Ref().CallCoreFunc("OnExit");

		TouchInputManager::DestroyInst();
		ImmGfx::DestroyInst();
		VirtualMachine::DestroyInst();
		ImguiManager::DestroyInst();
		SoundManager::DestroyInst();
		ShaderManager::DestroyInst();
		MVP::DestroyInst();
		Input::DestroyInst();
		RenderState::DestroyInst();

		#if defined(FMOD_ENABLED)
		FmodSystem::DestroyInst();
		#endif

		#if defined(ENABLE_LEAP)
		LeapController::DestroyInst();
		#endif

		LOG( "Core destructed in %f seconds", timer.GetTimeSecs() );
		Log::DestroyInst();
	}
	
	void Core::Init( int a_static_fps )
	{
		m_static_fps = a_static_fps;

		Timer timer;
		{
			// might be created before Core::Init..
			if ( !Log::InstExists() ) Log::CreateInst();

			#if defined(ENABLE_LEAP)
			LeapController::CreateInst();
			#endif

			#if defined(FMOD_ENABLED)
			FmodSystem::CreateInst();
			#endif

			RenderState::CreateInst();
			Input::CreateInst();
			MVP::CreateInst();
			ShaderManager::CreateInst();
			ImmGfx::CreateInst();
			ImguiManager::CreateInst();
			SoundManager::CreateInst();
			VirtualMachine::CreateInst();
			TouchInputManager::CreateInst();

			// boot VM
			Timer vm_timer;
			VirtualMachine::Ref().RunMain(m_static_fps);
			LOG( "VirtualMachine.RunMain() boot time: %f seconds", vm_timer.GetTimeSecs() );
		}
		LOG( "Core::Init() - Total boot time: %f seconds", timer.GetTimeSecs() );
	}
	
	void Core::Update( float timeSinceLastUpdate )
	{		
		// cleanup
		TouchInputManager::Ref().ClearTouchListeners();

		#if defined(ENABLE_LEAP)
		LeapController::Ref().Update();
		#endif
		
		Timer timer;
		VirtualMachine::Ref().Update();
		m_ms_update = timer.GetTimeMs();

		#if defined(FMOD_ENABLED)
		FmodSystem::Ref().Update(timeSinceLastUpdate);
		#endif

		SoundManager::Ref().Update( timeSinceLastUpdate );
	}
	
	void Core::Draw( float timeSinceLastDraw )
	{	
		Timer timer;
		{
			VirtualMachine::Ref().Render();
		}
		m_ms_render = timer.GetTimeMs();
		
		#if !defined(FUNK_FINAL)
		if ( m_check_gl ) CheckGLError();
		#endif
	}
	
	void Core::OnResume()
	{
		
	}
	
	void Core::OnPause()
	{
		
	}
	
	void Core::OnQuit()
	{		
	}
	
	void Core::Gui()
	{
		#if !defined(FUNK_FINAL)

		ImguiManager::Ref().PreUpdate();

		// shows all gui windows registered in GM
		// also shows debugger and console gui
		VirtualMachine::Ref().Gui();

		// show analaytics
		static bool s_show_gui = false;
		if ( Input::Ref().DidKeyJustGoDown("F1") ) s_show_gui = !s_show_gui;

		if ( s_show_gui )
		{
			const int padding = 5;
			const v2i windowDimen = Window::Get()->Sizei();
			const float deltaMs = 1000.0f / m_static_fps;
		
			Imgui::Begin("Analytics", v2i(padding, windowDimen.y-padding) );
			Imgui::SetSerializable(false);
			Imgui::FillBarFloat("Total", m_ms_update+m_ms_render, 0.0f, deltaMs );
			Imgui::FillBarFloat("Update", m_ms_update, 0.0f, deltaMs );
			Imgui::FillBarFloat("Render", m_ms_render, 0.0f, deltaMs );

			Imgui::Separator();
			Imgui::CheckBox( "GL Error Check", m_check_gl );

		#if !PLATFORM_MOBILE
			//Imgui::FillBarFloat("GPU", m_msGpu, 0.0f, deltaMs );
			//Imgui::CheckBox("Show Graphs", m_showGraphsGui );
			VirtualMachine::Get()->GuiStats();
			//SoundMngr::Get()->GuiStats();
			ShaderManager::Get()->Gui();

			#if defined(FMOD_ENABLED)
			FmodSystem::Ref().Gui();
			#endif
		#endif

			Imgui::End();	
		}

		ImguiManager::Ref().PostUpdate();

		#endif
	}

}