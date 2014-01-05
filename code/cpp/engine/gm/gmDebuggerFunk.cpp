#include "gmDebuggerFunk.h"

#include <cstring>
#include <common/Debug.h>

#include <gm/gmMachine.h>
#include <gm/gmThread.h>
#include <gm/gmConfig.h>
#include <vm/VirtualMachine.h>

#include <imgui/Imgui.h>
#include <imgui/ImguiGM.h>
#include <imgui/ImguiManager.h>

#include <system/Config.h>
#include <system/Window.h>
#include <system/Input.h>

#include <gfx/GLIncludes.h>
#include <gfx/ShaderManager.h>
#include <gfx/ImmGfx.h>
#include <gfx/RenderState.h>

using namespace funk;

const int DEBUG_WINDOW_PADDING = 20;
const float DEBUG_WINDOW_WIDTH = 0.47f;
const float DEBUG_WINDOW_HEIGHT = 0.6f;
const float DEBUG_THREADVIEWER_WIDTH = 0.25f;
const float DEBUG_THREADVIEWER_HEIGHT = 0.3f;

const char * KEY_PAUSE = "F9";
const char * KEY_STEP_OVER = "F10";
const char * KEY_STEP_INTO = "F11";

void gmDebugSendMsg(gmDebugSession * a_session, const void * a_command, int a_len)
{
	a_session->m_owner->ReceiveMsg(a_command, a_len);
}

const void * gmDebugPumpMsg(gmDebugSession * a_session, int &a_len)
{
	if( a_session->m_owner->HasPacket() )
	{
		const gmDebuggerFunk::DataPacket & packet = a_session->m_owner->GetPacket();
		a_len = packet.len;
		return packet.data;
	}
	
	return NULL;	
}

const char * ConvertStateToStr( gmThread::State state )
{
	switch (state)
	{
	case gmThread::RUNNING:			return "RUNNING";
	case gmThread::SLEEPING:		return "SLEEPING";
	case gmThread::BLOCKED:			return "BLOCKED";
	case gmThread::KILLED:			return "KILLED";
	case gmThread::EXCEPTION:		return "EXCEPTION";
	case gmThread::SYS_PENDING:		return "SYS_PENDING";
	case gmThread::SYS_YIELD:		return "SYS_YIELD";
	case gmThread::SYS_EXCEPTION:	return "SYS_EXCEPTION";
	}

	return "";
}

bool ThreadImgui(gmThread * a_thread, void * a_context)
{
    // NOTE: why this?
	//if ( a_thread->GetState() == gmThread::EXCEPTION ) 
	//{
		//return true;
	//}

    gmFunctionObject* func = a_thread->GetFunction()->GetFunctionObjectSafe();
    ASSERT( func != NULL );

    const gmuint32 source_id = func->GetSourceId();
    const char* funcname = func->GetDebugName();
    const char* filename;
    const char* source;
    char hint[128] = "unknown";

    if ( a_thread->GetMachine()->GetSourceCode(source_id, source, filename) )
    {
        const int len = strlen( filename );

        int a = len;
        int b = len;

        while ( a >= 0 && filename[a] != '/' ) { --a; }
        while ( b >= 0 && filename[b] != '.' ) { --b; }

        if ( a < b )
        {
            sprintf(hint, "%s:%s", filename + a + 1, funcname );
        }
    }

	char buffer[128];
	sprintf( buffer, "Thread %d (%d) - %s (%.2f secs, %u bytes) - %s",
		a_thread->GetId(), 
		a_thread->GetGroup(), 
		ConvertStateToStr(a_thread->GetState()),
		a_thread->GetThreadTime()/1000.0f,
		a_thread->GetSystemMemUsed(),
        hint
		);

	if ( a_thread->GetState() == gmThread::RUNNING ||
         a_thread->GetState() == gmThread::EXCEPTION )
	{
		if ( Imgui::Button(buffer) )
		{
			(*(gmThread**)a_context) = a_thread;
		}
	}
	else
	{
		Imgui::Print(buffer);
	}
	
	return true;
}

bool ThreadFindRunning(gmThread * a_thread, void * a_context)
{
	if ( a_thread->GetState() == gmThread::RUNNING ) 
	{
		(*(gmThread**)a_context) = a_thread;
		return false;
	}

	return true;
}

void gmDebuggerFunk::Open( gmMachine * machine )
{
	m_debugSesh.Open(machine);
	m_debugSesh.m_owner = this;
	m_debugSesh.m_sendMessage = gmDebugSendMsg;
	m_debugSesh.m_pumpMessage = gmDebugPumpMsg;

	memset( &m_debugState, 0, sizeof(m_debugState) );
	m_debugState.threadId = GM_INVALID_THREAD;
	m_showFunctions = false;

	// table traverse
	m_debugState.tableTraverse.Construct( machine, 16 );
	m_debugState.ResetTableSelector();

#if !PLATFORM_MOBILE
	
	// shader
	m_shader_bg = ShaderManager::Ref().GetProgram("common/shaders/gmDebuggerBG.vp", "common/shaders/gmDebuggerBG.fp" );
#endif
	
	m_cam2d.InitScreenSpace();
}

void gmDebuggerFunk::BreakIntoRunningThread()
{
	if ( IsDebugging() ) 
	{
		VirtualMachine::Get()->GetConsole().Log("Cannot break while debugger is running");
		return;
	}

	gmThread * thr = NULL;
	m_debugSesh.GetMachine()->ForEachThread( ThreadFindRunning, &thr );

	// clicked on a thread
	if ( thr && thr->GetState() == gmThread::RUNNING )
	{
		QueuePacket("mbrk", thr->GetId() );
	}
	else
	{
		TRACE("Couldn't break. No RUNNING threads!\n");
	}	
}

void gmDebuggerFunk::Update()
{
	gmMachine * machine = m_debugSesh.GetMachine();

	if ( Input::Get()->DidKeyJustGoDown(KEY_PAUSE) )
	{
		BreakIntoRunningThread();
	}

	// run debug thread
	if( IsDebugging() )
	{
		gmThread * thr = machine->GetThread( m_debugState.threadId );
		if ( thr && thr->m_debugFlags != 0 ) thr->Sys_Execute();
		machine->CollectGarbage(true);
	}

	m_debugSesh.Update();
}

void gmDebuggerFunk::Close()
{
	VirtualMachine::Get()->GetConsole().Enable(false);

	m_texBG = NULL;
	m_debugState.tableTraverse.Destruct(m_debugSesh.GetMachine());
	m_debugSesh.Close();
}

void gmDebuggerFunk::BeginSession()
{
	gmMachine* machine = m_debugSesh.GetMachine();
	machine->GetGlobals()->Set(machine, "g_debugger_on", gmVariable(1));

	m_debugState.jumpToLineNumber = true;
	m_debugState.jumpToLineNumberIfNotInView = false;

	VirtualMachine::Get()->GetConsole().Enable(true);
	VirtualMachine::Get()->GetConsole().Log("Begin debugger session");
	
	const v2i dimen = Window::Get()->Sizei();
	int numBytes = sizeof(unsigned int)*dimen.x*dimen.y;
	unsigned char * buffer = new unsigned char[numBytes];
	memset( buffer, 0, numBytes );
	
	#if !PLATFORM_MOBILE

	// texture
	Texture2dDesc tex_desc;
	tex_desc.format = TEXFORMAT_RGBA;
	tex_desc.data_type = DATATYPE_UNSIGNED_BYTE;
	tex_desc.dimen = Window::Get()->Sizei();
	m_texBG = new Texture2d();
	m_texBG->Init(tex_desc);

	// grab front buffer data
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, dimen.x, dimen.y, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	m_texBG->Bind();
	m_texBG->SubData( buffer, dimen.x, dimen.y );
	m_texBG->Unbind();
	#endif
	
	delete [] buffer;
}

void gmDebuggerFunk::EndSession()
{
	gmMachine* machine = m_debugSesh.GetMachine();
	machine->GetGlobals()->Set(machine, "g_debugger_on", gmVariable(0));

	m_debugState.threadId = GM_INVALID_THREAD;
	m_debugState.sourceId = 0;

	VirtualMachine::Get()->GetConsole().Enable(false);
	VirtualMachine::Get()->GetConsole().Log("End debugger session");
}

void gmDebuggerFunk::HandleRenderException()
{
	VirtualMachine & vm = VirtualMachine::Ref();

	vm.GetConsole().Log("Exception occurred during rendering");
	vm.GetConsole().Log("Render bugs will most likely crash the game");

	// handle Imgui error
	if ( ImguiManager::Get()->state.workingWindow )
	{
		char buffer[128];
		sprintf( buffer, "[ERROR] Exception occurred after Imgui::Begin('%s')", ImguiManager::Get()->state.workingWindow->name );
		vm.GetConsole().Log(buffer);

		Imgui::End();
	}

	// handle ImmGfx error
	if ( ImmGfx::Get()->IsDrawing() )
	{
		char buffer[128];
		sprintf( buffer, "[ERROR] Exception occurred between ImmGfx::Begin/End" );
		vm.GetConsole().Log(buffer);

		ImmGfx::Get()->EndDraw();
	}
}

void gmDebuggerFunk::ReceiveMsg( const void * a_command, int &a_len )
{
	if ( !a_command ) return;

	gmMachine* machine = m_debugSesh.GetMachine();
	const int command = *(int*)a_command;

	switch(command)
	{
		case GM_MAKE_ID32('d','b','r','k'):
		{
			int threadId = *((int*)(a_command)+1);
			int lineNumber = *((int*)(a_command)+3);
			gmuint32 sourceId = *((gmuint32*)(a_command)+2);

			ASSERT( threadId != GM_INVALID_THREAD );

			if ( !IsDebugging() ) BeginSession();

			m_debugState.jumpToLineNumber = (m_debugState.threadId != threadId) || (m_debugState.sourceId != sourceId);
			m_debugState.jumpToLineNumberIfNotInView = (m_debugState.threadId != threadId) || (m_debugState.sourceId != sourceId) || (m_debugState.lineNumber != lineNumber);
			m_debugState.threadId = threadId;
			m_debugState.sourceId = sourceId;
			m_debugState.lineNumber = lineNumber;
			m_debugState.callStackLevel = 0;
			m_debugState.ResetTableSelector();

			break;
		}
		case GM_MAKE_ID32('d','e','x','c'):
		{
			const int threadId = *((int*)(a_command)+1);
			ASSERT( threadId != GM_INVALID_THREAD );
			
			// ignore if console command failed
			if ( threadId == VirtualMachine::Get()->GetConsole().GetLastCmdThread() )
			{
				break;
			}

			// handle rendering exception
			gmVariable varRendering = machine->GetGlobals()->Get(machine, "g_rendering");
			if ( varRendering.IsInt() && varRendering.GetInt() == 1 ) 
			{
				HandleRenderException();
			}

			if ( !IsDebugging() ) BeginSession();			

			// get thread
			m_debugState.threadId = threadId;
			gmThread* thr = machine->GetThread(m_debugState.threadId);

			//const gmStackFrame * frame = thr->GetFrame();
			int base = thr->GetIntBase();
			const gmuint8 * ip = thr->GetInstruction();

			// get the function object
			gmVariable * fnVar = &thr->GetBottom()[base - 1];
			ASSERT(fnVar->m_type == GM_FUNCTION);
			gmFunctionObject * fn = (gmFunctionObject *) GM_MOBJECT(machine, fnVar->m_value.m_ref);

			m_debugState.lineNumber = fn->GetLine(ip);
			m_debugState.sourceId = fn->GetSourceId();
			m_debugState.callStackLevel = 0;
			m_debugState.ResetTableSelector();

			// get filename
			const char *source;
			const char *filename;
			m_debugSesh.GetMachine()->GetSourceCode(m_debugState.sourceId, source, filename);

			char buffer[128];
			sprintf( buffer, "Exception in Thread %d (File '%s', Line %d)", threadId, filename, m_debugState.lineNumber );
			VirtualMachine::Get()->GetConsole().Log(buffer);

			break;
		}
		case GM_MAKE_ID32('d','s','t','p'):
		{
			// Nothing.
		}
	}
}

void gmDebuggerFunk::DrawBG()
{
	// color overlay
	v3 color = v3(0.0f,0.0f,1.0f);
	gmThread * thr = m_debugSesh.GetMachine()->GetThread( m_debugState.threadId );
	if ( thr && thr->GetState() == gmThread::EXCEPTION ) color = v3(1.0f,0.0f,0.0f);
	
#if PLATFORM_MOBILE
	RenderState::Ref().SetClearColor( color.x, color.y, color.z, 1.0f );
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	
#else
	ImmGfx & imm = ImmGfx::Ref();
	
	m_shader_bg->Begin();
	m_texBG->Bind(0);
	m_shader_bg->Uniform("u_tex", m_texBG);
	m_shader_bg->Uniform("u_color", color);
	
	imm.BeginDraw();
	imm.DrawRectTexCoords(v2(-1.0f), v2(2.0f) );
	imm.EndDraw();
	
	m_texBG->Unbind();
	m_shader_bg->End();
#endif	
}

void gmDebuggerFunk::Gui()
{
	if ( IsDebugging() )
	{
		DrawBG();
		GuiSource();
		GuiThread();
		GuiThreadsViewer();
	}
}

void gmDebuggerFunk::GuiSource()
{
	if ( !IsDebugging() ) return;

	const char *source;
	const char *filename;
	m_debugSesh.GetMachine()->GetSourceCode(m_debugState.sourceId, source, filename);

	// Window title
	{
		char titleBuffer[128];
		sprintf(titleBuffer, "Source Code: %s", filename );
		const v2i windowDimen = Window::Get()->Sizei();
		const int width = (int)(windowDimen.x*DEBUG_WINDOW_WIDTH);
		const int height = (int)(windowDimen.y*DEBUG_WINDOW_HEIGHT);

		Imgui::Begin(titleBuffer, 
			v2i(DEBUG_WINDOW_PADDING, height + DEBUG_WINDOW_PADDING),
			v2i(width, height) );
	}

	Imgui::SetSerializable(false);

	int srcLen = strlen(source);
	//int offset = 0;
	int lineNumber = 1;
	const char * textPos = source;
	
	v2i currentLineNumberPos = v2i(0,0);

	// traverse each line
	while( textPos-source < srcLen )
	{
		const char * endPos = strchr( textPos, '\n' );

		if ( endPos == NULL ) endPos = source+srcLen;

		// copy line
		char buffer[256];
		int len = min( (int)(endPos-textPos), (int)sizeof(buffer)-1);
		memcpy(buffer, textPos, len);
		buffer[len] = 0;
		textPos = endPos+1;

		char lineCountBuffer[8];
		sprintf(lineCountBuffer, "%4d:", lineNumber);

		if ( lineNumber == m_debugState.lineNumber )
		{
			v2i pos = Imgui::GetDrawPos();
			Imgui::ColorBlock(NULL, v3(0.2f, 0.4f, 0.2f), v2i(Imgui::GetWindowDimenAutoSize().x - pos.x - 10, Imgui::FONT_HEIGHT + 2));
			Imgui::SetDrawPos(pos);
			Imgui::ColorBlock(NULL, v3(0.2f, 0.6f, 0.2f), v2i(40,Imgui::FONT_HEIGHT + 2));
			Imgui::SetDrawPos(pos);

			if ( m_debugState.jumpToLineNumber || m_debugState.jumpToLineNumberIfNotInView )
				currentLineNumberPos = pos;
		}

		Imgui::Print(lineCountBuffer);
		Imgui::SameLine();
		Imgui::Print(buffer);

		++lineNumber;
	}

	// fixme: setting scroll at this point create a 1 frame lag
	// it is a bit of a tricky issue, but knowing the height of our text we could infer the Y position and set it before Imgui::Begin()
	if ( m_debugState.jumpToLineNumber )
	{
		m_debugState.jumpToLineNumber = false;
		m_debugState.jumpToLineNumberIfNotInView = false;
		Imgui::SetScrollToPosY( currentLineNumberPos.y );
	}
	else if ( m_debugState.jumpToLineNumberIfNotInView )
	{
		m_debugState.jumpToLineNumberIfNotInView = false;
		Imgui::SetScrollToPosYIfNotInView( currentLineNumberPos.y, Imgui::FONT_HEIGHT * 4);
	}

	Imgui::End();
}

bool gmDebuggerFunk::IsDebugging() const
{ 
	return m_debugState.threadId != GM_INVALID_THREAD; 
}

void gmDebuggerFunk::GuiThread()
{
	gmThread * thr = m_debugSesh.GetMachine()->GetThread( m_debugState.threadId );
	
	// occurs when you continue from breakpoint
	if ( thr == NULL || (thr->GetState() != gmThread::EXCEPTION && thr->m_debugFlags == 0) )
	{
		EndSession();		
		return;
	}

	ASSERT( m_debugState.sourceId != 0 );
	gmMachine* machine = m_debugSesh.GetMachine();
	
	// create variables table
	gmTableObject * table = machine->AllocTableObject();
	table->Set(machine, "[globals]", gmVariable( machine->GetGlobals() ) );
	table->Set(machine, "[this]", *thr->GetThis() );
	
	// gather stack-frame local variables
	{
		const gmStackFrame * frame = thr->GetFrame();
		gmVariable * base = thr->GetBase();
		int level = 0;

		while(frame)
		{
			gmVariable * fnVar = base - 1;
			gmFunctionObject * fn = (gmFunctionObject *) GM_MOBJECT(machine, fnVar->m_value.m_ref);
			
			if( level == m_debugState.callStackLevel )
			{
				for(int i = 0; i < fn->GetNumParamsLocals(); ++i )
				{
					table->Set( machine, fn->GetSymbol(i), base[i] );
				}

				break;
			}

			base = thr->GetBottom() + frame->m_returnBase;
			frame = frame->m_prev;
			++level;
		}
	}

	// begin window
	char buffer[256];
	sprintf( buffer, "Thread %d (%s)", m_debugState.threadId, ConvertStateToStr(thr->GetState()) );
	const v2i windowDimen = Window::Get()->Sizei();
	const int width = (int)(windowDimen.x*DEBUG_WINDOW_WIDTH);
	const int height = (int)(windowDimen.y*DEBUG_WINDOW_HEIGHT);
	Imgui::Begin(buffer, 
		v2i(windowDimen.x-DEBUG_WINDOW_PADDING-width, height + DEBUG_WINDOW_PADDING),
		v2i(width, height) );
	
	Imgui::SetSerializable(false);

	Imgui::Header("Thread Control");

	// debug code stepping functions
	if ( thr->GetState() != gmThread::EXCEPTION )
	{
		if ( Imgui::Button("Continue (F9)") || Input::Get()->DidKeyJustGoDown(KEY_PAUSE) )
            QueuePacket( "mrun", m_debugState.threadId );

		Imgui::SameLine();

		if ( Imgui::Button("Step Over (F10)") || Input::Get()->DidKeyJustGoDown(KEY_STEP_OVER) )
            QueuePacket( "msov", m_debugState.threadId );

		Imgui::SameLine();

		if ( Imgui::Button("Step Into (F11)") || Input::Get()->DidKeyJustGoDown(KEY_STEP_INTO) )
            QueuePacket( "msin", m_debugState.threadId );

		Imgui::SameLine();

		if ( Imgui::Button("Step Out") )
            QueuePacket( "msou", m_debugState.threadId );

		Imgui::SameLine();
		Imgui::Print(" _ ");
		Imgui::SameLine();
	}

    if ( thr->GetState() == gmThread::EXCEPTION )
    {
    	if ( Imgui::Button("Force Continue") )
    	{
            // TODO: this should work with packet system for remote debug
            machine->Sys_SwitchState(thr, gmThread::RUNNING);
            QueuePacket("mbrk", m_debugState.threadId);
    	}

        Imgui::SameLine();
    }

	// can kill thread
    if ( thr->GetState() == gmThread::RUNNING )
    {
        Imgui::SameLine();

    	if ( Imgui::Button("Force Exception") )
    	{
            // TODO: this should work with packet system for remote debug
            machine->Sys_SwitchState(thr, gmThread::EXCEPTION);
            // TODO: these are not visible, should we use?
            //gmDebuggerStop(m_debugSesh, thr->GetId());
    	}

        Imgui::SameLine();
    }

	// can kill thread
	if ( Imgui::Button("Kill") )
	{
		machine->Sys_SwitchState(thr, gmThread::KILLED);
		EndSession();
	}
	
	// call-stack
	Imgui::Header("Call Stack");
	{
		const gmStackFrame * frame = thr->GetFrame();
		int base = thr->GetIntBase();
		const gmuint8 * ip = thr->GetInstruction();
		int level = 0;

		while(frame)
		{
			// get the function object
			gmVariable * fnVar = &thr->GetBottom()[base - 1];
			if( fnVar->m_type == GM_FUNCTION )
			{
				gmFunctionObject * fn = (gmFunctionObject *) GM_MOBJECT(machine, fnVar->m_value.m_ref);

				int lineNumber = fn->GetLine(ip);
				gmuint32 sourceId = fn->GetSourceId();
				const char *source;
				const char *filename;
				m_debugSesh.GetMachine()->GetSourceCode(sourceId, source, filename);

				const char * arrowbuffer = level == m_debugState.callStackLevel ? ">> " : "";
				sprintf(buffer, "%s[%d] %s() - %s (%d)", arrowbuffer, level, fn->GetDebugName(), filename, lineNumber );

				if ( Imgui::Button(buffer) )
				{
					m_debugState.ResetTableSelector();
					m_debugState.lineNumber = lineNumber;
					m_debugState.sourceId = sourceId;
					m_debugState.callStackLevel = level;
					m_debugState.jumpToLineNumber = true;
				}
			}

			base = frame->m_returnBase;
			ip = frame->m_returnAddress;
			frame = frame->m_prev;

			++level;
		}
	}

	Imgui::Header("Variables");
	Imgui::CheckBox("Show Functions", m_showFunctions);
	Imgui::Separator();
	funk::ImguiOutputTable( table, &m_debugState.tableTraverse, m_showFunctions );

	// thread info
	sprintf(buffer, "Group Id: %d\nStart Time: %.2f secs\nRunning Time: %.2f secs\nMachine Time: %.2f secs\nSystem Memory Used: %d bytes", 
		thr->GetGroup(), 
		thr->GetStartTime()/1000.0f, 
		thr->GetThreadTime()/1000.0f, 
		machine->GetTime()/1000.0f, 
		thr->GetSystemMemUsed() );

	Imgui::Header("Thread Info");
	Imgui::PrintParagraph(buffer);

	Imgui::End();
}

void gmDebuggerFunk::GuiThreadsViewer()
{
	gmMachine* machine = m_debugSesh.GetMachine();

	const v2i windowDimen = Window::Get()->Sizei();
	const int width = (int)(windowDimen.x*DEBUG_THREADVIEWER_WIDTH);
	const int height = (int)(windowDimen.y*DEBUG_THREADVIEWER_HEIGHT);
	Imgui::Begin("Threads Viewer", 
		v2i(windowDimen.x - width - DEBUG_WINDOW_PADDING, windowDimen.y - DEBUG_WINDOW_PADDING), 
		v2i(width, height) );

	Imgui::SetSerializable(false);

	gmThread * returnThread = NULL;
	machine->ForEachThread( ThreadImgui, &returnThread );

	// clicked on a thread
	if ( returnThread && returnThread->GetId() != m_debugState.threadId )
	{
		gmThread * thr = machine->GetThread(  m_debugState.threadId );
		// NOTE: why was this doing this on press?
		//if ( thr->GetState() == gmThread::EXCEPTION )
		//{
			//machine->Sys_SwitchState(thr, gmThread::KILLED);
		//}
		//else
		//{
			//QueuePacket( "mrun", m_debugState.threadId );
		//}
				
		QueuePacket( "mbrk", returnThread->GetId() );
		m_debugState.threadId = returnThread->GetId();
	}

	Imgui::End();
}

const gmDebuggerFunk::DataPacket & gmDebuggerFunk::GetPacket()
{
	ASSERT( !m_packets.empty() );

	DataPacket &packet = m_packets.front();
	memcpy( &m_currPacket, &packet, sizeof(DataPacket) );
	m_packets.pop();

	return m_currPacket;
}

void gmDebuggerFunk::QueuePacket( const char * msg )
{
	DataPacket &packet = CreatePacket();
	packet.len = 4;

	memcpy( packet.data, msg, 4 );
}

void gmDebuggerFunk::QueuePacket( const char * msg, int id )
{
	DataPacket &packet = CreatePacket();
	packet.len = 8;

	memcpy( packet.data, msg, 4 );
	memcpy( packet.data+4, &id, 4 );
}

void gmDebuggerFunk::QueuePacket( const char * msg, int id, int data )
{
	DataPacket &packet = CreatePacket();
	packet.len = 12;

	memcpy( packet.data, msg, 4 );
	memcpy( packet.data+4, &id, 4 );
	memcpy( packet.data+8, &data, 4 );
}

gmDebuggerFunk::DataPacket& gmDebuggerFunk::CreatePacket()
{
	m_packets.push( DataPacket() );
	DataPacket &packet = m_packets.back();
	return packet;
}

void gmDebuggerFunk::DebugState::ResetTableSelector()
{
	for( int i = 0; i < tableTraverse.Size(); ++i )
	{
		tableTraverse.SetAt( i, gmVariable(-1) );
	}
}