#pragma once

#include <common/Singleton.h>
#include "GLTypes.h"

#include <unordered_map>
#include <bitset>
#include <stdint.h>

namespace funk
{    
    class RenderState : public Singleton<RenderState>
	{
	private:

		// used for glEnable/glDisable
		std::unordered_map<GLStateFlag, bool, std::hash<int>> m_flags;

		float m_line_width;
		float m_clear_color[4];
		float m_clear_depth;
		float m_polygon_offset[2]; // factor,units
		int m_active_texture;
		int m_cullface_mode;
		int m_frontface_mode; // GL_CW/GL_CCW
		int m_viewport[4]; // x,y,width,height
		int m_scissor[4]; // x,y,width,height
		bool m_depth_mask; // write to depth buffer?

		// blend
		int32_t m_blend_eq;
		int32_t m_blend_src;
		int32_t m_blend_dst;

		// shader
		uint32_t m_shader_program_id;

		// handles calls to glEnableVertexAttribArray/glDisableVertexAttribArray
		std::bitset<8> m_enabled_vertex_attrib_array;

	public: 

		void SetLineWidth( float a_line_width );
		void SetActiveTexture( int a_tex_unit );
		void SetViewport( int a_x, int a_y, int a_width, int a_height );
		void SetScissor( int a_x, int a_y, int a_width, int a_height );
		void SetClearColor( float a_red, float a_green, float a_blue, float a_alpha );
		void SetClearDepth( float a_depth );
		void SetPolygonOffset( float a_factor, float a_units );
		void SetCullFaceMode( int a_mode );
		void SetFrontFace( int a_front_face );
		void SetDepthMask( bool a_enabled );

		// glEnable/glDisable
		void EnableFlag( GLStateFlag a_flag );
		void DisableFlag( GLStateFlag a_flag );

		// blending
		void SetBlendEq( int a_eq );
		void SetBlendFunc( int a_src, int a_dst );

		void EnableVertexAttribArray( int a_slot );
		void DisableVertexAttribArray( int a_slot );

		// shader
		void SetShaderProgram( uint32_t a_id );
		void ClearShaderProgram();
		uint32_t GetShaderProgramId() const { return m_shader_program_id; }

    private:
        friend Singleton<RenderState>;
		RenderState();
		~RenderState();
		
		void AddFlags();
		bool SetFlag( GLStateFlag a_flag, bool a_val ); // returns true if changed
		bool SetEnableVertexAttribArray( int a_slot, bool a_val ); // returns true if changed
    };
}
