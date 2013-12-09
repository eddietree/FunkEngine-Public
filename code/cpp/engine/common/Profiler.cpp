#include "Profiler.h"

#include <common/Util.h>
#include <common/Debug.h>
#include <imgui/Imgui.h>

namespace funk
{
	const char * PROFILER_ROOT_NAME = "[Root]";

	Profiler::Profiler() 
	{
		InitEntry( m_root, PROFILER_ROOT_NAME );

		m_active_entry = nullptr;
		m_paused = false;
		m_pause_state_goal = m_paused;
	}
	
	Profiler::~Profiler()
	{
		Clear();
	}

	void Profiler::EntryBegin( const char * a_entry_name )
	{
		if ( m_paused ) return;
		if ( m_active_entry == nullptr && a_entry_name != PROFILER_ROOT_NAME ) return;

		Entry * entry;

		if ( a_entry_name == PROFILER_ROOT_NAME ) 
			entry = &m_root;
		else
			entry = GetChildEntry( *m_active_entry, a_entry_name );

		// set active as true
		entry->time_start = GetMachinePerformanceCounter();
		entry->active = true;

		m_active_entry = entry;
	}

	void Profiler::EntryEnd( const char * a_entry_name )
	{
		if ( m_paused ) return;
		if ( m_active_entry == nullptr && a_entry_name != PROFILER_ROOT_NAME ) return;

		ASSERT( m_active_entry != nullptr );
		ASSERT( GetEntryKey( a_entry_name ) == m_active_entry->entry_key );

		// set end counter
		m_active_entry->time_end = GetMachinePerformanceCounter();
		m_active_entry->frames[ m_active_entry->running_times_index ].ms_time_inclusive_minus = 0.0f;

		// go towards parent
		m_active_entry = m_active_entry->parent;
	}

	void Profiler::Begin()
	{
		if ( m_paused ) return;

		// by the time we get here, profiler should be back at null
		ASSERT( m_active_entry == nullptr );

		// Go through tree and set active as false
		SetEntriesActiveState( m_root, false );

		EntryBegin( PROFILER_ROOT_NAME );
	}

	void Profiler::End()
	{
		EntryEnd( PROFILER_ROOT_NAME );

		// by the time we get here, profiler should be back at root
		ASSERT( m_active_entry == nullptr );

		// caculate times and offsets from root
		if ( !m_paused )
		{
			CalcEntryTime( &m_root );

			// Go through tree and remove inactive 
			ClearInactiveEntries( m_root );
		}

		// set the pause state
		if ( m_pause_state_goal != m_paused )
		{
			m_paused = m_pause_state_goal;
		}
	}

	void Profiler::ClearInactiveEntries( Entry & a_entry )
	{
		for( auto it = a_entry.children.begin(); it != a_entry.children.end(); ++it )
		{
			ClearInactiveEntries( it->second );

			if ( !it->second.active )
			{
				RemoveLineGraph( it->second.entry_key );
				a_entry.children.erase(it++);

				if ( it == a_entry.children.end() ) return;

				continue;
			}
		}
	}

	void Profiler::SetEntriesActiveState( Entry & a_entry, bool a_active )
	{
		for ( auto &it : a_entry.children )
		{
			it.second.active = a_active;
			SetEntriesActiveState( it.second, a_active );
		}
	}

	Profiler::EntryKey Profiler::GetEntryKey( const char * a_entry_name ) const
	{
		return HashString(a_entry_name);
	}

	void Profiler::Gui()
	{
		if ( Imgui::Button( m_paused ? "Play" : "Pause") )
		{
			SetPause( !m_pause_state_goal );
		}

		Imgui::Header("Help");
		Imgui::Print("[-/+] Expand Tree");
		Imgui::Print("[ x ] Plot Line Graph");
		Imgui::Print("[Inc] Inclusive: Includes all child nodes");
		Imgui::Print("[Exc] Exclusive: Does not include any children");
		
		Imgui::Header("        Entry                                                   Avg(I)  Avg(E)   Inc     Exc");
		GuiEntry( &m_root, 0 );
	}

	void Profiler::GuiLineGraphs()
	{
		for( auto & it : m_map_graphs )
		{
			LineGraphEntry &line_graph_entry = it.second;

			Imgui::Print( line_graph_entry.entry->name );
			Imgui::LineGraph( line_graph_entry.line_graph );
			Imgui::Separator();
		}
	}

	void Profiler::GuiEntry( Entry * a_entry, int a_level )
	{
		ASSERT( a_entry != nullptr );

		const int k_entry_name_len = 54;

		char buffer[128] = {0};

		// add initial tabs
		for( int i = 0; i < a_level; ++i )
		{
			strcat( buffer, "   " );
		}

		strcat( buffer, a_entry->name );

		int str_len = strlen( buffer );
		int num_dots = k_entry_name_len - str_len;
		memset( buffer + str_len, '.', num_dots );
		buffer[ strlen(buffer) ] ='\0';

		const auto & entry_frame = a_entry->GetCurrFrame();

		float ms_inclusive, ms_exclusive;
		a_entry->GetTimeAvg( ms_inclusive, ms_exclusive );

		sprintf( buffer + k_entry_name_len, "%7.3f %7.3f %7.3f %7.3f", ms_inclusive, ms_exclusive, entry_frame.ms_time_inclusive, entry_frame.ms_time_inclusive - entry_frame.ms_time_inclusive_minus );
		buffer[ strlen(buffer) ] ='\0';

		bool plot_graph = a_entry->plot_graph;
		Imgui::DropDown( "", a_entry->expanded );
		Imgui::SameLine();
		Imgui::CheckBox( "", plot_graph );
		Imgui::SameLine();
		Imgui::Print( buffer );

		// remove plot graph
		if ( plot_graph == false && a_entry->plot_graph != plot_graph )
		{
			RemoveLineGraph( a_entry->entry_key );
		}

		a_entry->plot_graph = plot_graph;
		
		if ( !a_entry->children.empty() && a_entry->expanded )
		{
			for( auto & entry_pair : a_entry->children )
			{
				Entry * entry = &(entry_pair.second);
				GuiEntry( entry, a_level+1 );
			}
		}
	}

	void Profiler::CalcEntryTime( Entry * a_entry )
	{
		// handle children
		for( auto & it : a_entry->children )
		{
			Entry & curr = it.second;
			CalcEntryTime( &curr );
		}

		// ms time
		const float ms_time = GetElapsedTimeMillsec( a_entry->time_start, a_entry->time_end );
		a_entry->frames[ a_entry->running_times_index ].ms_time_inclusive = ms_time;
		
		// if graphing. add to graph
		if ( a_entry->plot_graph )
		{
			LineGraphEntry * line_graph_entry = GetLineGraph( a_entry->entry_key );
			line_graph_entry->entry = a_entry;
			line_graph_entry->line_graph.PushVal( ms_time );
		}
		
		// parent exclusive
		Entry * parent = a_entry->parent;
		if ( parent != nullptr )
		{
			parent->frames[ parent->running_times_index ].ms_time_inclusive_minus += ms_time;
		}

		// next index
		const int num_indices = sizeof(a_entry->frames) / sizeof(a_entry->frames[0] );
		a_entry->running_times_index = ( a_entry->running_times_index + 1 ) % num_indices;
	}

	void Profiler::Clear()
	{
		m_map_graphs.clear();
		m_root.children.clear();
		m_active_entry = nullptr;
	}

	void Profiler::SetPause( bool a_pause )
	{
		m_pause_state_goal = a_pause;
	}

	void Profiler::InitEntry( Entry & a_entry, const char * a_entry_name )
	{
		a_entry.running_times_index = 0;
		a_entry.expanded = false;
		a_entry.entry_key = GetEntryKey( a_entry_name );
		a_entry.parent = nullptr;
		a_entry.expanded = true;
		a_entry.plot_graph = false;

		strncpy( a_entry.name, a_entry_name, sizeof(a_entry.name) );
		memset( a_entry.frames, 0, sizeof(a_entry.frames) );
	}

	Profiler::Entry * Profiler::GetChildEntry( Entry & a_parent, const char * a_entry_name )
	{
		ASSERT( a_entry_name != nullptr );

		EntryKey entry_key = GetEntryKey( a_entry_name );
		auto it = a_parent.children.find( entry_key );

		// found
		if ( it != a_parent.children.end() )
		{
			return &(it->second);
		}

		// create new entry
		auto it_insert = a_parent.children.insert( std::make_pair(entry_key, Entry() ) );
		Entry * new_entry = &(it_insert.first->second);
		
		// set new parameters
		InitEntry( *new_entry, a_entry_name );
		new_entry->parent = &a_parent;

		return new_entry;
	}

	Profiler::EntryFrameData & Profiler::Entry::GetCurrFrame()
	{
		const int num_indices = sizeof(frames) / sizeof( frames[0] );
		int curr_index = running_times_index - 1;

		if ( curr_index < 0 ) 
			curr_index += num_indices;

		return frames[curr_index];
	}

	void Profiler::Entry::GetTimeAvg( float & out_inclusive, float & out_exclusive )
	{
		const int num_indices = sizeof(frames) / sizeof( frames[0] );

		// accumulate times
		int count = 0;
		out_inclusive = 0.0f;
		out_exclusive = 0.0f;

		for( int i = 0; i < num_indices; ++i )
		{
			const float ms_time = frames[i].ms_time_inclusive;

			if ( ms_time != 0 ) 
			{
				out_inclusive += ms_time;
				out_exclusive += frames[i].ms_time_inclusive - frames[i].ms_time_inclusive_minus;
				++count;
			}
		}

		if ( count > 0 )
		{
			out_inclusive /= count;
			out_exclusive /= count;
		}
	}

	bool Profiler::RemoveLineGraph( EntryKey a_entry_key )
	{
		auto it = m_map_graphs.find( a_entry_key );

		if ( it != m_map_graphs.end() )
		{
			m_map_graphs.erase(it);
			return true;
		}

		return false;
	}

	Profiler::LineGraphEntry * Profiler::GetLineGraph( EntryKey a_entry_key )
	{
		// found old version
		auto it = m_map_graphs.find( a_entry_key );
		if ( it != m_map_graphs.end() )
		{
			return &(it->second);
		}

		auto it_insert = m_map_graphs.insert( std::make_pair(a_entry_key, LineGraphEntry() ) );

		// initialize
		LineGraphEntry * new_line_graph_entry = &(it_insert.first->second);
		new_line_graph_entry->line_graph.SetDimen( v2i( 200, 100 ) );
		new_line_graph_entry->line_graph.SetMinVal(0.0f);
		new_line_graph_entry->line_graph.SetMaxVal(32.0f);

		return new_line_graph_entry;
	}
}

#ifdef FUNK_FINAL
void gmThreadProfileBegin( gmThread* a_thread ) { }
void gmThreadProfileEnd( gmThread* a_thread ) { }
#else


struct ThreadProfileInfo
{
	gmThread* thread;
	const char* func;
};

std::vector<ThreadProfileInfo> thread_profiles;

void gmThreadProfileBegin( gmThread* a_thread )
{
    gmFunctionObject* func = a_thread->GetFunction()->GetFunctionObjectSafe();
    const char* funcname = func->GetDebugName();
	ThreadProfileInfo info = { a_thread, funcname };

	thread_profiles.push_back( info );

	PROFILE_BEGIN( info.func );
}

void gmThreadProfileEnd( gmThread* a_thread )
{
	auto a = thread_profiles.begin();
	auto b = thread_profiles.end();

	for ( ; a != b; ++a )
	{
		if ( a->thread == a_thread )
		{
			break;
		}
	}

	ASSERT( a != b );

	PROFILE_END( a->func );

	thread_profiles.erase( a );
}

#endif
