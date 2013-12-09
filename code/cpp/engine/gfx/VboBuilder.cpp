#include "VboBuilder.h"

#include "GLDebug.h"

#include <common/Debug.h>
#include <gm/gmBind.h>

namespace funk
{
	VboBuilder::VboBuilder (int a_max_num_verts ) : VertexBatcher(a_max_num_verts)
	{
		LOG("VboBuilder initialized");
		m_vertex_layout = CreateVertexLayout();
	}

	VboBuilder::~VboBuilder()
	{
	}

	void VboBuilder::Clear()
	{
		GetVertexBatch().clear();
	}

	StrongHandle<Vbo> VboBuilder::CreateVbo()
	{
		StrongHandle<Vbo> vbo = new Vbo;

		auto & batch = GetVertexBatch();

		if ( !batch.empty() )
		{
			vbo->Init( &batch.front(), batch.size(), GetNumBytesPerVertex(), m_vertex_layout );
		}

		CheckGLError();

		return vbo;
	}

GM_REG_NAMESPACE(VboBuilder)
{
	GM_MEMFUNC_CONSTRUCTOR(VboBuilder)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_max_num_verts, 0 );

		StrongHandle<VboBuilder> p = new VboBuilder(a_max_num_verts);

		GM_PUSH_USER_HANDLED( VboBuilder, p.Get() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(CreateVbo)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(VboBuilder, ptr);

		StrongHandle<Vbo> vbo = ptr->CreateVbo();
		GM_PUSH_USER_HANDLED( Vbo, vbo.Get() );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(Vertex2f)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_GET_THIS_PTR(VboBuilder, ptr);
		
		ptr->Vertex(a_pos);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex2f_TexCoord2f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );

		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Vertex(a_pos, a_uv);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Vertex2f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_V3_PARAM( a_normal, 1 );

		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Vertex(a_pos, v2(0.0f), a_normal);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex2f_TexCoord2f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );
		GM_CHECK_V3_PARAM( a_normal, 2 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Vertex(a_pos, a_uv, a_normal);
		return GM_OK;
	}	
	
	GM_MEMFUNC_DECL(Vertex3f)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V3_PARAM( a_pos, 0 );

		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Vertex(a_pos);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex3f_TexCoord2f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V3_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Vertex(a_pos, a_uv);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex3f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V3_PARAM( a_pos, 0 );
		GM_CHECK_V3_PARAM( a_normal, 1 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Vertex(a_pos, v2(0.0f), a_normal);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex3f_TexCoord2f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V3_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );
		GM_CHECK_V3_PARAM( a_normal, 2 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Vertex(a_pos, a_uv, a_normal);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawRect)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		GM_GET_THIS_PTR(VboBuilder, ptr);
		
		ptr->DrawRect( bottomLeft, dimen );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawRectTexCoords)
	{
		int numParams = a_thread->GetNumParams();
		GM_GET_THIS_PTR(VboBuilder, ptr);
		
		if ( numParams == 2 )
		{
			GM_CHECK_V2_PARAM( bottomLeft, 0 );
			GM_CHECK_V2_PARAM( dimen, 1 );
			ptr->DrawRectTexCoords( bottomLeft, dimen );
		}
		else if ( numParams == 4 )
		{
			GM_CHECK_V2_PARAM( bottomLeft, 0 );
			GM_CHECK_V2_PARAM( dimen, 1 );
			GM_CHECK_V2_PARAM( uv0, 2 );
			GM_CHECK_V2_PARAM( uv1, 3 );
			ptr->DrawRectTexCoords( bottomLeft, dimen, uv0, uv1 );
			
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawScreenUV)
	{
		GM_CHECK_NUM_PARAMS_EITHER(0,2);
		GM_GET_THIS_PTR(VboBuilder, ptr);

		if ( a_thread->GetNumParams() == 0 )
		{
			ptr->DrawScreenUV();
		}
		else
		{
			GM_CHECK_V2_PARAM( a_bottom_left, 0);
			GM_CHECK_V2_PARAM( a_dimen, 1);
			ptr->DrawScreenUV( a_bottom_left, a_dimen );
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Color)
	{
		GM_CHECK_V3_PARAM( a_color, 0);
		GM_FLOAT_PARAM( a_alpha, 1, 1.0f );

		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->Color( a_color, a_alpha );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawCube)
	{
		int numParams = a_thread->GetNumParams();
		GM_ASSERT(numParams >= 1 && numParams <= 2 );
		GM_CHECK_V3_PARAM( dimen, 0 );
		GM_GET_THIS_PTR(VboBuilder, ptr);

		if ( numParams == 2 )
		{
			GM_CHECK_V3_PARAM( pos, 1 );
			ptr->DrawCube( dimen, pos );
		}
		else
		{
			ptr->DrawCube( dimen );
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawCubeWire)
	{
		int numParams = a_thread->GetNumParams();
		GM_ASSERT(numParams >= 1 && numParams <= 2 );
		GM_CHECK_V3_PARAM( dimen, 0 );

		GM_GET_THIS_PTR(VboBuilder, ptr);

		if ( numParams == 2 )
		{
			GM_CHECK_V3_PARAM( pos, 1 );
			ptr->DrawCubeWire( dimen, pos );
		}
		else
		{
			ptr->DrawCubeWire( dimen );
		}
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawCircle)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_INT_PARAM( numSegs, 2, 32 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->DrawCircle( center, radius, numSegs);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawCircleTris)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_INT_PARAM( numSegs, 2, 32 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->DrawCircleTris( center, radius, numSegs);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawCircleWire)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_INT_PARAM( numSegs, 2, 32 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->DrawCircleWire( center, radius, numSegs);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawRibbon)
	{
		GM_CHECK_V3_PARAM( start, 0 );
		GM_CHECK_V3_PARAM( end, 1 );
		GM_CHECK_V3_PARAM( right, 2 );
		GM_CHECK_FLOAT_PARAM( thickness, 3 );
		GM_INT_PARAM( numSegs, 4, 32 );
		
		GM_GET_THIS_PTR(VboBuilder, ptr);
		ptr->DrawRibbon( start, end, right, thickness, numSegs );
		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_VOID_VOID( VboBuilder, Clear )
}

GM_REG_MEM_BEGIN(VboBuilder)
GM_REG_MEMFUNC( VboBuilder, Clear )
GM_REG_MEMFUNC( VboBuilder, CreateVbo )
GM_REG_MEMFUNC( VboBuilder, Vertex2f )
GM_REG_MEMFUNC( VboBuilder, Vertex2f_TexCoord2f )
GM_REG_MEMFUNC( VboBuilder, Vertex2f_TexCoord2f_Normal3f )
GM_REG_MEMFUNC( VboBuilder, Vertex2f_Normal3f )
GM_REG_MEMFUNC( VboBuilder, Vertex3f )
GM_REG_MEMFUNC( VboBuilder, Vertex3f_TexCoord2f )
GM_REG_MEMFUNC( VboBuilder, Vertex3f_TexCoord2f_Normal3f )
GM_REG_MEMFUNC( VboBuilder, Vertex3f_Normal3f )
GM_REG_MEMFUNC( VboBuilder, DrawRect )
GM_REG_MEMFUNC( VboBuilder, DrawRectTexCoords )
GM_REG_MEMFUNC( VboBuilder, DrawScreenUV )
GM_REG_MEMFUNC( VboBuilder, DrawCube )
GM_REG_MEMFUNC( VboBuilder, DrawCubeWire )
GM_REG_MEMFUNC( VboBuilder, DrawCircle )
GM_REG_MEMFUNC( VboBuilder, DrawCircleTris )
GM_REG_MEMFUNC( VboBuilder, DrawCircleWire )
GM_REG_MEMFUNC( VboBuilder, DrawRibbon )
GM_REG_MEMFUNC( VboBuilder, Color )


GM_REG_HANDLED_DESTRUCTORS(VboBuilder)
GM_REG_MEM_END()
GM_BIND_DEFINE(VboBuilder)

}