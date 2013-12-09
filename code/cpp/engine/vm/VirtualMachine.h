
#pragma once

#include <common/Singleton.h>
#include <common/StrongHandle.h>

#include <gm/gmVariable.h>
#include <gm/gmDebuggerFunk.h>
#include <gfx/LineGraph.h>

#include <unordered_map>

#include "VirtualConsole.h"

class gmMachine;

namespace funk
{
	class VirtualMachine : public Singleton<VirtualMachine>
	{
	private:

		float	m_dt;
		bool	m_bUseGmByteCode; // uses bytecode version
		int		m_numThreads;
		
		gmMachine *m_vm;
		int m_threadId;
		
		// draw manager
		gmFunctionObject* m_drawFunc;
		gmFunctionObject* m_clearFunc;
		gmVariable m_drawManager;
		gmDebuggerFunk m_debugger;
		
		struct ThreadAllocationItem
		{
			int allocations;
			int eraseCountdown;
		};
		std::unordered_map<const gmFunctionObject*, ThreadAllocationItem> m_threadAllocationsHistory;
		bool m_freezeThreadAllocationsGui;
		bool m_showThreadAllocationsGui;
		bool m_showSettingsGui;

		// analytics
		StrongHandle<LineGraph> m_lineGraphUpdate;
		StrongHandle<LineGraph> m_lineGraphDraw;
		StrongHandle<LineGraph> m_lineGraphMemory;
		float	m_updateMs;
		float	m_drawMs;
		
		VirtualConsole m_console;

	public:
		void Update();
		void Render();
		void RunMain(int a_fps);
		void GuiStats();
		void Gui();

		// calls GM func: g_core.a_func_name(a_param);
		void CallCoreFunc( const char * a_func_name, gmVariable *a_param = nullptr );

		gmMachine & GetVM () { return *m_vm; }
		VirtualConsole &GetConsole() { return m_console; }

		bool IsUsingByteCode() const { return m_bUseGmByteCode; }

	private:
	
		// gui
		void InitGuiSettings();
		void GuiSettings();

		// thread allocations gui
		void InitGuiThreadAllocations();
		void GuiThreadAllocations();

		void HandleErrors();
		void ResetVM();
		
		void InitDrawManager();
		void InitGlobals();
		
		void SaveImguiState();
		void LoadImguiState();

		friend Singleton<VirtualMachine>;
		VirtualMachine();
		~VirtualMachine();
	};
}
