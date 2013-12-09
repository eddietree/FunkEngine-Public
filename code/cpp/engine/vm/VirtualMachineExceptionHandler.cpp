#include "VirtualMachineExceptionHandler.h"

#include <gfx/ImmGfx.h>
#include <imgui/ImguiManager.h>
#include <imgui/Imgui.h>
#include <common/Profiler.h>

namespace funk
{
	void OnGmExceptionTriggered()
	{
		// handle exception in immgfx
		if ( ImmGfx::InstExists() && ImmGfx::Ref().IsDrawing() ) 
			ImmGfx::Ref().EndDraw();

		// handle exception in imgui call
		if ( ImguiManager::InstExists() && ImguiManager::Ref().state.workingWindow != nullptr )
		{
			Imgui::End();
			ImguiManager::Ref().state.workingWindow = nullptr;
		}

		// clear profiler
		if ( Profiler::InstExists() )
		{
			Profiler::Ref().Clear();
		}

	}
}