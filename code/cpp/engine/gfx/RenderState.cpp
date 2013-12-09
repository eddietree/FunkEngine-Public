#include "RenderState.h"

#include <common/Debug.h>
#include "GLDebug.h"
#include "GLIncludes.h"

namespace funk
{
	RenderState::RenderState() 
	{
		AddFlags();

		m_line_width = 1.0f;
		m_active_texture = -1;
		m_shader_program_id = 0;

        CheckGLError();
        
		glGetIntegerv( GL_BLEND_EQUATION_RGB, &m_blend_eq );
		glGetIntegerv( GL_BLEND_SRC_RGB, &m_blend_src );
		glGetIntegerv( GL_BLEND_DST_RGB, &m_blend_dst );
		glGetIntegerv( GL_VIEWPORT, (GLint*)m_viewport );
		glGetIntegerv( GL_SCISSOR_BOX, (GLint*)m_scissor );
		glGetIntegerv( GL_CULL_FACE_MODE, (GLint*)&m_cullface_mode );
		glGetIntegerv( GL_FRONT_FACE, (GLint*)&m_frontface_mode );
		glGetFloatv( GL_COLOR_CLEAR_VALUE, (GLfloat*)m_clear_color );
		glGetFloatv( GL_POLYGON_OFFSET_FACTOR , (GLfloat*)&m_polygon_offset[0] );
		glGetFloatv( GL_POLYGON_OFFSET_UNITS , (GLfloat*)&m_polygon_offset[1] );
		glGetFloatv( GL_DEPTH_CLEAR_VALUE, (GLfloat*)&m_clear_depth );

        CheckGLError();
        
		// depth mask
		int depth_write_mask;
		glGetIntegerv( GL_DEPTH_WRITEMASK, (GLint*)&depth_write_mask );
		m_depth_mask = depth_write_mask == GL_TRUE;

		LOG("RenderState initialized");
	}
	
	RenderState::~RenderState()
	{}

	void RenderState::AddFlags()
	{
		GLStateFlag flags[] = 
		{
			FLAG_BLEND,
			FLAG_DEPTH_TEST,
			FLAG_SCISSOR_TEST,
			FLAG_TEXTURE_2D,
			FLAG_CULL_FACE,

			#if !GL_ES
			FLAG_LINE_SMOOTH,
			FLAG_PROGRAM_POINT_SIZE,
			#endif
		};

		// add the flags
		m_flags.reserve(sizeof(flags)/sizeof(flags[0]));
		for( auto flag : flags )
		{
			GLboolean enabled = false;
            
            // for some reason, it crashes only on GL_TEXTURE_2D!
            if ( flag != FLAG_TEXTURE_2D )
                glGetBooleanv( flag, &enabled );
            
			m_flags.insert( std::make_pair(flag, enabled!=GL_FALSE) );
		}
        
        CheckGLError();
	}

	void RenderState::EnableFlag( GLStateFlag a_flag )
	{
		if ( SetFlag(a_flag, true) )
		{
			glEnable(a_flag);
		}
	}

	void RenderState::DisableFlag( GLStateFlag a_flag )
	{
		if ( SetFlag(a_flag, false) )
		{
			glDisable(a_flag);
		}
	}

	bool RenderState::SetFlag( GLStateFlag a_flag, bool a_val )
	{
		auto it = m_flags.find(a_flag);
		//ASSERT(it != m_flags.end());
        if ( it == m_flags.end() ) return false;
        
		if ( it->second != a_val )
		{
			it->second = a_val;
			return true;
		}

		return false;
	}

	void RenderState::SetLineWidth( float a_line_width )
	{
		if ( m_line_width != a_line_width )
		{
			m_line_width = a_line_width;
			glLineWidth(m_line_width);
		}
	}

	void RenderState::SetActiveTexture( int a_tex_unit )
	{
		if ( m_active_texture != a_tex_unit )
		{
			m_active_texture = a_tex_unit;
			glActiveTexture(GL_TEXTURE0 + a_tex_unit );
		}
	}

	void RenderState::SetViewport( int a_x, int a_y, int a_width, int a_height )
	{
		if ( m_viewport[0] != a_x 
			|| m_viewport[1] != a_y
			|| m_viewport[2] != a_width
			|| m_viewport[3] != a_height )
		{
			m_viewport[0] = a_x;
			m_viewport[1] = a_y;
			m_viewport[2] = a_width;
			m_viewport[3] = a_height;

			glViewport( a_x, a_y, a_width, a_height );
		}
	}

	void RenderState::SetScissor( int a_x, int a_y, int a_width, int a_height )
	{
		if ( m_scissor[0] != a_x 
			|| m_scissor[1] != a_y
			|| m_scissor[2] != a_width
			|| m_scissor[3] != a_height )
		{
			m_scissor[0] = a_x;
			m_scissor[1] = a_y;
			m_scissor[2] = a_width;
			m_scissor[3] = a_height;

			glScissor( a_x, a_y, a_width, a_height );
		}
	}

	void RenderState::SetClearColor( float a_red, float a_green, float a_blue, float a_alpha )
	{
		if ( m_clear_color[0] != a_red
			|| m_clear_color[1] != a_green
			|| m_clear_color[2] != a_blue
			|| m_clear_color[3] != a_alpha  )
		{
			m_clear_color[0] = a_red;
			m_clear_color[1] = a_green;
			m_clear_color[2] = a_blue;
			m_clear_color[3] = a_alpha;

			glClearColor( a_red, a_green, a_blue, a_alpha );
		}
	}

	void RenderState::SetClearDepth( float a_depth )
	{
		if ( m_clear_depth != a_depth )
		{
			m_clear_depth = a_depth;
			glClearDepth( a_depth );
		}
	}

	void RenderState::SetPolygonOffset( float a_factor, float a_units )
	{
		if ( m_polygon_offset[0] != a_factor || m_polygon_offset[1] != a_units )
		{
			m_polygon_offset[0] = a_factor;
			m_polygon_offset[1] = a_units;
			glPolygonOffset( a_factor, a_units );
		}
	}

	void RenderState::SetCullFaceMode( int a_mode )
	{
		ASSERT(a_mode == GL_FRONT || a_mode == GL_BACK || a_mode == GL_FRONT_AND_BACK);

		if ( m_cullface_mode != a_mode )
		{
			m_cullface_mode = a_mode;
			glCullFace( a_mode );
		}
	}

	void RenderState::SetFrontFace( int a_front_face )
	{
		ASSERT(a_front_face == GL_CW || a_front_face == GL_CCW );

		if ( m_frontface_mode != a_front_face )
		{
			m_frontface_mode = a_front_face;
			glFrontFace( a_front_face );
		}
	}

	void RenderState::SetDepthMask( bool a_enabled )
	{
		if ( m_depth_mask != a_enabled )
		{
			m_depth_mask = a_enabled;
			glDepthMask( a_enabled );
		}
	}

	void RenderState::SetBlendEq( int a_eq )
	{
		ASSERT( a_eq >= GL_FUNC_ADD && a_eq <= GL_FUNC_REVERSE_SUBTRACT );
		
		if ( m_blend_eq != a_eq )
		{
			m_blend_eq = a_eq;
			glBlendEquation( a_eq );
		}
	}

	void RenderState::SetBlendFunc( int a_src, int a_dst )
	{
		ASSERT( a_src >= GL_ZERO && a_src <= GL_SRC_ALPHA_SATURATE
			&& a_dst >= GL_ZERO && a_dst <= GL_SRC_ALPHA_SATURATE );

		if ( m_blend_src != a_src || m_blend_dst != a_dst )
		{
			m_blend_src = a_src;
			m_blend_dst = a_dst;
			glBlendFunc( a_src, a_dst );
		}
	}

	void RenderState::SetShaderProgram( uint32_t a_id )
	{
		ASSERT( a_id != 0 );

		if ( m_shader_program_id != a_id )
		{
			m_shader_program_id = a_id;
			glUseProgram( m_shader_program_id );
		}
	}

	void RenderState::ClearShaderProgram()
	{
		m_shader_program_id = 0;
	}

	void RenderState::EnableVertexAttribArray( int a_slot )
	{
		if ( SetEnableVertexAttribArray( a_slot, true ) )
			glEnableVertexAttribArray(a_slot);
	}

	void RenderState::DisableVertexAttribArray( int a_slot )
	{
		if ( SetEnableVertexAttribArray( a_slot, false ) )
			glDisableVertexAttribArray(a_slot);
	}

	bool RenderState::SetEnableVertexAttribArray( int a_slot, bool a_val )
	{
		if ( m_enabled_vertex_attrib_array.test(a_slot) != a_val )
		{
			m_enabled_vertex_attrib_array.flip(a_slot);
			return true;
		}
		
		return false;
	}
}