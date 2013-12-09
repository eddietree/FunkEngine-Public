#pragma once

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include "VertexLayout.h"
#include "GLTypes.h"

#include <stdint.h>

namespace funk
{
	class Vbo : public HandledObj<Vbo>
	{
	private:
		uint32_t		m_id;
		uint32_t		m_stride_bytes;
		uint32_t		m_num_elems;
		uint32_t		m_num_bytes_total;
		GLBufferType	m_data_type; // BUFFER_STATIC, BUFFER_DYNAMIC, BUFFER_STREAM
		
		bool m_bound;
		
		StrongHandle< VertexLayout > m_vert_layout;
		
	public:
		
		Vbo();
		~Vbo();
		
		void Init(void * a_data, int a_num_elems, uint32_t a_stride_bytes, StrongHandle<VertexLayout> a_layout, GLBufferType a_buffer_type = BUFFER_STATIC  );
		
		// that's right, all the verts
		static const uint32_t RENDER_ALL_VERTS = 0;
		
		// Use this to draw
		// begin/end binds and unbinds the vertex layout
		void Begin();
		void End();
		void Draw( GLPrimtive primitive = PRIM_TRIANGLES, uint32_t offset = 0, uint32_t numElems = RENDER_ALL_VERTS );
		
		uint32_t Id() const { return m_id; }
		uint32_t GetNumElems() const { return m_num_elems; }
		bool IsBound() const { return m_bound; }
		
		// bind just binds the vbo
		// use this for subData or any other low-level stuff
		void Bind();
		void Unbind();
		
		void SubData( void* a_data, uint32_t a_num_bytes, uint32_t a_byte_offset = 0 );
		
		GM_BIND_TYPEID(Vbo);
	};
	
	GM_BIND_DECL(Vbo);
}
