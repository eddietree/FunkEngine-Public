#include "VertexLayout.h"
#include "GLIncludes.h"
#include "RenderState.h"

#include <vector>
#include <common/Debug.h>

namespace funk
{
	VertexLayout::VertexLayout()
	: m_num_entries(0), m_vertex_num_bytes(0), m_is_constructing(false)
	{
	}
	
	void VertexLayout::Begin()
	{
		for( int i = 0; i < m_num_entries; ++i )
		{
			const Entry & entry = m_entries[i];
			unsigned int offsetBytes = entry.offset_bytes;
			
			RenderState::Ref().EnableVertexAttribArray(i);
			glVertexAttribPointer(i, entry.num_elems, entry.data_type, entry.normalize ? GL_TRUE:GL_FALSE, m_vertex_num_bytes, (GLvoid*)(offsetBytes) );
		}

		for( int i = m_num_entries; i < MAX_ENTRIES; ++i )
		{
			RenderState::Ref().DisableVertexAttribArray(i);
		}
	}
	
	void VertexLayout::End()
	{
	}
	
	void VertexLayout::BeginEntries(int vertexSizeBytes)
	{
		m_vertex_num_bytes = vertexSizeBytes;
		m_is_constructing = true;
	}
	
	void VertexLayout::AddEntry( GLDataType a_data_type, int a_num_elems, int a_offset_bytes, bool a_normalize )
	{
		ASSERT( m_is_constructing );
		ASSERT( m_num_entries < MAX_ENTRIES );
		
		// add entry
		Entry & entry		= m_entries[m_num_entries];
		entry.data_type		= a_data_type;
		entry.num_elems		= a_num_elems;
		entry.offset_bytes	= a_offset_bytes;
		entry.normalize		= a_normalize;
		
		++m_num_entries;
	}
	
	void VertexLayout::EndEntries()
	{
		ASSERT( m_is_constructing );
		m_is_constructing = false;

		// must have one entry..
		ASSERT( m_num_entries > 0 );
	}
	
}