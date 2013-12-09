
#pragma once

#include "VertexShader.h"
#include "FragShader.h"
#include "Texture2d.h"

#include <common/HandledObj.h>
#include <math/v2.h>
#include <math/v3.h>
#include <math/v4.h>
#include <math/matrix.h>
#include <gm/gmBindHeader.h>

#include <unordered_map>

namespace funk
{
    class ShaderProgram : public HandledObj<ShaderProgram>
    {
		
	private:
		
		// the program id
		uint32_t m_program_id;

		// shaders
		StrongHandle<VertexShader>	m_vp;
		StrongHandle<FragShader>	m_fp;
		
		struct UniformData
		{
			int			gl_id;
			uint32_t	gl_type_enum;
		};
		
		// string-hash to uniform data
		std::unordered_map< uint32_t, UniformData > m_uniform_map;

    public:
		
		// Fetch shader programs using ShaderManager::GetShaderProgram(...)
		ShaderProgram();
		~ShaderProgram();
		void Init( const char * a_filename_vp, const char* a_filename_fp );
		void Init( StrongHandle<VertexShader> a_vp, StrongHandle<FragShader> a_fp );
		
		uint32_t Id() const { return m_program_id; }
		
		// returns -1 if not found
		int GetUniform( const char* a_name ) const;
	
		// set uniforms
		void Uniform( const char* a_name, float a_val0 );
		void Uniform( const char* a_name, float a_val0, float a_val1 );
		void Uniform( const char* a_name, float a_val0, float a_val1, float a_val2 );
		void Uniform( const char* a_name, float a_val0, float a_val1, float a_val2, float a_val3 );
		void Uniform( const char* a_name, int a_val0 );
		void Uniform( const char* a_name, int a_val0, int a_val1 );
		void Uniform( const char* a_name, int a_val0, int a_val1, int a_val2 );
		void Uniform( const char* a_name, int a_val0, int a_val1, int a_val2, int a_val3 );
		void Uniform( const char* a_name, v2 a_val );
		void Uniform( const char* a_name, v3 a_val );
		void Uniform( const char* a_name, v4 a_val );
		void Uniform( const char* a_name, const StrongHandle<Texture2d>& a_val );
		void Uniform( const char* a_name, matrix a_val );
		void UniformTexBindIndex( const char* a_name, int a_val0 );
		void UniformMatArr( const char* a_name, const matrix* a_mat_array, int count );
		void UniformMat2x2( const char* a_name, const float* a_mat );
		void UniformMat3x3( const char* a_name, const float* a_mat );
		void UniformMat4x4( const char* a_name, const float* a_mat );
		void UniformMVP( const char* a_name );
		
		void Begin();
		void End();
		
		GM_BIND_TYPEID(ShaderProgram);
		
    private:
		
		friend class ShaderManager;
		void DetachShaders();
		void LinkShaders();
		
		void FetchAttribs();
		void FetchUniforms();
    };
	
	GM_BIND_DECL(ShaderProgram);
    
}
