#include "Vbo.h"
#include "GLIncludes.h"
#include "GLDebug.h"
#include <common/Debug.h>
#include <gm/gmBind.h>

namespace funk
{
	Vbo::Vbo() : m_id(0), m_stride_bytes(0), m_num_elems(0), m_num_bytes_total(0), m_bound(false)
	{
	}

	Vbo::~Vbo()
	{
		if ( m_id )
            glDeleteBuffers(1, &m_id);
	}

	void Vbo::Init(void * a_data, int a_num_elems, uint32_t a_stride_bytes, StrongHandle<VertexLayout> a_layout, GLBufferType a_buffer_type )
	{
		// ASSERT(a_data); // can init blank VBO
		ASSERT(a_layout != nullptr );
		ASSERT(a_stride_bytes > 0);
		
		m_vert_layout = a_layout;
		
		// load data onto CPU
		glGenBuffers( 1, &m_id );
		ASSERT(m_id);
		
		m_stride_bytes		= a_stride_bytes;
		m_num_elems			= a_num_elems;
		m_num_bytes_total	= m_stride_bytes * m_num_elems;
		m_data_type			= a_buffer_type;
		
        CheckGLBufferBindingID( GL_ARRAY_BUFFER_BINDING, 0 );

		// Bind the buffer and send data
		if ( m_num_bytes_total > 0 )
		{
			glBindBuffer( GL_ARRAY_BUFFER, m_id );
			glBufferData( GL_ARRAY_BUFFER, m_num_bytes_total, a_data, m_data_type );
			glBindBuffer( GL_ARRAY_BUFFER, 0 );		
		}
	}

	void Vbo::Bind()
	{
		ASSERT( m_id != 0 );
		ASSERT(!m_bound);
        CheckGLBufferBindingID( GL_ARRAY_BUFFER_BINDING, 0 );

		m_bound = true;
		
		glBindBuffer( GL_ARRAY_BUFFER, m_id );
	}

	void Vbo::Unbind()
	{
		ASSERT(m_bound);
        CheckGLBufferBindingID( GL_ARRAY_BUFFER_BINDING, m_id );

		m_bound = false;
		
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}
	
	void Vbo::Begin()
	{
		ASSERT(m_vert_layout != nullptr);
		ASSERT(m_id != 0);
		
		Bind();
		m_vert_layout->Begin();
	}
	
	void Vbo::End()
	{
		m_vert_layout->End();
		Unbind();
	}

	void Vbo::Draw( GLPrimtive a_primitive, uint32_t a_offset, uint32_t a_num_elems )
	{
		//ASSERT( m_bound ); // can be bound from VAO

        // TODO: is this check valid in the case of VAO?
        CheckGLBufferBindingID( GL_ARRAY_BUFFER_BINDING, m_id );
		
		ASSERT( a_offset + a_num_elems <= m_num_elems );
		ASSERT( a_offset >= 0 );
		ASSERT( a_num_elems >= 0 );

		const uint32_t num_elems = a_num_elems == RENDER_ALL_VERTS ? m_num_elems : a_num_elems;
		glDrawArrays( a_primitive, a_offset, num_elems );
	}

	void Vbo::SubData( void* a_data, uint32_t a_num_bytes, uint32_t a_bytes_offset )
	{
		ASSERT( m_id );
		ASSERT( m_data_type == BUFFER_STREAM || m_data_type == BUFFER_DYNAMIC );
		ASSERT( a_data );
		ASSERT( a_num_bytes );
		CHECK( a_num_bytes <= m_num_bytes_total, "Vbo subdata exceeded byte size!" );
		CHECK( m_bound, "Cannot sub datas, need to bind VBO!" );

        CheckGLBufferBindingID( GL_ARRAY_BUFFER_BINDING, m_id );

#if GL_ES
		// mapbufferRange runs faster on ios. using glBufferSubData, i got total shit framerates as it requires flushes
		void * ptr = glMapBufferRange( GL_ARRAY_BUFFER, a_bytes_offset,  a_num_bytes, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_RANGE_BIT );
		ASSERT(ptr);
		memcpy( ptr, a_data, a_num_bytes );
		
		glUnmapBuffer(GL_ARRAY_BUFFER);
		//glFlushMappedBufferRangeEXT( GL_ARRAY_BUFFER, a_bytes_offset, a_num_bytes );
		
#else
		glBufferSubData( GL_ARRAY_BUFFER, a_bytes_offset, a_num_bytes, a_data );
#endif
	}
	
GM_REG_NAMESPACE(Vbo)
{
	GM_MEMFUNC_DECL(CreateVbo)
	{
		// cannot create Vbo in GameMonkey
		return GM_EXCEPTION;
	}
	
	GM_MEMFUNC_DECL(Draw)
	{
		GM_CHECK_NUM_PARAMS_RANGE(1,3);
		GM_CHECK_INT_PARAM( a_draw_primitive, 0 );
		GM_INT_PARAM( a_offset, 1, 0 );
		GM_INT_PARAM( a_num_elems, 2, Vbo::RENDER_ALL_VERTS );
		
		GM_GET_THIS_PTR(Vbo, ptr);
		ptr->Draw( (GLPrimtive)a_draw_primitive, a_offset, a_num_elems );
		
		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_VOID_VOID( Vbo, Begin )
	GM_GEN_MEMFUNC_VOID_VOID( Vbo, End )
	GM_GEN_MEMFUNC_INT_VOID( Vbo, Id )
}

GM_REG_MEM_BEGIN(Vbo)
GM_REG_MEMFUNC( Vbo, Begin )
GM_REG_MEMFUNC( Vbo, End )
GM_REG_MEMFUNC( Vbo, Id )
GM_REG_MEMFUNC( Vbo, Draw )
GM_REG_HANDLED_DESTRUCTORS(Vbo)
GM_REG_MEM_END()
GM_BIND_DEFINE(Vbo)

}