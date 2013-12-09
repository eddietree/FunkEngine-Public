
#pragma once

#include <common/StrongHandle.h>
#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>

#include "Vbo.h"
#include "VertexLayout.h"
#include "GLTypes.h"

namespace funk
{
	class Vao : public HandledObj<Vao>
	{
	private:
		uint32_t m_id;
		StrongHandle<Vbo> m_vbo;
		
	public:
		
		Vao();
		~Vao();
		
		// generates a VBO and populates it
		void Init(void * a_data, int a_num_elems, uint32_t a_stride_bytes, StrongHandle<VertexLayout> a_layout, GLBufferType a_buffer_type = BUFFER_STATIC );
		
		// init with a specific VBO
		void Init( StrongHandle<Vbo> a_vbo );
		
		// that's right, all the verts
		static const uint32_t RENDER_ALL_VERTS = 0;
		
		void Begin();
		void End();
		void Draw( GLPrimtive primitive = PRIM_TRIANGLES, uint32_t offset = 0, uint32_t numElems = RENDER_ALL_VERTS );
		
		StrongHandle<Vbo> & GetVbo() { return m_vbo; }
		uint32_t Id() const { return m_id; }

		GM_BIND_TYPEID(Vao);
	};
	
	GM_BIND_DECL(Vao);
}
