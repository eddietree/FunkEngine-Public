#include "Ibo.h"

#include <common/Debug.h>
#include <gm/gmBind.h>

#include "GLDebug.h"
#include "GLIncludes.h"

namespace funk
{

Ibo::Ibo() : m_num_elems( 0 )
{
}

void Ibo::Init( void* a_data, uint32_t a_num_elems, uint32_t a_bytes_per_elem, GLDataType a_data_type, GLBufferType a_buffer_type )
{
	m_num_elems = a_num_elems;
	m_data_type = a_data_type;
	m_buffer_type = a_buffer_type;

	CHECK( a_data != 0, "No IBO data set" );

	// generate id
	glGenBuffers( 1, &m_id );
	ASSERT(m_id != 0);

	// no buffers can be bound
    CheckGLBufferBindingID( GL_ELEMENT_ARRAY_BUFFER_BINDING, 0 );

	// Bind the buffer and send data
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_id );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, a_num_elems * a_bytes_per_elem, a_data, m_buffer_type );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

Ibo::~Ibo()
{
	if ( m_id )
		glDeleteBuffers( 1, &m_id );
}

void Ibo::Begin()
{
    CheckGLBufferBindingID( GL_ELEMENT_ARRAY_BUFFER_BINDING, 0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_id );
}

void Ibo::Draw( GLPrimtive a_render_primitive, uint32_t a_num_elems )
{
	a_num_elems = a_num_elems == RENDER_ALL_VERTS ? m_num_elems : a_num_elems;

	ASSERT( a_num_elems <= m_num_elems );
	ASSERT( a_num_elems >= 0 );

    CheckGLBufferBindingID( GL_ELEMENT_ARRAY_BUFFER_BINDING, m_id );

	//glDrawRangeElements( renderPrimitive, 0, numElems, numElems, GL_UNSIGNED_INT, (GLvoid*)((offset) * sizeof(uint32_t)) );
	glDrawElements(a_render_primitive, a_num_elems, m_data_type, 0);
}

void Ibo::End()
{
    CheckGLBufferBindingID( GL_ELEMENT_ARRAY_BUFFER_BINDING, m_id );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

GM_REG_NAMESPACE(Ibo)
{
	GM_MEMFUNC_DECL(CreateIbo)
	{
		// cannot create Ibo in GameMonkey
		return GM_EXCEPTION;
	}
	
	GM_MEMFUNC_DECL(Draw)
	{
		GM_CHECK_NUM_PARAMS_RANGE(1,2);
		GM_CHECK_INT_PARAM( a_draw_primitive, 0 );
		GM_INT_PARAM( a_num_elems, 1, Ibo::RENDER_ALL_VERTS );
		
		GM_GET_THIS_PTR(Ibo, ptr);
		ptr->Draw( (GLPrimtive)a_draw_primitive, a_num_elems );
		
		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_VOID_VOID( Ibo, Begin )
	GM_GEN_MEMFUNC_VOID_VOID( Ibo, End )
	GM_GEN_MEMFUNC_INT_VOID( Ibo, Id )
}

GM_REG_MEM_BEGIN(Ibo)
GM_REG_MEMFUNC( Ibo, Begin )
GM_REG_MEMFUNC( Ibo, End )
GM_REG_MEMFUNC( Ibo, Id )
GM_REG_MEMFUNC( Ibo, Draw )
GM_REG_HANDLED_DESTRUCTORS(Ibo)
GM_REG_MEM_END()
GM_BIND_DEFINE(Ibo)


}