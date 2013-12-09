#include "VirtualMachine.h"

#include <system/FileUtil.h>
#include <system/TouchInputManager.h>
#include <system/Input.h>
#include <system/Config.h>
#include <system/Timer.h>
#include <common/Debug.h>
#include <system/Config.h>
#include <system/Window.h>

#include <hardware/leap/LeapController.h>
#include <audio/fmod/FmodSystem.h>

#include <gm/gmMachine.h>
#include <gm/gmThread.h>
#include <gm/gmDebuggerFunk.h>
#include <gm/gmUtilEx.h>
#include <gm/gmCall.h>
#include <imgui/ImguiManager.h>
#include <imgui/Imgui.h>

#include <map>

#include "VirtualMachineLibs.h"

namespace funk
{
const char * kEntryFile = "common/gm/Core.gm";
const char * kImguiSaveStateFile = "common/gm/user/imgui_state.gm";

VirtualMachine::VirtualMachine()
{
	m_vm = new gmMachine();
	m_vm->SetAutoMemoryUsage(false);

	m_dt = 1.0f/30.0f;
	m_updateMs = 0.0f;
	m_drawMs = 0.0f;
	m_bUseGmByteCode = false;
	m_numThreads = 0;
	m_threadId = 0;
	m_bUseGmByteCode = false;

	InitGuiSettings();
	InitGuiThreadAllocations();

	LOG("VirtualMachine initialized");
}

VirtualMachine::~VirtualMachine()
{
	m_console.Log("Destructing Virtual Machine..");
	SaveImguiState();

	if ( m_vm->GetDebugMode() ) m_debugger.Close();
	delete m_vm;

	m_console.Log("Virtual Machine destructed!");
}

void VirtualMachine::CallCoreFunc( const char * a_func_name, gmVariable *a_param )
{
	gmVariable core_key = gmVariable(m_vm->AllocStringObject("g_core"));
	gmTableObject * core_obj = m_vm->GetGlobals()->Get(core_key).GetTableObjectSafe();
	ASSERT(core_obj);

	// grab g_core.OnExit() function
	gmFunctionObject* core_exit_func = core_obj->Get(m_vm, a_func_name).GetFunctionObjectSafe();
	ASSERT(core_exit_func);

	// call the function
	gmCall call;
	call.BeginFunction( m_vm, core_exit_func, gmVariable(core_obj) );
	if ( a_param != nullptr ) call.AddParam( *a_param );
	call.End();
}

void VirtualMachine::Update()
{
	HandleErrors();
	
#if !defined(FUNK_FINAL)
	if( Input::Get()->DidKeyJustGoDown("F5") 
		|| (Input::Get()->IsKeyDown("LCTRL") && Input::Get()->DidKeyJustGoDown("R") ) )
	{
		ResetVM();
	}
#endif

	// run main game only if not debugging
	if ( !(m_vm->GetDebugMode() && m_debugger.IsDebugging()) )
	{
		Timer gmTimer;
		gmuint32 delta = (gmuint32)(m_dt*1000.0f);
		m_numThreads = m_vm->Execute( delta );
		m_updateMs = gmTimer.GetTimeMs();
	}

	// update debugger
	if ( m_vm->GetDebugMode() ) 
	{
		m_debugger.Update();
	}
}

void VirtualMachine::ResetVM()
{
	SaveImguiState();

	m_console.ClearText();
	m_console.Log("-----------------------------");
	m_console.Log("Restarting Virtual Machine..." );

	if ( m_vm->GetDebugMode() ) m_debugger.Close();

    // function references in here will become invalid
    m_threadAllocationsHistory.clear();

	Timer timer;

	ImguiManager::Get()->ClearAllWindows();
	m_vm->ResetAndFreeMemory();
	m_vm->Init();

	// need to re-instantiate fmod because harsh F5 resets sometimes doesn't free up all fmod resources
	#if defined(FMOD_ENABLED)
	FmodSystem::DestroyInst();
	FmodSystem::CreateInst();
	#endif

	if ( m_vm->GetDebugMode() ) m_debugger.Open(m_vm);

	RunMain(int(1.0f/m_dt));

	// log
	char buffer[128];
	sprintf( buffer, "Restarting Virtual Machine completed in %f seconds", timer.GetTimeSecs() );
	m_console.Log( buffer );
}

void VirtualMachine::Render()
{
	if ( !m_drawManager.IsNull() )
	{
		// draw only if not debugging
		if ( !m_vm->GetDebugMode()  || !m_debugger.IsDebugging() ) // PORT_TODO
		{
			Timer gmTimer;

			m_vm->GetGlobals()->Set( m_vm, "g_rendering", gmVariable(1) );
			m_vm->ExecuteFunction(m_drawFunc, 0, true, &m_drawManager);
			m_vm->GetGlobals()->Set( m_vm, "g_rendering", gmVariable(0) );

			m_drawMs = gmTimer.GetTimeMs();
		}
		else
		{
			m_vm->ExecuteFunction(m_clearFunc, 0, true, &m_drawManager);
		}
	}
}

void VirtualMachine::RunMain(int a_fps)
{
	int gm_debug_mode = 1; // enables debugger/console, etc
	int run_gm_bytecode = 0; // precompiled bytecode..
	
#if defined(FUNK_FINAL)
	// Final version runs non-debug and bytecode..
	gm_debug_mode = 0;
	run_gm_bytecode = 1;
#endif
	
	// gamemonkey Garbage collector settings
	int gcWorkPerIncrement = 400;
	int gcDestructsPerIncrement = 250;
    int memUsageSoft = 1024 * 1024 * 8;
    int memUsageHard = 1024 * 1024 * 10;

#if defined(PORT_TODO)

	IniReader ini("common/ini/main.ini");
	gcWorkPerIncrement = ini.GetInt("VirtualMachine", "GC_WorkPerIncrement");
	gcDestructsPerIncrement = ini.GetInt("VirtualMachine", "GC_DestructPerIncrement");
	memUsageSoft = ini.GetInt("VirtualMachine", "MemUsageSoft");
	memUsageHard = ini.GetInt("VirtualMachine", "MemUsageHard");
	
#endif // PORT_TODO

	// gamemonkey Garbage collector settings
	m_vm->GetGC()->SetWorkPerIncrement(gcWorkPerIncrement);
	m_vm->GetGC()->SetDestructPerIncrement(gcDestructsPerIncrement);
	m_vm->SetDesiredByteMemoryUsageSoft(memUsageSoft);
	m_vm->SetDesiredByteMemoryUsageHard(memUsageHard);

	// print current setting
	char buffer[128];
	sprintf(buffer, "Running at %d hz, VM Debug Mode: %d, VM Run Byte-Code: %d", a_fps, gm_debug_mode, run_gm_bytecode );
	m_console.Log(buffer);
	sprintf(buffer, "GC Works Per Increment: %d, GC Destructs Per Increment: %d", m_vm->GetGC()->GetWorkPerIncrement(),  m_vm->GetGC()->GetDestructPerIncrement() );
	m_console.Log(buffer);
	sprintf(buffer, "Mem Usage Soft: %d bytes, Mem Usage Hard: %d bytes", m_vm->GetDesiredByteMemoryUsageSoft(), m_vm->GetDesiredByteMemoryUsageHard() );
	m_console.Log(buffer);

	m_dt = 1.0f/a_fps;
	m_vm->SetDebugMode(gm_debug_mode==1);
	m_bUseGmByteCode = run_gm_bytecode != 0;

	if ( m_vm->GetDebugMode() )
        m_debugger.Open(m_vm);

	RegisterLibs(m_vm);
	InitGlobals();
	
	m_threadId = gmCompileStr(m_vm, kEntryFile);	
	TRACE("Entry GM file: '%s', Thread: %d\n", kEntryFile, m_threadId );

	InitDrawManager();
	TouchInputManager::Ref().InitGM();

	#if defined(ENABLE_LEAP)
	LeapController::Ref().InitGM();
	#endif

	LoadImguiState();
}

void VirtualMachine::InitDrawManager()
{
	// get draw manager
	gmVariable drawManagerKey = gmVariable(m_vm->AllocStringObject("g_drawManager"));
	gmTableObject* drawManager = m_vm->GetGlobals()->Get(drawManagerKey).GetTableObjectSafe();
	m_drawManager.Nullify();
	
	if ( drawManager )
	{
		m_drawManager = gmVariable(gmVariable(drawManager));
		
		// grab draw functions
		gmVariable drawKey = gmVariable(m_vm->AllocStringObject("Draw"));
		m_drawFunc = drawManager->Get(drawKey).GetFunctionObjectSafe();
		gmVariable clearKey = gmVariable(m_vm->AllocStringObject("Clear"));
		m_clearFunc = drawManager->Get(clearKey).GetFunctionObjectSafe();
	}
}

void VirtualMachine::InitGlobals()
{
	#if !defined(CONFIG_USER_GM)
		#error WARNING: You need to make 'code/cpp/engine/config/UserConfig.h'
	#endif

	m_vm->GetGlobals()->Set( m_vm, "g_dt", gmVariable(m_dt) );
	m_vm->GetGlobals()->Set( m_vm, "g_rendering", gmVariable(0) );
	m_vm->GetGlobals()->Set( m_vm, "g_debugger_on", gmVariable(0) );

#if defined(FUNK_FINAL)
	m_vm->GetGlobals()->Set( m_vm, "g_build_final", gmVariable(1) );
	m_vm->GetGlobals()->Set( m_vm, "g_user", gmVariable( m_vm->AllocStringObject("UNNAMED") ) );
#else
	m_vm->GetGlobals()->Set( m_vm, "g_build_final", gmVariable(0) );
	m_vm->GetGlobals()->Set( m_vm, "g_user", gmVariable( m_vm->AllocStringObject(CONFIG_USER_GM) ) );
#endif
	
#if PLATFORM_MOBILE
	m_vm->GetGlobals()->Set( m_vm, "g_config_is_mobile", gmVariable(1) );
#else
	m_vm->GetGlobals()->Set( m_vm, "g_config_is_mobile", gmVariable(0) );
#endif
}

void VirtualMachine::HandleErrors()
{
	bool firstErr = true;

	gmLog & compileLog = m_vm->GetLog();
	const char *msg = compileLog.GetEntry(firstErr);
	
	if ( msg )
	{
		const char * textHeader = "#############################\n[GameMonkey Run-time Error]:";
		m_console.Log(textHeader); 
		LOG("%s",textHeader);
	}

	while(msg)	
	{
		//LOG_SAMELINE("%s",msg);
		m_console.Log(msg, false);
		msg = compileLog.GetEntry(firstErr);
	}
	
	compileLog.Reset();
}

void VirtualMachine::GuiStats()
{
	Imgui::Header("GameMonkey");
	Imgui::FillBarFloat("Update", m_updateMs, 0.0f, 16.0 );
	Imgui::FillBarFloat("Draw", m_drawMs, 0.0f, 16.0 );
	Imgui::FillBarInt("Mem Usage (Bytes)", m_vm->GetCurrentMemoryUsage(), 0, m_vm->GetDesiredByteMemoryUsageHard() );
	Imgui::FillBarInt("Num Threads", m_numThreads, 0, 500 );
	Imgui::CheckBox("Show Settings", m_showSettingsGui );
	Imgui::CheckBox("Show Allocations", m_showThreadAllocationsGui );
}

void VirtualMachine::Gui()
{
	gmVariable imgui_mngr_key = gmVariable(m_vm->AllocStringObject("g_imgui_mngr"));
	gmVariable imgui_mngr_var = m_vm->GetGlobals()->Get(imgui_mngr_key);

	if ( !imgui_mngr_var.IsNull() )
	{
		gmTableObject * imgui_mngr_obj = m_vm->GetGlobals()->Get(imgui_mngr_key).GetTableObjectSafe();
		ASSERT(imgui_mngr_obj);

		// grab g_core.OnExit() function
		gmFunctionObject* draw_guis_func = imgui_mngr_obj->Get(m_vm, "Draw").GetFunctionObjectSafe();
		ASSERT(draw_guis_func);

		// call the function
		gmCall call;
		call.BeginFunction( m_vm, draw_guis_func, gmVariable(imgui_mngr_obj) );
		call.End();
	}

	if ( m_vm->GetDebugMode() ) m_debugger.Gui();
	if ( m_showSettingsGui ) GuiSettings();
	if ( m_showThreadAllocationsGui ) GuiThreadAllocations();
	m_console.Gui();
}

void VirtualMachine::InitGuiSettings()
{
	m_showSettingsGui = false;

	const float minVal = 0.0f;
	const float maxVal = 16.0f;
	const int width = 200;
	const int height = 100;
	const int numVals = 128;

	m_lineGraphUpdate	= new LineGraph( minVal, maxVal, v2i(width, height), numVals );
	m_lineGraphDraw		= new LineGraph( minVal, maxVal, v2i(width, height), numVals );
	m_lineGraphMemory	= new LineGraph( minVal, maxVal, v2i(width, height), numVals );
}

void VirtualMachine::GuiSettings()
{
	m_lineGraphUpdate->PushVal( m_updateMs );
	m_lineGraphUpdate->SetMaxVal(m_dt*1000.0f);
	m_lineGraphDraw->PushVal( m_drawMs );
	m_lineGraphDraw->SetMaxVal(m_dt*1000.0f);
	m_lineGraphMemory->SetMaxVal( (float)m_vm->GetDesiredByteMemoryUsageHard() );
	m_lineGraphMemory->PushVal( (float)m_vm->GetCurrentMemoryUsage() );

	int workPerIncrement = m_vm->GetGC()->GetWorkPerIncrement();
	int destructPerIncrement = m_vm->GetGC()->GetDestructPerIncrement();
	int memUsageSoft = m_vm->GetDesiredByteMemoryUsageSoft();
	int memUsageHard = m_vm->GetDesiredByteMemoryUsageHard();

	const v2i pos = v2i(300, Window::Get()->Sizei().y - 20 );

	Imgui::Begin("GameMonkey Settings", pos);
	Imgui::Print("Update");
	Imgui::LineGraph( m_lineGraphUpdate.Ref() );
	Imgui::Print("Draw");
	Imgui::LineGraph( m_lineGraphDraw.Ref() );
	Imgui::Print("Memory");
	Imgui::LineGraph( m_lineGraphMemory.Ref() );
	Imgui::FillBarInt("Mem Usage (Bytes)", m_vm->GetCurrentMemoryUsage(), 0, m_vm->GetDesiredByteMemoryUsageHard() );
	Imgui::Header("Garbage Collector");
	if ( Imgui::Button("Force Full Collect") ) { m_vm->CollectGarbage(true); }
	Imgui::SliderInt( "Work Per Increment", workPerIncrement, 1, 600 );
	Imgui::SliderInt( "Destructs Per Increment", destructPerIncrement, 1, 600 );
	Imgui::SliderInt( "Mem Usage Soft", memUsageSoft, 200000, memUsageHard );
	Imgui::SliderInt( "Mem Usage Hard", memUsageHard, memUsageSoft+500, memUsageSoft+200000 );
	Imgui::Separator();
	Imgui::FillBarInt("GC Warnings", m_vm->GetStatsGCNumWarnings(), 0, 200 );
	Imgui::FillBarInt("GC Full Collects", m_vm->GetStatsGCNumFullCollects(), 0, 200 );
	Imgui::FillBarInt("GC Inc Collects", m_vm->GetStatsGCNumIncCollects(), 0, 200 );
	Imgui::End();

	m_vm->SetDesiredByteMemoryUsageSoft(memUsageSoft);
	m_vm->SetDesiredByteMemoryUsageHard(memUsageHard);
	m_vm->GetGC()->SetWorkPerIncrement(workPerIncrement);
	m_vm->GetGC()->SetDestructPerIncrement(destructPerIncrement);
}

void VirtualMachine::InitGuiThreadAllocations()
{
	m_showThreadAllocationsGui = false;
	m_freezeThreadAllocationsGui = false;
}

void VirtualMachine::GuiThreadAllocations()
{
	const v2i pos = v2i(300, Window::Get()->Sizei().y - 20 );

	Imgui::Begin("Thread Allocations (Live)", pos);
	m_freezeThreadAllocationsGui = Imgui::CheckBox("Freeze", m_freezeThreadAllocationsGui);

	if (!m_freezeThreadAllocationsGui)
	{
		auto i = m_threadAllocationsHistory.begin();
		while (i != m_threadAllocationsHistory.end())
		{
			i->second.eraseCountdown -= 1;

			if (i->second.eraseCountdown <= 0)
			{
				i = m_threadAllocationsHistory.erase(i);
			}
			else
			{
				++i;
			}
		}
	}

	if (!m_freezeThreadAllocationsGui)
	{
		auto a = m_vm->GetAllocCountIteratorBegin();
		auto b = m_vm->GetAllocCountIteratorEnd();
		for (; a != b; ++a)
		{
			ThreadAllocationItem& item = m_threadAllocationsHistory[a->first];
			item.allocations = a->second;
			item.eraseCountdown = 300;
		}
	}

	{
		auto a = m_threadAllocationsHistory.begin();
		auto b = m_threadAllocationsHistory.end();
		for (; a != b; ++a)
		{
			const gmFunctionObject* func = a->first;
			const gmuint32 sourceid = func->GetSourceId();
			const char* sourcecode = NULL;
			const char* filename = NULL;
			m_vm->GetSourceCode(sourceid, sourcecode, filename);

            if ( filename == NULL ) 
                filename = "<unknown>";

			int allocations = a->second.allocations;
			if (allocations <= 0)
				continue;

			char buffer[256] = { '.' };
            const char* filename_clipped = filename + std::max<int>(0, strlen(filename) - 40);
			const char* fname = func->GetDebugName();
			const char* status = a->second.eraseCountdown == 300 ? "new" : "old";
			sprintf(buffer, "%s:%s [%s]", filename_clipped, fname, status);
			int len = strlen(buffer);
			memset( &buffer[len], '.', sizeof(buffer)-len-1);
			sprintf( buffer+64, "%d", allocations );
			//itoa(allocations, buffer+50, 10);

			Imgui::Print(buffer);
		}
	}

	Imgui::End();
}

void VirtualMachine::SaveImguiState()
{
	#if !defined(FUNK_FINAL) && PLATFORM_MOBILE==0

	gmTableObject * output_table = m_vm->AllocTableObject();
	
	const auto window_list = ImguiManager::Ref().GetWindowList();
	for( const auto it : window_list )
	{
		const StrongHandle<ImguiWindow> &window = it.second;

		if ( window->isSerializable )
		{
			// build params for table
			gmTableObject * params_table = m_vm->AllocTableObject();
			params_table->Set(m_vm, "pos", gmVariable( window->pos ) );
			params_table->Set(m_vm, "minimized", gmVariable( window->minimized ? 1: 0 ) );
			params_table->Set(m_vm, "name", gmVariable(m_vm->AllocStringObject(window->name)) );

			if ( !window->autosize )
			{
				params_table->Set(m_vm, "dimen", gmVariable( window->dimen ) );
			}

			// set output table
			output_table->Set( m_vm, output_table->Count(), gmVariable(params_table) );
		}
	}

	gmSaveTableToFile( output_table, kImguiSaveStateFile );
	output_table->ClearTable( m_vm );

	LOG( "Saved Imgui state to file '%s'", kImguiSaveStateFile );

	#endif
}

void VirtualMachine::LoadImguiState()
{
	#if !defined(FUNK_FINAL) && PLATFORM_MOBILE==0
	
	if ( !FileExists(kImguiSaveStateFile) ) return;

	// load file contents
	gmTableObject * params_table = gmLoadTableFromFile( m_vm, kImguiSaveStateFile );
	ASSERT( params_table != nullptr );

	gmTableIterator it;
	gmTableNode * window_data = params_table->GetFirst( it );

	// grab screen dimensions
	v2i screen_dimen = Window::Ref().Sizei();

	while( !params_table->IsNull(it) )
	{
		gmTableObject * window_table = window_data->m_value.GetTableObjectSafe();

		// grab window
		char buffer[128];
		gmVariable window_name_var = window_table->Get( m_vm, "name" );
		ASSERT( !window_name_var.IsNull() );
		const char * window_name = window_name_var.AsString( m_vm, buffer, sizeof(buffer) );
		ASSERT(window_name != nullptr);
		StrongHandle<ImguiWindow> window = ImguiManager::Ref().GetWindow( window_name );

		// set position
		gmVariable pos_var = window_table->Get( m_vm, "pos" );
		if ( !pos_var.IsNull() && pos_var.IsV2i() )
		{
			v2i window_pos = pos_var.GetV2i();

			const int padding = 100;
			int pos_x = min( window_pos.x, screen_dimen.x-padding );
			int pos_y = min( window_pos.y, screen_dimen.y );

			window->pos = v2i( pos_x, pos_y );
		}

		// dimen
		gmVariable dimen_var = window_table->Get( m_vm, "dimen" );
		if ( !dimen_var.IsNull() && pos_var.IsV2i() ) window->dimen = dimen_var.GetV2i();
		else window->dimen = v2i(-1);

		// set minimized
		gmVariable minimized_var = window_table->Get( m_vm, "minimized" );
		if ( !minimized_var.IsNull() )
		{
			window->minimized = minimized_var.GetIntSafe() == 0 ? false : true;
		}

		// next window
		window_data = params_table->GetNext(it);
	}

	LOG( "Loaded Imgui state from file '%s'", kImguiSaveStateFile );

	#endif 
}

}