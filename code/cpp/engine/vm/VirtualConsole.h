#pragma once

#include <deque>
#include <string>

namespace funk
{
	class VirtualConsole
	{
	public:
		VirtualConsole();
		~VirtualConsole();

		void Log(const char* text, bool newLine = true);
		void Gui();
		
		bool IsEnabled() const { return m_enable; }
		void Enable(bool enable);

		int GetLastCmdThread() const { return m_lastCmdThread; }
		void ClearText();

	private:

		void ScrollToBottom();
		void RunCommand( const std::string & cmd );

		bool m_enable;
		bool m_queueScrollBottom;
		int m_lastCmdThread;
		
		std::string m_text;
		std::string m_command;

		std::deque<std::string> m_oldCmds;
		int m_cmdIndex;
	};
}
