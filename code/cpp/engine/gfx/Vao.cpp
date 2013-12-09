#include "Vao.h"
#include "GLIncludes.h"
#include <common/Debug.h>
#include <gm/gmBind.h>

namespace funk
{
	Vao::Vao() : m_id(0)
	{
	}

	Vao::~Vao()
	{
		if ( m_id ) glDeleteVertexArrays(1, &m_id);
	}

	void Vao::Init(void * a_data, int a_num_elems, uint32_t a_stride_bytes, StrongHandle<VertexLayout> a_layout, GLBufferType a_buffer_type )
	{
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);
		ASSERT(m_id);

		m_vbo = new funk::Vbo();
		m_vbo->Init( a_data, a_num_elems, a_stride_bytes, a_layout, a_buffer_type );
		
		// bind this vbo to this VAO, including layout
		m_vbo->Begin();
		glBindVertexArray(0);
		m_vbo->End();
	}
	
	void Vao::Init( StrongHandle<Vbo> a_vbo )
	{
		ASSERT( a_vbo != nullptr );
		
		m_vbo = a_vbo;
		
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);
		ASSERT(m_id);
		
		m_vbo->Begin();
		glBindVertexArray(0);
		m_vbo->End();
	}
	
	void Vao::Begin()
	{
		glBindVertexArray(m_id);
	}
	
	void Vao::End()
	{
		glBindVertexArray(0);
	}

	void Vao::Draw( GLPrimtive a_primitive, uint32_t a_offset, uint32_t a_num_elems )
	{
		m_vbo->Draw( a_primitive, a_offset, a_num_elems );
	}

	
GM_REG_NAMESPACE(Vao)
{
	GM_MEMFUNC_DECL(CreateVao)
	{
		// cannot create Vao in GameMonkey
		return GM_EXCEPTION;
	}

	GM_MEMFUNC_DECL(Draw)
	{
		GM_CHECK_NUM_PARAMS_RANGE(1,3);
		GM_CHECK_INT_PARAM( a_draw_primitive, 0 );
		GM_INT_PARAM( a_offset, 1, 0 );
		GM_INT_PARAM( a_num_elems, 2, Vao::RENDER_ALL_VERTS );

		GM_GET_THIS_PTR(Vao, ptr);
		ptr->Draw( (GLPrimtive)a_draw_primitive, a_offset, a_num_elems );
		
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID( Vao, Begin )
	GM_GEN_MEMFUNC_VOID_VOID( Vao, End )
	GM_GEN_MEMFUNC_INT_VOID( Vao, Id )
}

GM_REG_MEM_BEGIN(Vao)
GM_REG_MEMFUNC( Vao, Begin )
GM_REG_MEMFUNC( Vao, End )
GM_REG_MEMFUNC( Vao, Id )
GM_REG_MEMFUNC( Vao, Draw )
GM_REG_HANDLED_DESTRUCTORS(Vao)
GM_REG_MEM_END()
GM_BIND_DEFINE(Vao)

}