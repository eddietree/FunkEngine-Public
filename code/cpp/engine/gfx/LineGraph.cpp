#include "LineGraph.h"

#include <gm/gmBind.h>
#include <gfx/GLIncludes.h>
#include <gfx/ImmGfx.h>
#include <gfx/RenderState.h>
#include <common/Debug.h>

namespace funk
{

LineGraph::LineGraph( float minVal, float maxVal, v2i dimen, int numValsMaxs )
: m_indexFront(0), m_numMaxVals(numValsMaxs), m_dimen(dimen), m_minVal(minVal), m_maxVal(maxVal)
{
	assert(m_numMaxVals > 0);
	m_vals.reserve( numValsMaxs );
}

void LineGraph::Clear()
{
	m_indexFront = 0;
	m_vals.clear();
}

void LineGraph::Draw( v2 pos )
{
	if ( m_vals.empty() ) return;

	const v2 minPos = pos + v2( 0.0f, (float)-m_dimen.y );
	const v2 maxPos = minPos + v2( (float)m_dimen.x, (float)m_dimen.y );
	const float deltaX = 1.0f / m_numMaxVals;
	const float denom = (m_maxVal-m_minVal);

	ImmGfx &imm = ImmGfx::Ref();
	imm.Color(1.0f, 1.0f, 0.0f);
	RenderState::Ref().SetLineWidth(1.0f);

	imm.BeginDefaultShader();
	imm.BeginDraw();
	imm.PolygonBegin( PRIM_LINE_STRIP );
	
	for ( int i = m_indexFront; i < (int)m_vals.size(); ++i )
	{
		float u = (i-m_indexFront) * deltaX;
		float v = (m_vals[i]-m_minVal)/denom;

		float x = lerp( minPos.x, maxPos.x, u );
		float y = lerp( minPos.y, maxPos.y, v );

		imm.Vertex(x,y);
	}

	if ( m_indexFront > 0 )
	{
		const int offset = (int)m_vals.size()-m_indexFront;

		for ( int i = 0; i < m_indexFront; ++i )
		{
			float u = (i+offset) * deltaX;
			float v = (m_vals[i]-m_minVal)/denom;

			float x = lerp( minPos.x, maxPos.x, u );
			float y = lerp( minPos.y, maxPos.y, v );

			imm.Vertex(x,y);
		}
	}

	imm.PolygonEnd();
	imm.EndDraw();
	imm.EndDefaultShader();
}

void LineGraph::PushVal( float val )
{
	if ( (int)m_vals.size() >= m_numMaxVals )
	{
		m_vals[m_indexFront++] = val;
		m_indexFront %= m_numMaxVals;
	}
	else
	{
		m_vals.push_back(val);
	}
}

void LineGraph::PopVal()
{
	if ( !m_vals.empty() ) m_vals.pop_back();
}

void LineGraph::DrawBG( v2 pos )
{
	const v2 minPos = pos-v2(0.0f, (float)m_dimen.y);
	const v2 dimen = v2( (float)m_dimen.x, (float)m_dimen.y );
	const v2 maxPos = minPos+dimen;

	ImmGfx &imm = ImmGfx::Ref();
		
	imm.BeginDefaultShader();
	imm.BeginDraw();
	
	imm.Color( v3(0.2f, 0.2f, 0.2f), 0.8f );
	imm.DrawRect( minPos, dimen );

	imm.Color( v3(1.0f), 0.1f );
	int numLines = 3;
	for ( int i = 0; i <= numLines+1; ++i )
	{
		float alpha = (float)i/(numLines+1);
		
		float x = lerp( minPos.x, maxPos.x, alpha );
		float y = lerp( minPos.y, maxPos.y, alpha );

		imm.DrawLine( v2(x, minPos.y), v2(x, maxPos.y));
		imm.DrawLine( v2(minPos.x, y), v2(maxPos.x, y));
	}
	
	imm.EndDraw();
	imm.EndDefaultShader();
}

GM_REG_NAMESPACE(LineGraph)
{
	GM_MEMFUNC_CONSTRUCTOR(LineGraph)
	{
		GM_CHECK_FLOAT_PARAM(minVal, 0);
		GM_CHECK_FLOAT_PARAM(maxVal, 1);
		GM_CHECK_V2_PARAM(dimen, 2);
		GM_INT_PARAM(numValsMax,3, 128);
		
		StrongHandle<LineGraph> lineGraph = new LineGraph(minVal, maxVal, v2i((int)dimen.x,(int)dimen.y), numValsMax);
		GM_PUSH_USER_HANDLED( LineGraph, lineGraph );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Dimen)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(LineGraph, ptr);

		v2i dimen = ptr->Dimen();
		a_thread->PushV2( v2((float)dimen.x, (float)dimen.y) );
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_V2( LineGraph, Draw )
	GM_GEN_MEMFUNC_VOID_V2( LineGraph, DrawBG )
	GM_GEN_MEMFUNC_VOID_FLOAT( LineGraph, PushVal )
	GM_GEN_MEMFUNC_VOID_VOID( LineGraph, PopVal )
	GM_GEN_MEMFUNC_VOID_VOID( LineGraph, Clear )
	GM_GEN_MEMFUNC_VOID_FLOAT( LineGraph, SetMinVal )
	GM_GEN_MEMFUNC_VOID_FLOAT( LineGraph, SetMaxVal )
}

GM_REG_MEM_BEGIN(LineGraph)
GM_REG_MEMFUNC( LineGraph, Draw )
GM_REG_MEMFUNC( LineGraph, DrawBG )
GM_REG_MEMFUNC( LineGraph, PushVal )
GM_REG_MEMFUNC( LineGraph, PopVal )
GM_REG_MEMFUNC( LineGraph, Clear )
GM_REG_MEMFUNC( LineGraph, SetMinVal )
GM_REG_MEMFUNC( LineGraph, SetMaxVal )
GM_REG_MEMFUNC( LineGraph, Dimen )
GM_REG_MEM_END()
GM_BIND_DEFINE(LineGraph)

}