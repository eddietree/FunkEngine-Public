#pragma once

#include <common/Singleton.h>
#include <system/Timer.h>
#include <gfx/LineGraph.h>

#include <unordered_map>

#if !defined(FUNK_FINAL)
	#define PROFILE_BEGIN( A_ENTRY_NAME ) if (funk::Profiler::InstExists() ) { funk::Profiler::Ref().EntryBegin(A_ENTRY_NAME); }
	#define PROFILE_END( A_ENTRY_NAME ) if (funk::Profiler::InstExists() ) { funk::Profiler::Ref().EntryEnd(A_ENTRY_NAME); }
#else // FUNK_FINAL
	#define PROFILE_BEGIN( A_ENTRY_NAME )
	#define PROFILE_END( A_ENTRY_NAME )
#endif // FUNK_FINAL


namespace funk
{
    class Profiler : public Singleton<Profiler>
	{
	private:

		typedef uint32_t EntryKey;

		struct EntryFrameData
		{
			float ms_time_inclusive;
			float ms_time_inclusive_minus;
		};

		struct Entry
		{
			char name[64];
			Entry * parent;
			EntryKey entry_key;
			PerformanceCounter time_start;
			PerformanceCounter time_end;

			std::unordered_map<EntryKey, Entry> children;

			// average persistent time across frames
			EntryFrameData frames[32]; 
			int running_times_index;
			
			bool active;		// if inactive, will remove during next cleanup process
			bool expanded;		// tab expanded
			bool plot_graph;	// plot graph

			EntryFrameData & GetCurrFrame();
			void GetTimeAvg( float & out_inclusive, float & out_exclusive );
		};

		Entry m_root;
		Entry * m_active_entry; // current active entry

		bool m_paused;
		bool m_pause_state_goal;
	
	private:

		struct LineGraphEntry
		{
			Entry * entry;
			LineGraph line_graph;
		};

		std::unordered_map< EntryKey, LineGraphEntry > m_map_graphs;

		bool RemoveLineGraph( EntryKey a_entry_key );
		LineGraphEntry * GetLineGraph( EntryKey a_entry_key );

    public:
		
		void Begin();
		void End();
		void EntryBegin( const char * a_entry_name );
		void EntryEnd( const char * a_entry_name );

		void Clear();
		void SetPause( bool a_pause );

		void Gui();

		// line graphs
		bool HasLineGraphs() const { return !m_map_graphs.empty(); }
		void GuiLineGraphs();

	private:

		// recursive
		void ClearInactiveEntries( Entry & a_entry );
		void SetEntriesActiveState( Entry & a_entry, bool a_active );
		void GuiEntry( Entry * a_entry, int a_level );
		void CalcEntryTime( Entry * a_entry );

		// fetches the child entry with key, returns new instance if does not exist
		Entry * GetChildEntry( Entry & a_parent, const char * a_entry_name );
		EntryKey GetEntryKey( const char * a_entry_name ) const;

		void InitEntry( Entry & a_entry, const char * a_entry_name );

        friend Singleton<Profiler>;
		Profiler();
		~Profiler();
    };
}