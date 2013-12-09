#pragma once

#include <vector>
#include <common/HandledObj.h>
#include "GLTypes.h"

namespace funk
{
    class VertexLayout : public HandledObj<VertexLayout>
    {
	private:
		
		struct Entry
        {
			GLDataType		data_type;
            int				num_elems;
			int				offset_bytes;
			bool			normalize;
        };
		
		// entries
		static const int MAX_ENTRIES = 8;
		Entry	m_entries[MAX_ENTRIES];
		int		m_num_entries;
		
		// vertex data
		int	m_vertex_num_bytes;

		// currently building the layout?
		bool m_is_constructing;
		
    public:
		
		VertexLayout();
        
        void Begin();
        void End();
        
		// creation time only
		void BeginEntries(int vertexSizeBytes);
		void AddEntry( GLDataType a_data_type, int a_num_elems, int a_offset_bytes, bool a_normalize = false );
		void EndEntries();
        
    private:
        
    };
    
}
