#pragma once

#include "Vbo.h"
#include "VertexBatcher.h"

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>

namespace funk
{	
	class VboBuilder : public VertexBatcher, public HandledObj<VboBuilder>
	{
	private:

		StrongHandle<VertexLayout> m_vertex_layout;

	public:

		VboBuilder( int a_max_num_verts );
		~VboBuilder();
		
		void Clear();
		StrongHandle<Vbo> CreateVbo();

		GM_BIND_TYPEID(VboBuilder);

	private:
		
	};

	GM_BIND_DECL( VboBuilder );
}
