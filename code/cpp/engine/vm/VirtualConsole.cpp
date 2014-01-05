#include "VirtualConsole.h"

#include <time.h>
#include <assert.h>

#include <common/Debug.h>
#include <system/Config.h>
#include <system/Input.h>
#include <system/Window.h>
//#include <common/IniReader.h>
#include <imgui/Imgui.h>

#include <gm/gmMachine.h>
#include <gm/gmThread.h>

#include "VirtualMachine.h"

namespace funk
{
const float CONSOLE_WIDTH = 0.4f;
const float CONSOLE_HEIGHT = 0.3f;
const int CONSOLE_PADDING = 20;
const int CONSOLE_INPUT_PADDING = 30;

const int MAX_OLD_CMDS = 16;
const char * LOG_FILE = "LOG.txt";

VirtualConsole::VirtualConsole() : m_enable(false), m_queueScrollBottom(false), m_lastCmdThread(0)
{
	m_cmdIndex = 0;
	m_text.reserve(16384);

	// grab current time
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	LOG("VirtualConsole initialized");
	LOG("Hello user '%s'!", CONFIG_USER_GM);
	LOG("The current local time is: %s:)", asctime(timeinfo) )	
}

VirtualConsole::~VirtualConsole()
{
}

void VirtualConsole::ClearText()
{
	m_text.clear();
}

void VirtualConsole::Log(const char* text, bool newLine)
{
	if ( !text ) return;

	if ( newLine )
	{
		LOG("[GM] %s", text );
	}
	else 
	{
		LOG_SAMELINE("[GM] %s", text );
	}

	m_text += text;
	if ( newLine ) m_text += "\n";

	ScrollToBottom();	
}

void VirtualConsole::Gui()
{
	if ( !m_enable ) return;

	const v2i windowDimen = Window::Get()->Sizei();
	const int width = (int)(windowDimen.x*CONSOLE_WIDTH);
	const int height = (int)(windowDimen.y*CONSOLE_HEIGHT);
	
	Imgui::Begin("Console", v2i(windowDimen.x/2 - width/2, windowDimen.y-CONSOLE_PADDING), v2i(width, height) );
	Imgui::PrintParagraph(m_text.c_str());
	Imgui::Print(">");
	Imgui::SameLine();

	if ( Imgui::TextInput( NULL , m_command, width-CONSOLE_INPUT_PADDING ) == 1 )
	{
		char buffer[64];
		sprintf(buffer, "> %s", m_command.c_str() );
		Log(buffer);
		RunCommand( m_command.c_str() );

		m_command = "";
	}

	// set the input to the text input if is new window
	int textInputWidgetId = Imgui::GetPrevWidgetId();
	if ( Imgui::IsWorkingWindowNew() )
	{
		Imgui::SetActiveWidgetId(textInputWidgetId);
	}

	if ( m_queueScrollBottom )
	{
		Imgui::SetScrollRatioY(1.0f);
		m_queueScrollBottom = false;
	}

	if ( Imgui::IsWindowActive() ) 
	{
		int deltaIndex = 0;

		if ( !m_oldCmds.empty() )
		{
			if ( Input::Get()->DidKeyJustGoDown("UP") ) deltaIndex = -1;
			if ( Input::Get()->DidKeyJustGoDown("DOWN") ) deltaIndex = +1;
		}

		if ( deltaIndex != 0 )
		{
			m_cmdIndex = clamp( m_cmdIndex+deltaIndex, 0, (int)m_oldCmds.size()-1 );
			m_command = m_oldCmds[m_cmdIndex];
		}
	}

	Imgui::End();
}

void VirtualConsole::RunCommand( const std::string & cmd )
{
	if ( !cmd.empty() )
	{
		// remove command if reached limit
		if ( m_oldCmds.size() > MAX_OLD_CMDS ) m_oldCmds.pop_front();
		m_oldCmds.push_back(cmd);
	}
	m_cmdIndex = m_oldCmds.size();
	
	gmMachine * machine = &VirtualMachine::Get()->GetVM();
	assert(machine);

	// add semicolon at end just in case
	char buffer[128];
	sprintf(buffer, "%s;", cmd.c_str() );

	int threadId = GM_INVALID_THREAD;
	//int errors = machine->ExecuteString( buffer, &threadId, false );
	machine->ExecuteString( buffer, &threadId, false );

	if ( threadId != GM_INVALID_THREAD )
	{
		gmThread * thread = machine->GetThread(threadId);
		
		if ( thread )
		{
			m_lastCmdThread = threadId;

			// execute command
			gmVariable returnVal;
			gmThread::State state = thread->Sys_Execute(&returnVal);

			// failed
			if ( state == gmThread::EXCEPTION )
			{
				Log("Error: Command Failed");
				m_lastCmdThread = GM_INVALID_THREAD;
				machine->GetLog().Reset();
			}

			// successfully ran
			else if ( state == gmThread::KILLED )
			{
				if ( !returnVal.IsNull() )
				{
					Log( returnVal.AsStringWithType(machine, buffer, sizeof(buffer)) );
				}
			}
		}
	}
}

void VirtualConsole::Enable(bool enable)
{
	// if just activated, scroll to bottom
	if ( m_enable == false && enable == true )
	{
		ScrollToBottom();
	}

	m_enable = enable;
}

void VirtualConsole::ScrollToBottom()
{
	m_queueScrollBottom = true;
}
}