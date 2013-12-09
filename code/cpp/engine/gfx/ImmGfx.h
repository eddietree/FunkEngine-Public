
#pragma once

#include <common/Singleton.h>

#include "GLTypes.h"
#include "Vbo.h"
#include "ShaderProgram.h"
#include "VertexBatcher.h"

namespace funk
{	
	class ImmGfx : public VertexBatcher, public Singleton<ImmGfx>
	{
	private:
		
		StrongHandle<Vbo> m_vbo;
		int	m_vbo_index;

        // default white texture
        StrongHandle<Texture2d> m_texture_default;
        StrongHandle<Texture2d> m_texture_current;
		
		// used to populate and then send to GPU
		bool					m_is_building_shape;
		GLPrimtive				m_draw_primitive;
		
		// default shader used to render solid colored objs
		StrongHandle<ShaderProgram> m_shader_default;
		
		// default shader used to render textured objs
		StrongHandle<ShaderProgram> m_shader_default_tex;

	public:
		
		// shader - solid colors
		StrongHandle<ShaderProgram> GetDefaultShader() { return m_shader_default; }
		void BeginDefaultShader();
		void EndDefaultShader();
		void SetDefaultShaderMVP();
		void SetDefaultShaderPointSize( float a_pt_size );

		// shader - textured
		StrongHandle<ShaderProgram> GetDefaultTexShader() { return m_shader_default_tex; }
		void BeginDefaultTexShader(); // begins and binds texture
		void EndDefaultTexShader();
		void SetDefaultTexShaderMVP();
		
		// begin/end the VBO for immediate rendering
		void BeginDraw();
		void EndDraw();
		bool IsDrawing() const { return m_vbo->IsBound(); }

		// begin/end the shape
		void PolygonBegin( GLPrimtive a_primitive );
		void PolygonEnd();
		
        // Texture state
		void Texture( StrongHandle<Texture2d> a_tex );
		
	private:
	
		void InitVbo();
		
		friend Singleton<ImmGfx>;
		ImmGfx();
		~ImmGfx();
	};
}
