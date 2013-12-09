#pragma once

#include "SvgDrawer.h"

namespace funk
{
	class SvgReader
	{

	private:

		std::vector<SvgVert>	m_verts;
		std::vector<uint32_t>	m_indices;
		std::vector<SvgDrawCmd> m_draw_cmds;

	public:
		
		void LoadFile( const char * a_filename );
		StrongHandle<SvgDrawer> GenerateSvgDrawer();

		void GenerateFlattenedVerticesArray( std::vector<SvgVert>& a_output_vertices );

		// Vertices
		inline const SvgVert GetVertex( int a_index ) { return m_verts[a_index]; }
		inline size_t GetNumVerts() const { return m_verts.size(); }

		// Indices
		inline const uint32_t GetIndex( int a_index ) { return m_indices[a_index]; }
		inline size_t GetNumIndices() const { return m_indices.size(); }		

	private:

	};
}
