#include "VertexBatcher.h"

#include <math/ColorUtil.h>
#include <common/Debug.h>

#include <stdint.h>

namespace funk
{
	const float PI = 3.14159f;

	VertexBatcher::VertexBatcher( int a_reserve_count ) : m_color(0.0f)
	{
		m_vertex_batch.reserve(a_reserve_count);
	}
	
	void VertexBatcher::Vertex( v3 pos, v2 uv, v3 normal )
	{
		// cannot exceed scratch space
		ASSERT( m_vertex_batch.size() < m_vertex_batch.capacity() );

		// hande the normal
		int16_t max_val = int16_t(int16_t(1<<15)-1);
		int16_t normal_x = int16_t( max_val * normal.x);
		int16_t normal_y = int16_t( max_val * normal.y);
		int16_t normal_z = int16_t( max_val * normal.z);
		int16_t normal_w = int16_t( max_val );

		VertexBatcherVert vert;
		
		vert.pos	= pos;
		vert.uv		= uv;
		vert.color  = m_color;

		vert.normal[0] = normal_x;
		vert.normal[1] = normal_y;
		vert.normal[2] = normal_z;
		vert.normal[3] = normal_w;
		
		m_vertex_batch.push_back(vert);
	}
	
	void VertexBatcher::Color( v3 color, float alpha )
	{
		m_color.x = color.x;
		m_color.y = color.y;
		m_color.z = color.z;
		m_color.w = alpha;
	}

	StrongHandle<VertexLayout> VertexBatcher::CreateVertexLayout() const
	{
		StrongHandle<VertexLayout> result = new VertexLayout;

		result->BeginEntries( sizeof(VertexBatcherVert) );
		result->AddEntry( DATATYPE_FLOAT, 3, offsetof(VertexBatcherVert,pos) );
		result->AddEntry( DATATYPE_FLOAT, 2, offsetof(VertexBatcherVert,uv) );
		result->AddEntry( DATATYPE_SHORT, 4, offsetof(VertexBatcherVert,normal), true );
		result->AddEntry( DATATYPE_FLOAT, 4, offsetof(VertexBatcherVert,color) );
		result->EndEntries();

		return result;
	}

	void VertexBatcher::DrawLine( const v2 start, const v2 end )
	{
		PolygonBegin( PRIM_LINES );
		Vertex( start.x, start.y );
		Vertex( end.x, end.y );
		PolygonEnd();
	}

	void VertexBatcher::DrawLineThick( const v2 start, const v2 end, float thickness )
	{
		v2 dir = end - start;
		float len = length(dir);
		if ( len < 0.01f ) return;

		v2 dir_normalized = dir/len;
		v2 dir_perp = perpendicular(dir_normalized);
		v2 dir_opposite_top = dir_perp * thickness * 0.5f;
		v2 dir_opposite_bot = -dir_perp * thickness * 0.5f;

		v2 pos0 = start + dir_opposite_top;
		v2 pos1 = start + dir_opposite_bot;
		v2 pos2 = end + dir_opposite_bot;
		v2 pos3 = end + dir_opposite_top;
		
		PolygonBegin( PRIM_TRIANGLES );
		Vertex( pos0 );
		Vertex( pos1 );
		Vertex( pos2 );
		Vertex( pos0 );
		Vertex( pos2 );
		Vertex( pos3 );
		PolygonEnd();
	}

	void VertexBatcher::DrawLineThickWire( const v2 start, const v2 end, float thickness )
	{
		v2 dir = end - start;
		float len = length(dir);
		if ( len < 0.01f ) return;

		v2 dir_normalized = dir/len;
		v2 dir_perp = perpendicular(dir_normalized);
		v2 dir_opposite_top = dir_perp * thickness * 0.5f;
		v2 dir_opposite_bot = -dir_perp * thickness * 0.5f;

		v2 pos0 = start + dir_opposite_top;
		v2 pos1 = start + dir_opposite_bot;
		v2 pos2 = end + dir_opposite_bot;
		v2 pos3 = end + dir_opposite_top;
		
		PolygonBegin( PRIM_LINE_LOOP );
		Vertex( pos0 );
		Vertex( pos1 );
		Vertex( pos2 );
		Vertex( pos3 );
		PolygonEnd();
	}

	void VertexBatcher::DrawLineThickWithWire( const v2 start, const v2 end, float thickness )
	{
		v2 dir = end - start;
		float len = length(dir);
		if ( len < 0.01f ) return;

		v2 dir_normalized = dir/len;
		v2 dir_perp = perpendicular(dir_normalized);
		v2 dir_opposite_top = dir_perp * thickness * 0.5f;
		v2 dir_opposite_bot = -dir_perp * thickness * 0.5f;

		v2 pos0 = start + dir_opposite_top;
		v2 pos1 = start + dir_opposite_bot;
		v2 pos2 = end + dir_opposite_bot;
		v2 pos3 = end + dir_opposite_top;
		
		PolygonBegin( PRIM_TRIANGLES );
		Vertex( pos0 );
		Vertex( pos1 );
		Vertex( pos2 );
		Vertex( pos0 );
		Vertex( pos2 );
		Vertex( pos3 );
		PolygonEnd();

		PolygonBegin( PRIM_LINE_LOOP );
		Vertex( pos0 );
		Vertex( pos1 );
		Vertex( pos2 );
		Vertex( pos3 );
		PolygonEnd();
	}
	
	void VertexBatcher::DrawLine( const v3 start, const v3 end )
	{
		PolygonBegin( PRIM_LINES );
		Vertex( start );
		Vertex( end );
		PolygonEnd();
	}

	void VertexBatcher::DrawLineDashed( const v2 start, const v2 end, float step_size )
	{
		DrawLineDashed( v3(start.x, start.y, 0.0f), v3(end.x, end.y, 0.0f), step_size );
	}

	void VertexBatcher::DrawLineDashed( const v3 start, const v3 end, float step_size )
	{
		const v3 diff = end-start;
		const float len = length(diff);

		if ( len < 0.01f || step_size < 0.01f ) return;

		const v3 dir = diff / len;
		const v3 dir_step = dir * step_size;
		const int num_steps = int( len / step_size );

		PolygonBegin( PRIM_LINES );

		for( int i = 0; i < num_steps; i+=2 )
		{
			float index = float(i);

			v3 pos0 = start + dir_step * index;
			v3 pos1 = start + dir_step * (index+1.0f);

			Vertex( pos0 );
			Vertex( pos1 );
		}

		PolygonEnd();
	}
	
	void VertexBatcher::DrawRect( const v2 bottomLeft, const v2 dimen, float z )
	{
		v3 pos[4] =
		{
			v3( bottomLeft.x, bottomLeft.y, z ),
			v3( bottomLeft.x + dimen.x, bottomLeft.y, z ),
			v3( bottomLeft.x + dimen.x, bottomLeft.y + dimen.y, z),
			v3( bottomLeft.x, bottomLeft.y + dimen.y, z),
		};
		
		v2 uv[4] =
		{
			v2(0.0f,0.0f),
			v2(1.0f,0.0f),
			v2(1.0f,1.0f),
			v2(0.0f,1.0f),
		};
		
		v3 normal(0.0f,0.0f,1.0f);
		
		
		PolygonBegin( PRIM_TRIANGLES );
		
		Vertex( pos[0], uv[0], normal );
		Vertex( pos[1], uv[1], normal );
		Vertex( pos[2], uv[2], normal );
		Vertex( pos[0], uv[0], normal );
		Vertex( pos[2], uv[2], normal );
		Vertex( pos[3], uv[3], normal );
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawCircle( const v2 center, float radius, int segments )
	{
		const float delta_angle = 2.0f * PI / segments;
		
		PolygonBegin( PRIM_TRIANGLE_FAN );
		
		Vertex( center, v2(0.5f) );
		
		for ( int i = 0; i <= segments; ++i )
		{
			const float currAngle = delta_angle * i;
			const v2 dir_normalized = unitCircle(currAngle);
			const v2 uv = dir_normalized * 0.5f + v2(0.5f);

			v2 pos = center + dir_normalized * radius;
			Vertex( pos, uv );
		}
		
		PolygonEnd();
	}

	void VertexBatcher::DrawCircleTris( const v2 center, float radius, int segments )
	{
		const float delta_angle = 2.0f * PI / segments;
		
		PolygonBegin( PRIM_TRIANGLES );
		
		for ( int i = 0; i < segments; ++i )
		{
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i+1);
			v2 pos_0 = center + unitCircle(angle_0) * radius;
			v2 pos_1 = center + unitCircle(angle_1) * radius;

			Vertex( center );
			Vertex( pos_0 );
			Vertex( pos_1 );
		}
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawCircleWire( const v2 center, float radius, int segments )
	{
		float delta_angle = 2.0f * PI / segments;
		
		PolygonBegin( PRIM_LINE_LOOP );
		
		for ( int i = 0; i < segments; ++i )
		{
			float currAngle = delta_angle * i;
			v2 pos = center + unitCircle(currAngle) * radius;
			Vertex( pos.x, pos.y );
		}
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawDonut( const v2 center, float radius, float thickness, int segments /*= 32 */ )
	{
		float delta_angle = 2.0f * PI / segments;
		
		PolygonBegin( PRIM_TRIANGLE_STRIP );
		
		for ( int i = 0; i <= segments; ++i )
		{
			float currAngle = delta_angle * i;
			
			v2 dir = v2(cosf(currAngle), sinf(currAngle) );
			v2 pos0 = center+ (radius-0.5f*thickness)*dir;
			v2 pos1 = center+ (radius+0.5f*thickness)*dir;
			
			Vertex( pos0 );
			Vertex( pos1 );
		}
		
		PolygonEnd();
	}

	void VertexBatcher::DrawDonutWire( const v2 center, float radius, float thickness, int segments /*= 32 */ )
	{
		float delta_angle = 2.0f * PI / segments;
		
		PolygonBegin( PRIM_LINES );
		
		for ( int i = 0; i < segments; ++i )
		{
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i+1.0f);
			
			v2 dir_0 = v2(cosf(angle_0), sinf(angle_0) );
			v2 dir_1 = v2(cosf(angle_1), sinf(angle_1) );

			v2 pos_0_0 = center+ (radius-0.5f*thickness)*dir_0;
			v2 pos_0_1 = center+ (radius-0.5f*thickness)*dir_1;

			v2 pos_1_0 = center+ (radius+0.5f*thickness)*dir_0;
			v2 pos_1_1 = center+ (radius+0.5f*thickness)*dir_1;
			
			Vertex( pos_0_0 );
			Vertex( pos_0_1 );
			Vertex( pos_1_0 );
			Vertex( pos_1_1 );
		}
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawArcDonut( const v2 center, float radius, float thickness, float startAngle, float endAngle, int segments )
	{
		float delta_angle = (endAngle-startAngle) / (segments);
		
		PolygonBegin( PRIM_TRIANGLE_STRIP );
		
		for ( int i = 0; i <= segments; ++i )
		{
			float currAngle = startAngle + delta_angle * i;
			
			v2 dir = v2(cosf(currAngle), sinf(currAngle) );
			v2 pos0 = center+ (radius-0.5f*thickness)*dir;
			v2 pos1 = center+ (radius+0.5f*thickness)*dir;
			
			Vertex( pos0.x, pos0.y );
			Vertex( pos1.x, pos1.y );
		}
		
		PolygonEnd();
	}

	void VertexBatcher::DrawArcDonutWire( const v2 center, float radius, float thickness, float startAngle, float endAngle, int segments )
	{
		float delta_angle = (endAngle-startAngle) / (segments);
		
		PolygonBegin( PRIM_LINES );
		
		for ( int i = 0; i < segments; ++i )
		{
			float angle_0 = startAngle + delta_angle * i;
			float angle_1 = startAngle + delta_angle * (i+1.0f);
			
			v2 dir_0 = v2(cosf(angle_0), sinf(angle_0) );
			v2 dir_1 = v2(cosf(angle_1), sinf(angle_1) );

			v2 pos_0_0 = center+ (radius-0.5f*thickness)*dir_0;
			v2 pos_0_1 = center+ (radius-0.5f*thickness)*dir_1;
			v2 pos_1_0 = center+ (radius+0.5f*thickness)*dir_0;
			v2 pos_1_1 = center+ (radius+0.5f*thickness)*dir_1;

			Vertex( pos_0_0 );
			Vertex( pos_0_1 );
			Vertex( pos_1_0 );
			Vertex( pos_1_1 );
		}
		
		PolygonEnd();
	}

	
	void VertexBatcher::DrawArcLine( const v2 center, float radius, float startAngle, float endAngle, int segments )
	{
		float delta_angle = (endAngle-startAngle) / (segments-1);
		
		PolygonBegin( PRIM_LINES );
		
		for ( int i = 0; i < segments-1; ++i )
		{
			float currAngle = startAngle + delta_angle * i;
			float nextAngle = startAngle + delta_angle * (i+1);
			
			float x0 = center.x + radius * cosf( currAngle );
			float y0 = center.y + radius * sinf( currAngle );
			float x1 = center.x + radius * cosf( nextAngle );
			float y1 = center.y + radius * sinf( nextAngle );
			
			Vertex( x0, y0 );
			Vertex( x1, y1 );
		}
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawTriangle( const v2 v0, const v2 v1, const v2 v2 )
	{
		PolygonBegin( PRIM_TRIANGLES );
		Vertex( v0.x, v0.y );
		Vertex( v1.x, v1.y );
		Vertex( v2.x, v2.y );
		PolygonEnd();
	}
	
	void VertexBatcher::DrawTriangleWire( const v2 v0, const v2 v1, const v2 v2 )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		Vertex( v0.x, v0.y );
		Vertex( v1.x, v1.y );
		Vertex( v2.x, v2.y );
		PolygonEnd();
	}
	
	void VertexBatcher::DrawTriangleWire( const v3 v0, const v3 v1, const v3 v2 )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		Vertex( v0.x, v0.y, v0.z );
		Vertex( v1.x, v1.y, v1.z );
		Vertex( v2.x, v2.y, v2.z );
		PolygonEnd();
	}
	
	void VertexBatcher::DrawTriangle( const v3 v0, const v3 v1, const v3 v2 )
	{
		PolygonBegin( PRIM_TRIANGLES );
		Vertex( v0.x, v0.y, v0.z );
		Vertex( v1.x, v1.y, v1.z );
		Vertex( v2.x, v2.y, v2.z );
		PolygonEnd();
	}
	
	void VertexBatcher::DrawRectTexCoords( const v2 bottomLeft, const v2 dimen, v2 uv0, v2 uv1 )
	{
		v2 uv[4] =
		{
			v2(uv0.x, uv0.y),
			v2(uv1.x, uv0.y),
			v2(uv1.x, uv1.y),
			v2(uv0.x, uv1.y),
		};
		
		v2 pos[4] =
		{
			v2( bottomLeft.x, bottomLeft.y ),
			v2( bottomLeft.x + dimen.x, bottomLeft.y ),
			v2( bottomLeft.x + dimen.x, bottomLeft.y + dimen.y),
			v2( bottomLeft.x, bottomLeft.y + dimen.y),
		};
		
		v3 normal(0.0f,0.0f,1.0f);
		
		PolygonBegin( PRIM_TRIANGLES );
		
		Vertex( pos[0], uv[0], normal );
		Vertex( pos[1], uv[1], normal );
		Vertex( pos[2], uv[2], normal );
		
		Vertex( pos[0], uv[0], normal );
		Vertex( pos[2], uv[2], normal );
		Vertex( pos[3], uv[3], normal );
		
		PolygonEnd();
	}

	void VertexBatcher::DrawScreenUV( v2 topLeftUV, v2 dimenUV )
	{
		topLeftUV = topLeftUV *2.0f - v2(1.0f);
		topLeftUV.y *= -1.0f;
		dimenUV *= 2.0f;

		v2 br = v2( topLeftUV.x+dimenUV.x, topLeftUV.y-dimenUV.y );
		v2 bl = v2( topLeftUV.x, topLeftUV.y-dimenUV.y );
		v2 tl = v2( topLeftUV.x, topLeftUV.y );
		v2 tr = v2( topLeftUV.x+dimenUV.x, topLeftUV.y );

		PolygonBegin( PRIM_TRIANGLES );

		Vertex( bl, v2( 0.0f, 0.0f) );
		Vertex( br, v2( 1.0f, 0.0f) );
		Vertex( tr, v2( 1.0f, 1.0f) );
		Vertex( bl, v2( 0.0f, 0.0f) );
		Vertex( tr, v2( 1.0f, 1.0f) );
		Vertex( tl, v2( 0.0f, 1.0f) );

		PolygonEnd();
	}
		
	void VertexBatcher::DrawRectWire( const v2 bottomLeft, const v2 dimen )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		Vertex( bottomLeft.x + dimen.x, bottomLeft.y  );
		Vertex( bottomLeft.x, bottomLeft.y );
		Vertex( bottomLeft.x, bottomLeft.y + dimen.y );
		Vertex( bottomLeft.x + dimen.x, bottomLeft.y  + dimen.y);
		PolygonEnd();
	}
	
	void VertexBatcher::DrawRoundRectHelper( float startAngleRad, float radius, v2 startPos )
	{
		const int numRoundedVerts = 4;
		const float roundedCornerAngle = PI*0.5f;
		const float roundedCircledelta_angle = roundedCornerAngle / (numRoundedVerts-1);
		
		for(int i = 0; i < numRoundedVerts; ++i )
		{
			float angleRad = startAngleRad + i*roundedCircledelta_angle;
			v2 pos = startPos + unitCircle(angleRad)*radius;
			Vertex(pos.x, pos.y);
		}
	}
	
	void VertexBatcher::DrawRectRounded( const v2 bottomLeft, const v2 dimen, float cornerRadius )
	{
		PolygonBegin( PRIM_TRIANGLE_FAN );
		
		// center
		const v2 centerPos = bottomLeft + dimen*0.5f;
		Vertex(centerPos.x, centerPos.y);
		
		DrawRoundRectHelper( PI, cornerRadius, bottomLeft + v2(cornerRadius) ); // bottom left
		DrawRoundRectHelper( 1.5f*PI, cornerRadius, bottomLeft + v2(dimen.x-cornerRadius, cornerRadius) ); // bottom right
		DrawRoundRectHelper( 0.0f, cornerRadius, bottomLeft + dimen - v2(cornerRadius) ); // top right
		DrawRoundRectHelper( PI*0.5f, cornerRadius, bottomLeft + v2(cornerRadius, dimen.y-cornerRadius) ); // top right
		
		// back to start
		v2 finalPos = bottomLeft + v2(0.0f, cornerRadius);
		Vertex( finalPos.x, finalPos.y );
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawRectRoundedTop( const v2 bottomLeft, const v2 dimen, float cornerRadius )
	{
		PolygonBegin( PRIM_TRIANGLE_FAN );
		
		// center
		const v2 centerPos = bottomLeft + dimen*0.5f;
		Vertex(centerPos.x, centerPos.y);
		
		Vertex(bottomLeft.x, bottomLeft.y); // bottom left
		Vertex(bottomLeft.x + dimen.x, bottomLeft.y); // bottom right
		DrawRoundRectHelper( 0.0f, cornerRadius, bottomLeft + dimen - v2(cornerRadius) ); // top right
		DrawRoundRectHelper( PI*0.5f, cornerRadius, bottomLeft + v2(cornerRadius, dimen.y-cornerRadius) ); // top right
		Vertex(bottomLeft.x, bottomLeft.y); // bottom left, back to start
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawRectRoundedTopWire( const v2 bottomLeft, const v2 dimen, float cornerRadius )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		
		Vertex(bottomLeft.x, bottomLeft.y); // bottom left
		Vertex(bottomLeft.x + dimen.x, bottomLeft.y); // bottom right
		DrawRoundRectHelper( 0.0f, cornerRadius, bottomLeft + dimen - v2(cornerRadius) ); // top right
		DrawRoundRectHelper( PI*0.5f, cornerRadius, bottomLeft + v2(cornerRadius, dimen.y-cornerRadius) ); // top right
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawRectRoundedBot( const v2 bottomLeft, const v2 dimen, float cornerRadius )
	{
		PolygonBegin( PRIM_TRIANGLE_FAN );
		
		// center
		const v2 centerPos = bottomLeft + dimen*0.5f;
		Vertex(centerPos.x, centerPos.y);
		
		DrawRoundRectHelper( PI, cornerRadius, bottomLeft + v2(cornerRadius) ); // bottom left
		DrawRoundRectHelper( 1.5f*PI, cornerRadius, bottomLeft + v2(dimen.x-cornerRadius, cornerRadius) ); // bottom right
		Vertex(bottomLeft.x+dimen.x, bottomLeft.y+dimen.y); // top right
		Vertex(bottomLeft.x, bottomLeft.y+dimen.y); // top left
		
		// back to start
		v2 finalPos = bottomLeft + v2(0.0f, cornerRadius);
		Vertex( finalPos.x, finalPos.y );
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawRectRoundedBotWire( const v2 bottomLeft, const v2 dimen, float cornerRadius )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		
		DrawRoundRectHelper( PI, cornerRadius, bottomLeft + v2(cornerRadius) ); // bottom left
		DrawRoundRectHelper( 1.5f*PI, cornerRadius, bottomLeft + v2(dimen.x-cornerRadius, cornerRadius) ); // bottom right
		Vertex(bottomLeft.x+dimen.x, bottomLeft.y+dimen.y); // top right
		Vertex(bottomLeft.x, bottomLeft.y+dimen.y); // top left
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawRectRoundedWire( const v2 bottomLeft, const v2 dimen, float cornerRadius )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		
		DrawRoundRectHelper( PI, cornerRadius, bottomLeft + v2(cornerRadius) ); // bottom left
		DrawRoundRectHelper( 1.5f*PI, cornerRadius, bottomLeft + v2(dimen.x-cornerRadius, cornerRadius) ); // bottom right
		DrawRoundRectHelper( 0.0f, cornerRadius, bottomLeft + dimen - v2(cornerRadius) ); // top right
		DrawRoundRectHelper( PI*0.5f, cornerRadius, bottomLeft + v2(cornerRadius, dimen.y-cornerRadius) ); // top right
		
		PolygonEnd();
	}

	void VertexBatcher::DrawQuad( const v2 p0, const v2 p1, const v2 p2, const v2 p3 )
	{
		PolygonBegin( PRIM_TRIANGLES );
		Vertex( p0, v2(0.0f) );
		Vertex( p1, v2(1.0f, 0.0f) );
		Vertex( p2, v2(1.0f, 1.0f) );
		Vertex( p0, v2(0.0f, 0.0f) );
		Vertex( p2, v2(1.0f, 1.0f) );
		Vertex( p3, v2(0.0f, 1.0f) );
		PolygonEnd();
	}

	void VertexBatcher::DrawQuadWire( const v2 p0, const v2 p1, const v2 p2, const v2 p3 )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		Vertex( p0, v2(0.0f) );
		Vertex( p1, v2(1.0f, 0.0f) );
		Vertex( p2, v2(1.0f, 1.0f) );
		Vertex( p3, v2(0.0f, 1.0f) );
		PolygonEnd();
	}

	void VertexBatcher::DrawQuad( const v3 p0, const v3 p1, const v3 p2, const v3 p3 )
	{
		PolygonBegin( PRIM_TRIANGLES );
		Vertex( p0, v2(0.0f) );
		Vertex( p1, v2(1.0f, 0.0f) );
		Vertex( p2, v2(1.0f, 1.0f) );
		Vertex( p0, v2(0.0f, 0.0f) );
		Vertex( p2, v2(1.0f, 1.0f) );
		Vertex( p3, v2(0.0f, 1.0f) );
		PolygonEnd();
	}

	void VertexBatcher::DrawQuadWire( const v3 p0, const v3 p1, const v3 p2, const v3 p3 )
	{
		PolygonBegin( PRIM_LINE_LOOP );
		Vertex( p0, v2(0.0f) );
		Vertex( p1, v2(1.0f, 0.0f) );
		Vertex( p2, v2(1.0f, 1.0f) );
		Vertex( p3, v2(0.0f, 1.0f) );
		PolygonEnd();
	}
	
	void VertexBatcher::DrawCube( const v3 dimen, const v3 center )
	{
		v3 dimenHalf = dimen * 0.5f;
		
		v3 normals[6] =
		{
			v3(0.0f,0.0f,1.0f),
			v3(0.0f,0.0f,-1.0f),
			v3(-1.0f,0.0f,0.0f),
			v3(1.0f,0.0f,0.0f),
			v3(0.0f,-1.0f,0.0f),
			v3(0.0f,1.0f,0.0f),
		};
		
		v2 uv[4] =
		{
			v2(0.0f,0.0f),
			v2(1.0f,0.0f),
			v2(1.0f,1.0f),
			v2(0.0f,1.0f),
		};
		
		v3 pos[8] =
		{
			v3( center.x -dimenHalf.x, center.y-dimenHalf.y, center.z +dimenHalf.z ), // 0 (-,-,+)
			v3( center.x -dimenHalf.x, center.y+dimenHalf.y, center.z +dimenHalf.z ), // 1 (-,+,+)
			v3( center.x +dimenHalf.x, center.y+dimenHalf.y, center.z +dimenHalf.z ), // 2 (+,+,+)
			v3( center.x +dimenHalf.x, center.y-dimenHalf.y, center.z +dimenHalf.z ), // 3 (+,-,+)
			v3( center.x -dimenHalf.x, center.y-dimenHalf.y, center.z -dimenHalf.z ), // 4 (-,-,-)
			v3( center.x -dimenHalf.x, center.y+dimenHalf.y, center.z -dimenHalf.z ), // 5 (-,+,-)
			v3( center.x +dimenHalf.x, center.y+dimenHalf.y, center.z -dimenHalf.z ), // 6 (+,+,-)
			v3( center.x +dimenHalf.x, center.y-dimenHalf.y, center.z -dimenHalf.z ), // 7 (+,-,-)
		};
		
		PolygonBegin( PRIM_TRIANGLES );

		// z-pos
		Vertex( pos[0], uv[0], normals[0] );
		Vertex( pos[3], uv[1], normals[0] );
		Vertex( pos[2], uv[2], normals[0] );
		Vertex( pos[0], uv[0], normals[0] );
		Vertex( pos[2], uv[2], normals[0] );
		Vertex( pos[1], uv[3], normals[0] );
		
		// z-neg
		Vertex( pos[7], uv[0], normals[1] );
		Vertex( pos[4], uv[1], normals[1] );
		Vertex( pos[5], uv[2], normals[1] );
		Vertex( pos[7], uv[0], normals[1] );
		Vertex( pos[5], uv[2], normals[1] );
		Vertex( pos[6], uv[3], normals[1] );
		
		// x-neg
		Vertex( pos[4], uv[0], normals[2] );
		Vertex( pos[0], uv[1], normals[2] );
		Vertex( pos[1], uv[2], normals[2] );
		Vertex( pos[4], uv[0], normals[2] );
		Vertex( pos[1], uv[2], normals[2] );
		Vertex( pos[5], uv[3], normals[2] );
		
		// x-pos
		Vertex( pos[3], uv[0], normals[3] );
		Vertex( pos[7], uv[1], normals[3] );
		Vertex( pos[6], uv[2], normals[3] );
		Vertex( pos[3], uv[0], normals[3] );
		Vertex( pos[6], uv[2], normals[3] );
		Vertex( pos[2], uv[3], normals[3] );
		
		// y-neg
		Vertex( pos[7], uv[0], normals[4] );
		Vertex( pos[3], uv[1], normals[4] );
		Vertex( pos[0], uv[2], normals[4] );
		Vertex( pos[7], uv[0], normals[4] );
		Vertex( pos[0], uv[2], normals[4] );
		Vertex( pos[4], uv[3], normals[4] );
		
		// y-pos
		Vertex( pos[2], uv[0], normals[5] );
		Vertex( pos[6], uv[1], normals[5] );
		Vertex( pos[5], uv[2], normals[5] );
		Vertex( pos[2], uv[0], normals[5] );
		Vertex( pos[5], uv[2], normals[5] );
		Vertex( pos[1], uv[3], normals[5] );

		PolygonEnd();
	}
	
	void VertexBatcher::DrawCubeWire( const v3 dimen, const v3 center )
	{
		PolygonBegin( PRIM_LINES );
		v3 dimenHalf = dimen * 0.5f;
		
		// z-pos
		Vertex( center.x -dimenHalf.x, center.y -dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y +dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y +dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y +dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y +dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y -dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y -dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y -dimenHalf.y, center.z +dimenHalf.z );
		
		// z-neg
		Vertex( center.x -dimenHalf.x, center.y -dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y +dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y +dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y +dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y +dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y -dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y -dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y -dimenHalf.y, center.z -dimenHalf.z );
		
		// sides
		Vertex( center.x -dimenHalf.x, center.y -dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y -dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y +dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x -dimenHalf.x, center.y +dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y +dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y +dimenHalf.y, center.z +dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y -dimenHalf.y, center.z -dimenHalf.z );
		Vertex( center.x +dimenHalf.x, center.y -dimenHalf.y, center.z +dimenHalf.z );
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawMayaPlane( int gridsWidth )
	{
		PolygonBegin( PRIM_LINES );
		
		float start = -gridsWidth * 0.5f;
		
		for( int x = 0; x < gridsWidth; ++x )
		{
			for( int z = 0; z < gridsWidth; ++z )
			{
				Vertex( start+x, 0, start+z );
				Vertex( start+x+1, 0, start+z );
				Vertex( start+x, 0, start+z );
				Vertex( start+x, 0, start+z+1 );
			}
		}
		
		Vertex( -start, 0, start );
		Vertex( -start, 0, -start );
		
		Vertex( start, 0, -start );
		Vertex( -start, 0, -start );
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawSpline( const v2 v0, const v2 tan0, const v2 v1, const v2 tan1, const float t0 /*= 0.0f*/, const float t1 /*= 1.0f */, int numSteps )
	{
		const float deltaT = (t1-t0)/(numSteps-1);
		
		PolygonBegin( PRIM_LINE_STRIP );
		
		for( int i = 0; i < numSteps; ++i )
		{
			const float t = t0 + deltaT*i;
			v2 pos = hermite(v0, tan0, v1, tan1, t );
			
			Vertex( pos.x, pos.y );
		}
		PolygonEnd();
	}
	
	void VertexBatcher::DrawSpline( const v3 v0, const v3 tan0, const v3 v1, const v3 tan1, const float t0 /*= 0.0f*/, const float t1 /*= 1.0f*/, int numSteps /*= 64 */ )
	{
		const float deltaT = (t1-t0)/(numSteps-1);
		
		PolygonBegin( PRIM_LINE_STRIP );
		
		for( int i = 0; i < numSteps; ++i )
		{
			const float t = t0 + deltaT*i;
			v3 pos = hermite(v0, tan0, v1, tan1, t );
			
			Vertex( pos.x, pos.y, pos.z );
		}
		PolygonEnd();
	}
	
	void VertexBatcher::DrawBasisVectors()
	{
		// Z
		Color( v3(1.0f,0.0f,0.0f) );
		DrawArrow( v3(0.0f), v3(1.0f,0.0f,0.0f));
		
		// Y
		Color( v3(0.0f,1.0f,0.0f) );
		DrawArrow( v3(0.0f), v3(0.0f,1.0f,0.0f));
		
		// Z
		Color( v3(0.0f,0.0f,1.0f) );
		DrawArrow( v3(0.0f), v3(0.0f,0.0f,1.0f));
	}
	
	void VertexBatcher::DrawArrow( const v3 start, const v3 end )
	{
		const float len = length(start-end);

		// ignore if len too low
		if ( len < 0.001f ) return;

		//const float coeff = 0.2f;
		const float hatLength = len <1.0f ? len*0.2f : 0.2f;
		const float stemThickness = 0.025f;
		const float hatRadius = 0.1f;
		
		v3 dir = (end-start)/len;
		v3 dirOpp = -dir;
		v3 dirPerp0 = normalize(perpendicular(dirOpp));
		v3 dirPerp1 = normalize(cross( dirOpp, dirPerp0 ));
		v3 baseHat = end + hatLength * dirOpp;
		
		static const int numPts = 8;
		float delta_angle = PI * 2.0f / (numPts-1);
		
		v3 circleDir[numPts];
		for ( int i = 0; i < numPts; ++i )
		{
			v2 dirCircle = unitCircle( -delta_angle*i );
			circleDir[i] = dirCircle.x * dirPerp0 + dirCircle.y * dirPerp1;
		}
		
		// stem
		PolygonBegin( PRIM_TRIANGLES );
		for ( int i = 0; i < numPts-1; ++i )
		{
			v3 pt0 = start + stemThickness*circleDir[i];
			v3 pt1 = start + stemThickness*circleDir[i+1];
			v3 pt2 = baseHat + stemThickness*circleDir[i+1];
			v3 pt3 = baseHat + stemThickness*circleDir[i];
			
			Vertex(pt0.x, pt0.y, pt0.z);
			Vertex(pt1.x, pt1.y, pt1.z);
			Vertex(pt2.x, pt2.y, pt2.z);
			
			Vertex(pt0.x, pt0.y, pt0.z);
			Vertex(pt2.x, pt2.y, pt2.z);
			Vertex(pt3.x, pt3.y, pt3.z);
		}
		PolygonEnd();
		
		// hat
		PolygonBegin( PRIM_TRIANGLE_FAN );
		Vertex( end.x, end.y, end.z );
		for ( int i = 0; i < numPts; ++i )
		{
			v3 pt = baseHat + hatRadius*circleDir[i];
			Vertex(pt.x,pt.y,pt.z);
		}
		PolygonEnd();
		
		// bottom of hat
		PolygonBegin( PRIM_TRIANGLE_FAN );
		Vertex( baseHat.x, baseHat.y, baseHat.z );
		for ( int i = numPts-1; i >= 0; --i )
		{
			v3 pt = baseHat + hatRadius*circleDir[i];
			Vertex(pt.x,pt.y,pt.z);
		}
		PolygonEnd();
	}
	
	void VertexBatcher::DrawDisk( const v3 center, const v3 dir, float radius, int numSteps )
	{
		const v3 up_vec = normalize(dir);
		const v3 right_vec = normalize(perpendicular(up_vec)) * radius;
		const v3 z_vec = normalize(cross(right_vec,up_vec)) * radius;
		
		const float delta_angle = 2.0f*PI / (numSteps);
		
		PolygonBegin( PRIM_TRIANGLE_FAN );
		
		Vertex(center, v2(0.0f), up_vec );		
		for ( int i = 0; i <= numSteps; ++i )
		{
			float angle = delta_angle*i;
			float xDir = cosf(angle);
			float yDir = sinf(angle);
			
			v3 pos = center + xDir*right_vec + yDir*z_vec;
			Vertex(pos, v2(0.0f), up_vec );
		}
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawDiskWire( const v3 center, const v3 dir, float radius, int numSteps )
	{
		const v3 up_vec = normalize(dir);
		const v3 right_vec = normalize(perpendicular(up_vec)) * radius;
		const v3 z_vec = normalize(cross(right_vec,up_vec)) * radius;
		
		const float delta_angle = 2.0f*PI / numSteps;
		PolygonBegin( PRIM_LINE_LOOP );
		
		for ( int i = 0; i < numSteps; ++i )
		{
			float angle = delta_angle*i;
			float xDir = cosf(angle);
			float yDir = sinf(angle);
			
			v3 pos = center + xDir*right_vec + yDir*z_vec;
			Vertex(pos, v2(0.0f), up_vec );
		}
		
		PolygonEnd();
	}

	void VertexBatcher::DrawDiskWireThick( const v3 center, const v3 dir, float radius, float thickness, int numSteps )
	{
		const v3 up_vec = normalize(dir);
		const v3 right_vec = normalize(perpendicular(up_vec));
		const v3 z_vec = normalize(cross(right_vec,up_vec));
		
		const float delta_angle = 2.0f*PI / numSteps;
		const float radius_0 = radius - thickness*0.5f;
		const float radius_1 = radius + thickness*0.5f;

		PolygonBegin( PRIM_TRIANGLES );		
		
		for ( int i = 0; i < numSteps; ++i )
		{
			float angle_0 = delta_angle*i;
			float xDir_0 = cosf(angle_0);
			float yDir_0 = sinf(angle_0);

			float angle_1 = delta_angle*(i+1.0f);
			float xDir_1 = cosf(angle_1);
			float yDir_1 = sinf(angle_1);
			
			v3 pos_0 = center + xDir_0*right_vec*radius_0 + yDir_0*z_vec*radius_0;
			v3 pos_1 = center + xDir_0*right_vec*radius_1 + yDir_0*z_vec*radius_1;
			v3 pos_2 = center + xDir_1*right_vec*radius_1 + yDir_1*z_vec*radius_1;
			v3 pos_3 = center + xDir_1*right_vec*radius_0 + yDir_1*z_vec*radius_0;

			Vertex(pos_0, v2(0.0f), up_vec );
			Vertex(pos_1, v2(0.0f), up_vec );
			Vertex(pos_2, v2(0.0f), up_vec );
			Vertex(pos_0, v2(0.0f), up_vec );
			Vertex(pos_2, v2(0.0f), up_vec );
			Vertex(pos_3, v2(0.0f), up_vec );
		}
		
		PolygonEnd();
	}

	void VertexBatcher::DrawRibbon( const v3 start, const v3 end, const v3 right, float thickness,int numSteps )
	{
		const v3 straight_vec = end-start;
		const float len = length(straight_vec);
		if ( len < 0.001f ||  thickness < 0.001f || numSteps == 0) return;

		const float num_step_inv = 1.0f / numSteps;
		const float delta_step = len*num_step_inv;
		const v3 straight_vec_normalized  = straight_vec/len;
		const v3 right_vec = right;
		const v3 normal = normalize( cross(straight_vec, right) );

		PolygonBegin( PRIM_TRIANGLES );		
		
		for ( int i = 0; i < numSteps; ++i )
		{
			v3 pos_middle_0 = start + straight_vec_normalized*delta_step*(i+0.0f);
			v3 pos_middle_1 = start + straight_vec_normalized*delta_step*(i+1.0f);

			v3 pos_0 = pos_middle_0 - right*0.5f*thickness;
			v3 pos_1 = pos_middle_0 + right*0.5f*thickness;
			v3 pos_2 = pos_middle_1 - right*0.5f*thickness;
			v3 pos_3 = pos_middle_1 + right*0.5f*thickness;

			v2 uv0 = v2(0.0f, num_step_inv*i);
			v2 uv1 = v2(1.0f, num_step_inv*i);
			v2 uv2 = v2(0.0f, num_step_inv*(i+1.0f));
			v2 uv3 = v2(1.0f, num_step_inv*(i+1.0f));

			Vertex( pos_0, uv0, normal );
			Vertex( pos_1, uv1, normal ); 
			Vertex( pos_3, uv3, normal ); 
			Vertex( pos_0, uv0, normal ); 
			Vertex( pos_3, uv3, normal ); 
			Vertex( pos_2, uv2, normal ); 
		}
		
		PolygonEnd();
	}
	
	void VertexBatcher::DrawSphere( const v3 center, float radius )
	{
		// TODO!!
	}
}