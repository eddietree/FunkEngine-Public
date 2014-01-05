#ifndef _INCLUDE_GM_DEBUGGER_FUNK_H
#define _INCLUDE_GM_DEBUGGER_FUNK_H

#include <queue>

#include <gm/gmDebug.h>
#include <gm/gmArrayLib.h>
#include <gfx/Texture2d.h>
#include <gfx/ShaderProgram.h>
#include <math/Cam2d.h>

class gmMachine;

class gmDebuggerFunk
{
public:
	void Open( gmMachine * machine );
	void Update();
	void Close();
	void Gui();

	bool IsDebugging() const;
	int GetDebugThreadId() const { return m_debugState.threadId; }

	struct DataPacket
	{
		char	data[32];
		int		len;
	};

	bool HasPacket() const { return !m_packets.empty(); }
	const DataPacket & GetPacket();
	void ReceiveMsg(const void * a_command, int &a_len);

	void BreakIntoRunningThread();

private:

	void GuiSource();
	void GuiThread();
	void GuiThreadsViewer();
	void DrawBG();

	void BeginSession();
	void EndSession();

	void HandleRenderException();

	// sending packets
	DataPacket& CreatePacket();
	void QueuePacket( const char * msg );
	void QueuePacket( const char * msg, int id );
	void QueuePacket( const char * msg, int id, int data );

	std::queue<DataPacket> m_packets;
	DataPacket m_currPacket;
	bool m_showFunctions;

	// background
	funk::StrongHandle<funk::Texture2d> m_texBG;
	funk::StrongHandle<funk::ShaderProgram> m_shader_bg;
	funk::Cam2d m_cam2d;

	struct DebugState
	{
		int threadId;
		int callStackLevel;
		int lineNumber;
		gmuint32 sourceId;
		gmUserArray tableTraverse;
		bool jumpToLineNumber;
		bool jumpToLineNumberIfNotInView;

		void ResetTableSelector();
	} m_debugState;

	gmDebugSession m_debugSesh;
};

#endif