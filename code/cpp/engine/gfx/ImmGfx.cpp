#include "ImmGfx.h"

#include "ShaderManager.h"
#include "GLDebug.h"
#include <common/Debug.h>

#include <stdint.h>

// maximum number of vertices in VBO and scratch space
const int IMMGFX_MAX_VERTS = 8192;
const int IMMGFX_TEX_BINDSLOT = 0;

namespace funk
{
	ImmGfx::ImmGfx() : VertexBatcher(IMMGFX_MAX_VERTS), m_is_building_shape(false)
	{
		LOG("Initializing ImmGfx...");

		LOG("\tImmGfx - InitVbO()");
		InitVbo();
		
		// default shader
		LOG("\tImmGfx - Initializing shaders");
		m_shader_default = ShaderManager::Ref().GetProgram( "common/shaders/immGfxDefault.vp", "common/shaders/immGfxDefault.fp" );
		m_shader_default_tex = ShaderManager::Ref().GetProgram( "common/shaders/immGfxDefaultTex.vp", "common/shaders/immGfxDefaultTex.fp" );

        // default texture
		LOG("\tImmGfx - Initializing default texture");
        const unsigned char white_data[4] = { 255, 255, 255, 255 };
        m_texture_default = StrongHandle<Texture2d>( new Texture2d() );
        m_texture_default->Init(Texture2dDesc());
        m_texture_default->Bind();
        m_texture_default->SubData((void*)white_data, 1, 1);
        m_texture_default->Unbind();

        m_texture_current = m_texture_default;

		LOG("ImmGfx initialized");
	}

	ImmGfx::~ImmGfx()
	{
	}
	
	void ImmGfx::InitVbo()
	{   
		m_vbo_index = 0;
		
		m_vbo = new Vbo();
		m_vbo->Init( nullptr, IMMGFX_MAX_VERTS, GetNumBytesPerVertex(), CreateVertexLayout(), BUFFER_STREAM );

		CheckGLError();
	}
	
	void ImmGfx::BeginDraw()
	{
		ASSERT(!m_vbo->IsBound());
		m_vbo->Begin();
	}
	
	void ImmGfx::EndDraw()
	{
		ASSERT(m_vbo->IsBound());
		m_vbo->End();
	}
	
	void ImmGfx::PolygonBegin( GLPrimtive a_primitive )
	{
		CHECK(!m_is_building_shape, "Forgot to PolygonEnd");

		m_is_building_shape = true;
		
		m_draw_primitive = a_primitive;

		GetVertexBatch().clear();
	}
	
	void ImmGfx::PolygonEnd()
	{
		CHECK(m_is_building_shape, "Forgot to PolygonBegin");
		m_is_building_shape = false;

		
		// reach around? (hehe)
		if ( m_vbo_index + GetVertexBatch().size() >= IMMGFX_MAX_VERTS )
		{
			m_vbo_index = 0;
		}
		
		// send gpu data and render
		if ( !GetVertexBatch().empty() )
		{
			// send data to GPU
			m_vbo->SubData( &GetVertexBatch().front(), (uint32_t)GetVertexBatch().size()*GetNumBytesPerVertex(), m_vbo_index*GetNumBytesPerVertex() );
			
			// draw primitive
			m_vbo->Draw( m_draw_primitive, m_vbo_index, (uint32_t)GetVertexBatch().size() );
			
			m_vbo_index += GetVertexBatch().size();
		}
	}

    void ImmGfx::Texture( StrongHandle<Texture2d> a_tex )
    {
		// avoid unbind/bind if doesn't change?
		if ( a_tex == m_texture_current ) return;
        
        m_texture_current->Unbind();

        if ( a_tex.IsNull() )
            m_texture_current = m_texture_default;
        else
            m_texture_current = a_tex;

		m_texture_current->Bind( IMMGFX_TEX_BINDSLOT );
    }

	void ImmGfx::BeginDefaultShader()
	{
		m_shader_default->Begin();
		m_shader_default->UniformMVP("u_mvp");
	}
	
	void ImmGfx::SetDefaultShaderMVP()
	{
		m_shader_default->UniformMVP("u_mvp");
	}

	void ImmGfx::SetDefaultShaderPointSize( float a_pt_size )
	{
		m_shader_default->Uniform("u_point_size", a_pt_size);
	}
	
	void ImmGfx::EndDefaultShader()
	{
		m_shader_default->End();
	}

	void ImmGfx::BeginDefaultTexShader()
	{
		m_shader_default_tex->Begin();
		m_shader_default_tex->UniformMVP("u_mvp");
		m_shader_default_tex->UniformTexBindIndex( "u_tex", IMMGFX_TEX_BINDSLOT );

		m_texture_current->Bind( IMMGFX_TEX_BINDSLOT );
	}
	
	void ImmGfx::SetDefaultTexShaderMVP()
	{
		m_shader_default_tex->UniformMVP("u_mvp");
	}
	
	void ImmGfx::EndDefaultTexShader()
	{
		// finish texture binding
		m_texture_current->Unbind();
		m_texture_current = m_texture_default;

		m_shader_default_tex->End();
	}
}