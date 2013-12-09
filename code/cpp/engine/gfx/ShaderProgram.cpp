#include "ShaderProgram.h"

#include "GLIncludes.h"
#include "GLDebug.h"
#include "ShaderManager.h"
#include "RenderState.h"

#include <common/Debug.h>
#include <common/Util.h>
#include <system/FileUtil.h>

#include <math/MVP.h>
#include <gm/gmBind.h>

namespace funk
{
	ShaderProgram::ShaderProgram()
	: m_program_id(0)
	{
		m_program_id = glCreateProgram();
		ASSERT(m_program_id);
	}
	
	ShaderProgram::~ShaderProgram()
	{
		if ( RenderState::Ref().GetShaderProgramId() == m_program_id )
		{
			RenderState::Ref().ClearShaderProgram();
		}

		DetachShaders();		
		glDeleteProgram(m_program_id);
		
		// remove from registry
		if ( m_vp && m_fp )
		{
			ShaderManager::Ref().ReleaseShaderProgram(m_vp->GetFileName().c_str(), m_fp->GetFileName().c_str() );
		}
	}
	
	void ShaderProgram::Init( const char * a_filename_vp, const char* a_filename_fp )
	{
		ASSERT( a_filename_vp );
		ASSERT( a_filename_fp );
		
		m_vp = new VertexShader();
		m_vp->Init(a_filename_vp);
		m_fp = new FragShader();
		m_fp->Init(a_filename_fp);
		
		LinkShaders();
		
		ShaderManager::Ref().RegisterShaderProgram(this, a_filename_vp, a_filename_fp );
	}

	void ShaderProgram::Init( StrongHandle<VertexShader> a_vp, StrongHandle<FragShader> a_fp )
	{
		ASSERT( a_vp != nullptr );
		ASSERT( a_fp != nullptr );
		
		m_vp = a_vp;
		m_fp = a_fp;
		
		LinkShaders();
		
		ShaderManager::Ref().RegisterShaderProgram(this, a_vp->GetFileName().c_str(), a_fp->GetFileName().c_str() );
	}
	
	void ShaderProgram::DetachShaders()
	{
		if ( m_vp ) glDetachShader( m_program_id, m_vp->Id() );
		if ( m_fp ) glDetachShader( m_program_id, m_fp->Id() );
	}
	
	void ShaderProgram::LinkShaders()
	{
		ASSERT( m_vp != nullptr && m_vp->Id() );
		ASSERT( m_fp != nullptr && m_fp->Id() );
		
		// attach
		glAttachShader( m_program_id, m_vp->Id() );
		glAttachShader( m_program_id, m_fp->Id() );

		// link
		GLint status;
		glLinkProgram(m_program_id);
		//glValidateProgram(m_program_id);
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &status);
		ASSERT( status == GL_TRUE );		
		
		FetchUniforms();
		FetchAttribs();
		
		// TODO: Why do I have to link twice?
		glLinkProgram(m_program_id);
		
		CheckGLError();
	}
	
	void ShaderProgram::FetchAttribs()
	{
		GLint num_active_attribs = 0;
		glGetProgramiv(m_program_id, GL_ACTIVE_ATTRIBUTES, &num_active_attribs);
		
		// goes thru each attrib defined in the vertex shader
		// attributes should only be float1, float2, float3, float4
		// attributes should be of the form: "attrib_0", "attrib_1"
		
		for ( int i = 0; i < num_active_attribs; ++i )
		{
			char buffer[64];
			
			GLsizei		attrib_length = 0;
			GLint		attrib_size = 0;
			GLenum		attrib_type = 0;
			
			glGetActiveAttrib( m_program_id, (GLuint)i, (GLsizei)sizeof(buffer), &attrib_length, &attrib_size, &attrib_type, buffer );
			
			// assure data correct
			ASSERT( attrib_length );
			
			// attrib types can only be float1-float4
			ASSERT( attrib_type == GL_FLOAT || attrib_type == GL_FLOAT_VEC2 || attrib_type == GL_FLOAT_VEC3 || attrib_type == GL_FLOAT_VEC4 );
			
			// grab number
			const char * attrib_title = "attrib_";
			const char * index_str = strstr( buffer, attrib_title);
			ASSERT( index_str );
			index_str += strlen(attrib_title);
			int attrib_index = atoi(index_str);
			
			// bind the attribute
			glBindAttribLocation( m_program_id, attrib_index, buffer );
		}
		
		CheckGLError();
	}
	
	int ShaderProgram::GetUniform(const char *a_name ) const
	{
		uint32_t hashed_str = HashString(a_name);
		
		// find the uniform
		auto it = m_uniform_map.find( hashed_str );
		if ( it == m_uniform_map.end() ) return -1;
		
		return it->second.gl_id;
	}
	
	void ShaderProgram::FetchUniforms()
	{
		GLint num_active_uniforms = 0;
		glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &num_active_uniforms);
		
		// uniforms
		for( int i = 0; i < num_active_uniforms; ++i )
		{
			char buffer[64];
			
			GLsizei uniform_length = 0;
			GLint	uniform_size = 0;
			GLenum	uniform_type = 0;
			
			// grab uniform info
			glGetActiveUniform( m_program_id, (GLuint)i, (GLsizei)sizeof(buffer), &uniform_length, &uniform_size, &uniform_type, buffer );
			
			ASSERT( buffer );
			ASSERT( uniform_length );
			
			// get uniform location
			GLint uniform_id = glGetUniformLocation( m_program_id, buffer );
			ASSERT(uniform_id != -1);
			
			// add to map
			UniformData uniform_data;
			uniform_data.gl_id			= uniform_id;
			uniform_data.gl_type_enum	= uniform_type;
			
			uint32_t hashed_str = HashString(buffer);
			m_uniform_map.insert( std::make_pair(hashed_str,uniform_data) );
		}
		
		CheckGLError();
	}
	
#define CHECK_UNIFORM(name, type) \
	uint32_t hashed_str = HashString(name); \
	auto it = m_uniform_map.find( hashed_str ); \
	if ( it == m_uniform_map.end() ) {return; } \
	const UniformData & uniform = it->second; \
	ASSERT( uniform.gl_type_enum == type );
	
	
	void ShaderProgram::Uniform( const char* a_name, float a_val0 )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT );
		glUniform1f( uniform.gl_id, a_val0 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, float a_val0, float a_val1 )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_VEC2 );
		glUniform2f( uniform.gl_id, a_val0, a_val1 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, float a_val0, float a_val1, float a_val2 )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_VEC3 );
		glUniform3f( uniform.gl_id, a_val0, a_val1, a_val2 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, float a_val0, float a_val1, float a_val2, float a_val3 )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_VEC4 );
		glUniform4f( uniform.gl_id, a_val0, a_val1, a_val2, a_val3 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, int a_val0 )
	{
		CHECK_UNIFORM(a_name, GL_INT );
		glUniform1i( uniform.gl_id, a_val0 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, int a_val0, int a_val1 )
	{
		CHECK_UNIFORM(a_name, GL_INT_VEC2 );
		glUniform2i( uniform.gl_id, a_val0, a_val1 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, int a_val0, int a_val1, int a_val2 )
	{
		CHECK_UNIFORM(a_name, GL_INT_VEC3 );
		glUniform3i( uniform.gl_id, a_val0, a_val1, a_val2 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, int a_val0, int a_val1, int a_val2, int a_val3 )
	{
		CHECK_UNIFORM(a_name, GL_INT_VEC4 );
		glUniform4i( uniform.gl_id, a_val0, a_val1, a_val2, a_val3 );
	}
	
	void ShaderProgram::Uniform( const char* a_name, v2 a_val )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_VEC2 );
		glUniform2f( uniform.gl_id, a_val.x, a_val.y );
	}
	
	void ShaderProgram::Uniform( const char* a_name, v3 a_val )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_VEC3 );
		glUniform3f( uniform.gl_id, a_val.x, a_val.y, a_val.z );
	}
	
	void ShaderProgram::Uniform( const char* a_name, v4 a_val )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_VEC4 );
		glUniform4f( uniform.gl_id, a_val.x, a_val.y, a_val.z, a_val.w );
	}
	
	void ShaderProgram::Uniform( const char* a_name, const StrongHandle<Texture2d>& a_val )
	{
		CHECK_UNIFORM(a_name, GL_SAMPLER_2D );
		
		int bind_slot = a_val->GetBindSlot();
		CHECK(bind_slot >= 0, "Texture is not bound before setting uniform");
		glUniform1i( uniform.gl_id, bind_slot );
	}
	
	void ShaderProgram::Uniform( const char* a_name, matrix a_val )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_MAT4 );
		glUniformMatrix4fv( uniform.gl_id, (GLsizei)1, (GLboolean)0, a_val.GetFloatPtr() );
	}
	
	void ShaderProgram::UniformTexBindIndex( const char* a_name, int a_val0 )
	{
		CHECK_UNIFORM(a_name, GL_SAMPLER_2D );
		glUniform1i( uniform.gl_id, a_val0 );
	}

	void ShaderProgram::UniformMatArr( const char* a_name, const matrix* a_mat_array, int count )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_MAT4 );
		glUniformMatrix4fv( uniform.gl_id, (GLsizei)count, false, (float*)a_mat_array );
	}
	
	void ShaderProgram::UniformMat2x2( const char* a_name, const float* a_mat )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_MAT2 );
		glUniformMatrix2fv( uniform.gl_id, (GLsizei)1, (GLboolean)0, a_mat );
	}
	
	void ShaderProgram::UniformMat3x3( const char* a_name, const float* a_mat )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_MAT3 );
		glUniformMatrix3fv( uniform.gl_id, (GLsizei)1, (GLboolean)0, a_mat );
	}
	
	void ShaderProgram::UniformMat4x4( const char* a_name, const float* a_mat )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_MAT4 );
		glUniformMatrix4fv( uniform.gl_id, (GLsizei)1, (GLboolean)0, a_mat );
	}
	
	void ShaderProgram::UniformMVP( const char* a_name )
	{
		CHECK_UNIFORM(a_name, GL_FLOAT_MAT4 );
		
		matrix mvp = MVP::Ref().Get();
		glUniformMatrix4fv( uniform.gl_id, (GLsizei)1, (GLboolean)0, (float*)&mvp );
	}

#undef CHECK_UNIFORM
	
	void ShaderProgram::Begin()
	{
		RenderState::Ref().SetShaderProgram(m_program_id);
	}
	
	void ShaderProgram::End()
	{
	}
	
GM_REG_NAMESPACE(ShaderProgram)
{
	GM_MEMFUNC_DECL(CreateShaderProgram)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_vp_filename, 0 );
		GM_CHECK_STRING_PARAM( a_fp_filename, 1 );
		
		StrongHandle<ShaderProgram> prog = ShaderManager::Ref().GetProgram(a_vp_filename, a_fp_filename);
		GM_PUSH_USER_HANDLED( ShaderProgram, prog.Get() );
		return GM_OK;
	}
		
	GM_MEMFUNC_DECL(GetUniform)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		a_thread->PushInt( ptr->GetUniform(a_name) );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform1f)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_FLOAT_PARAM( a_val0, 1 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform2f)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_FLOAT_PARAM( a_val0, 1 );
		GM_CHECK_FLOAT_PARAM( a_val1, 2 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0, a_val1 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform3f)
	{
		GM_CHECK_NUM_PARAMS(4);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_FLOAT_PARAM( a_val0, 1 );
		GM_CHECK_FLOAT_PARAM( a_val1, 2 );
		GM_CHECK_FLOAT_PARAM( a_val2, 3 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0, a_val1, a_val2 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform4f)
	{
		GM_CHECK_NUM_PARAMS(5);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_FLOAT_PARAM( a_val0, 1 );
		GM_CHECK_FLOAT_PARAM( a_val1, 2 );
		GM_CHECK_FLOAT_PARAM( a_val2, 3 );
		GM_CHECK_FLOAT_PARAM( a_val3, 4 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0, a_val1, a_val2, a_val3 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform1i)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_INT_PARAM( a_val0, 1 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform2i)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_INT_PARAM( a_val0, 1 );
		GM_CHECK_INT_PARAM( a_val1, 2 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0, a_val1 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform3i)
	{
		GM_CHECK_NUM_PARAMS(4);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_INT_PARAM( a_val0, 1 );
		GM_CHECK_INT_PARAM( a_val1, 2 );
		GM_CHECK_INT_PARAM( a_val2, 3 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0, a_val1, a_val2 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Uniform4i)
	{
		GM_CHECK_NUM_PARAMS(5);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_INT_PARAM( a_val0, 1 );
		GM_CHECK_INT_PARAM( a_val1, 2 );
		GM_CHECK_INT_PARAM( a_val2, 3 );
		GM_CHECK_INT_PARAM( a_val3, 4 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0, a_val1, a_val2, a_val3 );
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(UniformV2)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_V2_PARAM( a_val0, 1 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(UniformV3)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_V3_PARAM( a_val0, 1 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_val0 );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(UniformTex2d)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_USER_PARAM_PTR( Texture2d, a_tex, 1 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, a_tex );
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(UniformTexBindIndex)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		GM_CHECK_INT_PARAM( a_index, 1 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->UniformTexBindIndex( a_name, a_index );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(UniformMVP)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->UniformMVP( a_name );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(UniformModelMatrix)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, MVP::Ref().Model().Get() );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(UniformViewMatrix)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, MVP::Ref().View().Get() );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(UniformProjMatrix)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0 );
		
		GM_GET_THIS_PTR(ShaderProgram, ptr);
		ptr->Uniform( a_name, MVP::Ref().Proj().Get() );
		
		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_VOID_VOID( ShaderProgram, Begin )
	GM_GEN_MEMFUNC_VOID_VOID( ShaderProgram, End )
	GM_GEN_MEMFUNC_INT_VOID( ShaderProgram, Id )
}

GM_REG_MEM_BEGIN(ShaderProgram)
GM_REG_MEMFUNC( ShaderProgram, Begin )
GM_REG_MEMFUNC( ShaderProgram, End )
GM_REG_MEMFUNC( ShaderProgram, Id )
GM_REG_MEMFUNC( ShaderProgram, GetUniform )
GM_REG_MEMFUNC( ShaderProgram, Uniform1f )
GM_REG_MEMFUNC( ShaderProgram, Uniform2f )
GM_REG_MEMFUNC( ShaderProgram, Uniform3f )
GM_REG_MEMFUNC( ShaderProgram, Uniform4f )
GM_REG_MEMFUNC( ShaderProgram, Uniform1i )
GM_REG_MEMFUNC( ShaderProgram, Uniform2i )
GM_REG_MEMFUNC( ShaderProgram, Uniform3i )
GM_REG_MEMFUNC( ShaderProgram, Uniform4i )
GM_REG_MEMFUNC( ShaderProgram, UniformV2 )
GM_REG_MEMFUNC( ShaderProgram, UniformV3 )
GM_REG_MEMFUNC( ShaderProgram, UniformTex2d )
GM_REG_MEMFUNC( ShaderProgram, UniformTexBindIndex )
GM_REG_MEMFUNC( ShaderProgram, UniformMVP )
GM_REG_MEMFUNC( ShaderProgram, UniformModelMatrix )
GM_REG_MEMFUNC( ShaderProgram, UniformViewMatrix )
GM_REG_MEMFUNC( ShaderProgram, UniformProjMatrix )
GM_REG_HANDLED_DESTRUCTORS(ShaderProgram)
GM_REG_MEM_END()
GM_BIND_DEFINE(ShaderProgram)

}