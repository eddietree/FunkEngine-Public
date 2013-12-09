#include "gmExtLibImmGfx.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"

#include <gm/gmBind.h>

#include <gfx/ImmGfx.h>

namespace funk
{
struct gmfImmGfxLib
{
	GM_MEMFUNC_DECL(BeginDefaultShader)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().BeginDefaultShader();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(EndDefaultShader)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().EndDefaultShader();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(GetDefaultShader)
	{
		GM_PUSH_USER_HANDLED( ShaderProgram, ImmGfx::Ref().GetDefaultShader() );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(SetDefaultShaderMVP)
	{
		ImmGfx::Ref().SetDefaultShaderMVP();
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetDefaultShaderPointSize)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_FLOAT_OR_INT_PARAM( a_pt_size, 0);
		ImmGfx::Ref().SetDefaultShaderPointSize(a_pt_size);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(BeginDefaultTexShader)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().BeginDefaultTexShader();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(EndDefaultTexShader)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().EndDefaultTexShader();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(GetDefaultTexShader)
	{
		GM_PUSH_USER_HANDLED( ShaderProgram, ImmGfx::Ref().GetDefaultTexShader() );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(SetDefaultTexShaderMVP)
	{
		ImmGfx::Ref().SetDefaultTexShaderMVP();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(BeginDraw)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().BeginDraw();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(EndDraw)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().EndDraw();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(PolygonBegin)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_draw_prim, 0 );
		ImmGfx::Ref().PolygonBegin( (GLPrimtive)a_draw_prim );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(PolygonEnd)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().PolygonEnd();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex2f)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		
		ImmGfx::Ref().Vertex(a_pos);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex2f_TexCoord2f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );
		
		ImmGfx::Ref().Vertex(a_pos, a_uv);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Vertex2f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_V3_PARAM( a_normal, 1 );
		
		ImmGfx::Ref().Vertex(a_pos, v2(0.0f), a_normal);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex2f_TexCoord2f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );
		GM_CHECK_V3_PARAM( a_normal, 2 );
		
		ImmGfx::Ref().Vertex(a_pos, a_uv, a_normal);
		return GM_OK;
	}	
	
	GM_MEMFUNC_DECL(Vertex3f)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V3_PARAM( a_pos, 0 );
		ImmGfx::Ref().Vertex(a_pos);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex3f_TexCoord2f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V3_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );
		
		ImmGfx::Ref().Vertex(a_pos, a_uv);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex3f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V3_PARAM( a_pos, 0 );
		GM_CHECK_V3_PARAM( a_normal, 1 );
		
		ImmGfx::Ref().Vertex(a_pos, v2(0.0f), a_normal);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Vertex3f_TexCoord2f_Normal3f)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V3_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_uv, 1 );
		GM_CHECK_V3_PARAM( a_normal, 2 );
		
		ImmGfx::Ref().Vertex(a_pos, a_uv, a_normal);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Color)
	{
		GM_CHECK_V3_PARAM( a_color, 0);
		GM_FLOAT_PARAM( a_alpha, 1, 1.0f );
		ImmGfx::Ref().Color( a_color, a_alpha );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Texture)
	{
		GM_USER_PARAM( Texture2d*, tex, 0 );
        StrongHandle<Texture2d> texh = tex;
        ImmGfx::Ref().Texture( texh );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawLine)
	{
		GM_CHECK_NUM_PARAMS(2);
		
		if ( GM_IS_PARAM_V2(0) )
		{
			GM_CHECK_V2_PARAM(p0,0);
			GM_CHECK_V2_PARAM(p1,1);
			ImmGfx::Ref().DrawLine( p0, p1 );
		}
		
		else if ( GM_IS_PARAM_V3(0) )
		{
			GM_CHECK_V3_PARAM(p0,0);
			GM_CHECK_V3_PARAM(p1,1);
			ImmGfx::Ref().DrawLine( p0, p1 );
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawLineDashed)
	{
		GM_CHECK_NUM_PARAMS(3);
		
		if ( GM_IS_PARAM_V2(0) )
		{
			GM_CHECK_V2_PARAM(p0,0);
			GM_CHECK_V2_PARAM(p1,1);
			GM_CHECK_FLOAT_PARAM(step_size,2);
			ImmGfx::Ref().DrawLineDashed( p0, p1, step_size );
		}
		
		else if ( GM_IS_PARAM_V3(0) )
		{
			GM_CHECK_V3_PARAM(p0,0);
			GM_CHECK_V3_PARAM(p1,1);
			GM_CHECK_FLOAT_PARAM(step_size,2);
			ImmGfx::Ref().DrawLineDashed( p0, p1, step_size );
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawLineThick)
	{
		GM_CHECK_NUM_PARAMS(3);

		GM_CHECK_V2_PARAM(p0,0);
		GM_CHECK_V2_PARAM(p1,1);
		GM_CHECK_FLOAT_PARAM(thickness,2);

		ImmGfx::Ref().DrawLineThick( p0, p1, thickness );
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawLineThickWire)
	{
		GM_CHECK_NUM_PARAMS(3);

		GM_CHECK_V2_PARAM(p0,0);
		GM_CHECK_V2_PARAM(p1,1);
		GM_CHECK_FLOAT_PARAM(thickness,2);

		ImmGfx::Ref().DrawLineThickWire( p0, p1, thickness );
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawLineThickWithWire)
	{
		GM_CHECK_NUM_PARAMS(3);

		GM_CHECK_V2_PARAM(p0,0);
		GM_CHECK_V2_PARAM(p1,1);
		GM_CHECK_FLOAT_PARAM(thickness,2);

		ImmGfx::Ref().DrawLineThickWithWire( p0, p1, thickness );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawSpline)
	{
		if ( GM_IS_PARAM_V2(0) )
		{
			GM_CHECK_V2_PARAM(v0,0);
			GM_CHECK_V2_PARAM(tan0,1);
			GM_CHECK_V2_PARAM(v1,2);
			GM_CHECK_V2_PARAM(tan1,3);
			GM_FLOAT_PARAM(t0, 4, 0.0f);
			GM_FLOAT_PARAM(t1, 5, 1.0f);
			GM_INT_PARAM(numSegs, 6, 32);
			
			ImmGfx::Ref().DrawSpline(v0,tan0,v1,tan1,t0,t1,numSegs);
		}
		
		else if ( GM_IS_PARAM_V3(0) )
		{
			GM_CHECK_V3_PARAM(v0,0);
			GM_CHECK_V3_PARAM(tan0,1);
			GM_CHECK_V3_PARAM(v1,2);
			GM_CHECK_V3_PARAM(tan1,3);
			GM_FLOAT_PARAM(t0, 4, 0.0f);
			GM_FLOAT_PARAM(t1, 5, 1.0f);
			GM_INT_PARAM(numSegs, 6, 32);
			
			ImmGfx::Ref().DrawSpline(v0,tan0,v1,tan1,t0,t1,numSegs);
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRect)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		
		ImmGfx::Ref().DrawRect( bottomLeft, dimen );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectRounded)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		GM_CHECK_FLOAT_PARAM( cornerRadius, 2 );
		
		ImmGfx::Ref().DrawRectRounded( bottomLeft, dimen, cornerRadius );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectRoundedWire)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		GM_CHECK_FLOAT_PARAM( cornerRadius, 2 );
		
		ImmGfx::Ref().DrawRectRoundedWire( bottomLeft, dimen, cornerRadius );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectRoundedTop)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		GM_CHECK_FLOAT_PARAM( cornerRadius, 2 );
		
		ImmGfx::Ref().DrawRectRoundedTop( bottomLeft, dimen, cornerRadius );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectRoundedTopWire)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		GM_CHECK_FLOAT_PARAM( cornerRadius, 2 );
		
		ImmGfx::Ref().DrawRectRoundedTopWire( bottomLeft, dimen, cornerRadius );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectRoundedBot)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		GM_CHECK_FLOAT_PARAM( cornerRadius, 2 );
		
		ImmGfx::Ref().DrawRectRoundedBot( bottomLeft, dimen, cornerRadius );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectRoundedBotWire)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		GM_CHECK_FLOAT_PARAM( cornerRadius, 2 );
		
		ImmGfx::Ref().DrawRectRoundedBotWire( bottomLeft, dimen, cornerRadius );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectWire)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( bottomLeft, 0 );
		GM_CHECK_V2_PARAM( dimen, 1 );
		
		ImmGfx::Ref().DrawRectWire( bottomLeft, dimen );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawRectTexCoords)
	{
		int numParams = a_thread->GetNumParams();
		
		if ( numParams == 2 )
		{
			GM_CHECK_V2_PARAM( bottomLeft, 0 );
			GM_CHECK_V2_PARAM( dimen, 1 );
			ImmGfx::Ref().DrawRectTexCoords( bottomLeft, dimen );
		}
		else if ( numParams == 4 )
		{
			GM_CHECK_V2_PARAM( bottomLeft, 0 );
			GM_CHECK_V2_PARAM( dimen, 1 );
			GM_CHECK_V2_PARAM( uv0, 2 );
			GM_CHECK_V2_PARAM( uv1, 3 );
			ImmGfx::Ref().DrawRectTexCoords( bottomLeft, dimen, uv0, uv1 );
			
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

		if ( a_thread->GetNumParams() == 0 )
		{
			ImmGfx::Ref().DrawScreenUV();
		}
		else
		{
			GM_CHECK_V2_PARAM( a_bottom_left, 0);
			GM_CHECK_V2_PARAM( a_dimen, 1);
			ImmGfx::Ref().DrawScreenUV( a_bottom_left, a_dimen );
		}
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawQuad)
	{
		GM_CHECK_NUM_PARAMS(4);	

		if ( GM_IS_PARAM_V2(0) )
		{
			GM_CHECK_V2_PARAM(p0,0);
			GM_CHECK_V2_PARAM(p1,1);
			GM_CHECK_V2_PARAM(p2,2);
			GM_CHECK_V2_PARAM(p3,3);

			ImmGfx::Ref().DrawQuad(p0, p1, p2, p3);
		}
		
		else if ( GM_IS_PARAM_V3(0) )
		{
			GM_CHECK_V3_PARAM(p0,0);
			GM_CHECK_V3_PARAM(p1,1);
			GM_CHECK_V3_PARAM(p2,2);
			GM_CHECK_V3_PARAM(p3,3);

			ImmGfx::Ref().DrawQuad(p0, p1, p2, p3);
		}

		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawQuadWire)
	{
		GM_CHECK_NUM_PARAMS(4);	

		if ( GM_IS_PARAM_V2(0) )
		{
			GM_CHECK_V2_PARAM(p0,0);
			GM_CHECK_V2_PARAM(p1,1);
			GM_CHECK_V2_PARAM(p2,2);
			GM_CHECK_V2_PARAM(p3,3);

			ImmGfx::Ref().DrawQuadWire(p0, p1, p2, p3);
		}
		
		else if ( GM_IS_PARAM_V3(0) )
		{
			GM_CHECK_V3_PARAM(p0,0);
			GM_CHECK_V3_PARAM(p1,1);
			GM_CHECK_V3_PARAM(p2,2);
			GM_CHECK_V3_PARAM(p3,3);

			ImmGfx::Ref().DrawQuadWire(p0, p1, p2, p3);
		}

		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawCircle)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_INT_PARAM( numSegs, 2, 32 );
		
		ImmGfx::Ref().DrawCircle( center, radius, numSegs);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawCircleWire)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_INT_PARAM( numSegs, 2, 32 );
		
		ImmGfx::Ref().DrawCircleWire( center, radius, numSegs);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawDonut)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_CHECK_FLOAT_PARAM( thickness, 2);
		GM_INT_PARAM( numSegs, 3, 32 );
		
		ImmGfx::Ref().DrawDonut( center, radius, thickness, numSegs);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawDonutWire)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_CHECK_FLOAT_PARAM( thickness, 2);
		GM_INT_PARAM( numSegs, 3, 32 );
		
		ImmGfx::Ref().DrawDonutWire( center, radius, thickness, numSegs);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawArcDonut)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_CHECK_FLOAT_PARAM( thickness, 2);
		GM_CHECK_FLOAT_PARAM( startAngle, 3);
		GM_CHECK_FLOAT_PARAM( endAngle, 4);
		GM_INT_PARAM( numSegs, 5, 32 );
		
		ImmGfx::Ref().DrawArcDonut( center, radius, thickness, startAngle, endAngle, numSegs);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawArcDonutWire)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_CHECK_FLOAT_PARAM( thickness, 2);
		GM_CHECK_FLOAT_PARAM( startAngle, 3);
		GM_CHECK_FLOAT_PARAM( endAngle, 4);
		GM_INT_PARAM( numSegs, 5, 32 );
		
		ImmGfx::Ref().DrawArcDonutWire( center, radius, thickness, startAngle, endAngle, numSegs);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawArcLine)
	{
		GM_CHECK_V2_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1);
		GM_CHECK_FLOAT_PARAM( startAngle, 2);
		GM_CHECK_FLOAT_PARAM( endAngle, 3);
		GM_INT_PARAM( numSegs, 4, 32 );
		
		ImmGfx::Ref().DrawArcLine( center, radius, startAngle, endAngle, numSegs);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawTriangle)
	{
		GM_CHECK_NUM_PARAMS(3);
		
		if ( GM_IS_PARAM_V2(0) )
		{
			GM_CHECK_V2_PARAM(p0,0);
			GM_CHECK_V2_PARAM(p1,1);
			GM_CHECK_V2_PARAM(p2,2);
			ImmGfx::Ref().DrawTriangle( p0, p1, p2 );
		}
		
		else if ( GM_IS_PARAM_V3(0) )
		{
			GM_CHECK_V3_PARAM(p0,0);
			GM_CHECK_V3_PARAM(p1,1);
			GM_CHECK_V3_PARAM(p2,2);
			ImmGfx::Ref().DrawTriangle( p0, p1, p2 );
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawTriangleWire)
	{
		GM_CHECK_NUM_PARAMS(3);
		
		if ( GM_IS_PARAM_V2(0) )
		{
			GM_CHECK_V2_PARAM(p0,0);
			GM_CHECK_V2_PARAM(p1,1);
			GM_CHECK_V2_PARAM(p2,2);
			ImmGfx::Ref().DrawTriangleWire( p0, p1, p2 );
		}
		
		else if ( GM_IS_PARAM_V3(0) )
		{
			GM_CHECK_V3_PARAM(p0,0);
			GM_CHECK_V3_PARAM(p1,1);
			GM_CHECK_V3_PARAM(p2,2);
			ImmGfx::Ref().DrawTriangleWire( p0, p1, p2 );
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawRibbon)
	{
		GM_CHECK_V3_PARAM( start, 0 );
		GM_CHECK_V3_PARAM( end, 1 );
		GM_CHECK_V3_PARAM( right, 2 );
		GM_CHECK_FLOAT_PARAM( thickness, 3 );
		GM_INT_PARAM( numSegs, 4, 32 );
		
		ImmGfx::Ref().DrawRibbon( start, end, right, thickness, numSegs );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawCube)
	{
		int numParams = a_thread->GetNumParams();
		GM_ASSERT(numParams >= 1 && numParams <= 2 );
		GM_CHECK_V3_PARAM( dimen, 0 );
		if ( numParams == 2 )
		{
			GM_CHECK_V3_PARAM( pos, 1 );
			ImmGfx::Ref().DrawCube( dimen, pos );
		}
		else
		{
			ImmGfx::Ref().DrawCube( dimen );
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawCubeWire)
	{
		int numParams = a_thread->GetNumParams();
		GM_ASSERT(numParams >= 1 && numParams <= 2 );
		GM_CHECK_V3_PARAM( dimen, 0 );
		if ( numParams == 2 )
		{
			GM_CHECK_V3_PARAM( pos, 1 );
			ImmGfx::Ref().DrawCubeWire( dimen, pos );
		}
		else
		{
			ImmGfx::Ref().DrawCubeWire( dimen );
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawMayaPlane)
	{
		//GM_CHECK_NUM_PARAMS(1);
		GM_INT_PARAM( numGrids, 0, 10 );
		
		ImmGfx::Ref().DrawMayaPlane( (int)numGrids);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawBasisVectors)
	{
		GM_CHECK_NUM_PARAMS(0);
		ImmGfx::Ref().DrawBasisVectors();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawArrow)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V3_PARAM( start, 0 );
		GM_CHECK_V3_PARAM( end, 1 );
		
		ImmGfx::Ref().DrawArrow( start, end );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawSphere)
	{
		GM_CHECK_V3_PARAM( center, 0 );
		GM_CHECK_FLOAT_PARAM( radius, 1 );
		
		ImmGfx::Ref().DrawSphere( center, radius );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawDisk)
	{
		GM_CHECK_V3_PARAM( center, 0 );
		GM_CHECK_V3_PARAM( dir, 1 );
		GM_CHECK_FLOAT_OR_INT_PARAM( radius, 2 );
		GM_INT_PARAM( numSteps, 3, 16 );
		
		ImmGfx::Ref().DrawDisk( center, dir, radius, numSteps );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawDiskWire)
	{
		GM_CHECK_V3_PARAM( center, 0 );
		GM_CHECK_V3_PARAM( dir, 1 );
		GM_CHECK_FLOAT_OR_INT_PARAM( radius, 2 );
		GM_INT_PARAM( numSteps, 3, 16 );
		
		ImmGfx::Ref().DrawDiskWire( center, dir, radius, numSteps );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(DrawDiskWireThick)
	{
		GM_CHECK_V3_PARAM( center, 0 );
		GM_CHECK_V3_PARAM( dir, 1 );
		GM_CHECK_FLOAT_OR_INT_PARAM( radius, 2 );
		GM_CHECK_FLOAT_OR_INT_PARAM( thickness, 3 );
		GM_INT_PARAM( numSteps, 4, 16 );
		
		ImmGfx::Ref().DrawDiskWireThick( center, dir, radius, thickness, numSteps );
		return GM_OK;
	}
};

static gmFunctionEntry s_gmImmGfxLib[] =
{
	// default shader
	GM_LIBFUNC_ENTRY(BeginDefaultShader, ImmGfx)
	GM_LIBFUNC_ENTRY(EndDefaultShader, ImmGfx)
	GM_LIBFUNC_ENTRY(GetDefaultShader, ImmGfx)
	GM_LIBFUNC_ENTRY(SetDefaultShaderMVP, ImmGfx)
	GM_LIBFUNC_ENTRY(SetDefaultShaderPointSize, ImmGfx)

	// default textured shader
	GM_LIBFUNC_ENTRY(BeginDefaultTexShader, ImmGfx)
	GM_LIBFUNC_ENTRY(EndDefaultTexShader, ImmGfx)
	GM_LIBFUNC_ENTRY(GetDefaultTexShader, ImmGfx)
	GM_LIBFUNC_ENTRY(SetDefaultTexShaderMVP, ImmGfx)

	GM_LIBFUNC_ENTRY(BeginDraw, ImmGfx)
	GM_LIBFUNC_ENTRY(EndDraw, ImmGfx)
	
	// custom build
	GM_LIBFUNC_ENTRY(PolygonBegin, ImmGfx)
	GM_LIBFUNC_ENTRY(PolygonEnd, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex2f, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex2f_TexCoord2f, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex2f_TexCoord2f_Normal3f, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex2f_Normal3f, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex3f, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex3f_TexCoord2f, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex3f_TexCoord2f_Normal3f, ImmGfx)
	GM_LIBFUNC_ENTRY(Vertex3f_Normal3f, ImmGfx)
	GM_LIBFUNC_ENTRY(Color, ImmGfx)
	GM_LIBFUNC_ENTRY(Texture, ImmGfx)
	
	// Draw Primitives
	GM_LIBFUNC_ENTRY(DrawLine, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawLineDashed, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawLineThick, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawLineThickWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawLineThickWithWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawSpline, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRect, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectTexCoords, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectRounded, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectRoundedWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectRoundedTop, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectRoundedTopWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectRoundedBot, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRectRoundedBotWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawQuad, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawQuadWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawScreenUV, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawCircle, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawCircleWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawDonut, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawDonutWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawArcDonut, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawArcDonutWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawArcLine, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawTriangle, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawTriangleWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawRibbon, ImmGfx)
	
	GM_LIBFUNC_ENTRY(DrawCube, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawCubeWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawMayaPlane, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawBasisVectors, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawSphere, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawArrow, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawDisk, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawDiskWire, ImmGfx)
	GM_LIBFUNC_ENTRY(DrawDiskWireThick, ImmGfx)
};

void gmBindImmGfxLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmImmGfxLib, sizeof(s_gmImmGfxLib) / sizeof(s_gmImmGfxLib[0]), "Imm" );
}

}