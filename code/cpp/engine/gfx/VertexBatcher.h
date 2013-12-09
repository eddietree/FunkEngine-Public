#pragma once

#include <math/v2.h>
#include <math/v3.h>
#include <math/v4.h>
#include "VertexLayout.h"
#include "GLTypes.h"

#include <vector>
#include <functional>

namespace funk
{	
	class VertexBatcher
	{

	protected:

		// data format for vertices
		struct VertexBatcherVert
		{
			v3 pos;
			int16_t normal[4]; // signed shorts
			v4 color;
			v2 uv;
		};

		StrongHandle<VertexLayout> CreateVertexLayout() const;

		inline size_t GetNumBytesPerVertex() const { return sizeof(VertexBatcherVert); }
		inline std::vector<VertexBatcherVert>& GetVertexBatch() { return m_vertex_batch; }

	public:

		// begin/end the shape
		virtual void PolygonBegin( GLPrimtive a_primitive ) {;}
		virtual void PolygonEnd()  {;}
		
		// Draw primitives (2d)
		void DrawLine( const v2 start, const v2 end );
		void DrawLineThick( const v2 start, const v2 end, float thickness );
		void DrawLineThickWire( const v2 start, const v2 end, float thickness );
		void DrawLineThickWithWire( const v2 start, const v2 end, float thickness );
		void DrawLine( const v3 start, const v3 end );
		void DrawLineDashed( const v2 start, const v2 end, float step_size );
		void DrawLineDashed( const v3 start, const v3 end, float step_size );
		void DrawSpline( const v2 v0, const v2 tan0, const v2 v1, const v2 tan1, const float t0 = 0.0f, const float t1 = 1.0f, int numSteps = 64 );
		void DrawSpline( const v3 v0, const v3 tan0, const v3 v1, const v3 tan1, const float t0 = 0.0f, const float t1 = 1.0f, int numSteps = 64 );
		void DrawTriangle(  const v2 v0, const v2 v1, const v2 v2 );
		void DrawTriangle(  const v3 v0, const v3 v1, const v3 v2 );
		void DrawTriangleWire(  const v2 v0, const v2 v1, const v2 v2 );
		void DrawTriangleWire(  const v3 v0, const v3 v1, const v3 v2 );
		void DrawRect( const v2 bottomLeft, const v2 dimen, float z = 0.0f );
		void DrawRectRounded( const v2 bottomLeft, const v2 dimen, float cornerRadius );
		void DrawRectRoundedWire( const v2 bottomLeft, const v2 dimen, float cornerRadius );
		void DrawRectRoundedTop( const v2 bottomLeft, const v2 dimen, float cornerRadius );
		void DrawRectRoundedTopWire( const v2 bottomLeft, const v2 dimen, float cornerRadius );
		void DrawRectRoundedBot( const v2 bottomLeft, const v2 dimen, float cornerRadius );
		void DrawRectRoundedBotWire( const v2 bottomLeft, const v2 dimen, float cornerRadius );
		void DrawRectWire( const v2 bottomLeft, const v2 dimen );
		void DrawRectTexCoords( const v2 bottomLeft, const v2 dimen, v2 uv0 = v2(0.0f,1.0f), v2 uv1=v2(1.0f,0.0f) );
		void DrawQuad( const v2 p0, const v2 p1, const v2 p2, const v2 p3 ); // counter clockwise!!!
		void DrawQuadWire( const v2 p0, const v2 p1, const v2 p2, const v2 p3 ); // counter clockwise!!!
		void DrawQuad( const v3 p0, const v3 p1, const v3 p2, const v3 p3 ); // counter clockwise!!!
		void DrawQuadWire( const v3 p0, const v3 p1, const v3 p2, const v3 p3 ); // counter clockwise!!!
		void DrawScreenUV(  v2 topLeftScreenUV = v2(0,0) /*topleft*/,  v2 dimenScreenUV = v2(1,1) );

		// Circular (2d)
		void DrawCircle( const v2 center, float radius, int segments = 32 );
		void DrawCircleTris( const v2 center, float radius, int segments = 32 ); // uses triangles instead of strip
		void DrawCircleWire( const v2 center, float radius, int segments = 32 );
		void DrawDonut( const v2 center, float radius, float thickness, int segments = 32 );
		void DrawDonutWire( const v2 center, float radius, float thickness, int segments = 32 );
		void DrawArcDonut( const v2 center, float radius, float thickness, float startAngle, float endAngle, int segments = 32 );
		void DrawArcDonutWire( const v2 center, float radius, float thickness, float startAngle, float endAngle, int segments = 32 );
		void DrawArcLine( const v2 center, float radius, float startAngle, float endAngle, int segments = 32 );
		
		// 3D
		void DrawCube( const v3 dimen, const v3 center = v3(0.0f) );
		void DrawCubeWire( const v3 dimen, const v3 center = v3(0.0f) );
		void DrawMayaPlane( int gridsWidth = 10 );
		void DrawBasisVectors();
		void DrawArrow( const v3 startB, const v3 end );
		void DrawSphere( const v3 center = v3(0.0f), float radius = 1.0f );
		void DrawDisk( const v3 center, const v3 dir, float radius, int numSteps = 32 );
		void DrawDiskWire( const v3 center, const v3 dir, float radius, int numSteps = 32 );
		void DrawDiskWireThick( const v3 center, const v3 dir, float radius, float thickness, int numSteps = 32 );
		void DrawRibbon( const v3 start, const v3 end, const v3 right, float thickness, int numSteps = 32 );

	private:

		v4 m_color; // current color
		std::vector<VertexBatcherVert> m_vertex_batch; // used to populate and then send to GPU

	public:
		VertexBatcher( int a_reserve_count );

		// send a vertex
		void Vertex( v3 pos, v2 uv = v2(0.0f), v3 normal = v3(0.0f) );
		inline void Vertex( v2 pos, v2 uv = v2(0.0f), v3 normal = v3(0.0f) ) { Vertex( v3(pos.x, pos.y, 0.0f), uv, normal );  }
		inline void Vertex( float x, float y, float z = 0.0f ) { Vertex( v3(x,y,z)); }

		void Color( v3 color, float alpha = 1.0f );
		void Color( v4 color ) { m_color = color; }
		inline void Color( float x, float y, float z, float a = 1.0f ) { Color(v3(x,y,z), a); }
		v4 GetColor() const { return m_color; }

	private:
		void DrawRoundRectHelper( float startAngleRad, float radius, v2 startPos );

	};
}
