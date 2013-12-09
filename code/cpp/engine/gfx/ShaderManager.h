
#pragma once

#include <common/Singleton.h>

#include "VertexShader.h"
#include "FragShader.h"
#include "ShaderProgram.h"

#include <unordered_map>

namespace funk
{
    class ShaderManager : public Singleton<ShaderManager>
	{
	private:
	
		// map of hash(filename)->shader
		std::unordered_map<uint32_t, VertexShader*>		m_map_vs;
		std::unordered_map<uint32_t, FragShader*>		m_map_fs;
		std::unordered_map<uint64_t, ShaderProgram*>	m_map_programs;
		
    public:
	
		StrongHandle<ShaderProgram> GetProgram( const char* a_vp_filename, const char* a_fp_filename );
		
		// reloads all shaders
		void ReloadAll();
		
		void Gui();

    private:
        friend Singleton<ShaderManager>;
		ShaderManager();
		~ShaderManager();
		
		// shader programs
		friend ShaderProgram;
		void RegisterShaderProgram( ShaderProgram* a_shader, const char* a_vp, const char* a_fp );
		void ReleaseShaderProgram( const char* a_vp, const char* a_fp );
		
		// vertex shader
		friend VertexShader;
		void RegisterVertexShader( VertexShader* a_vp, const char* a_file );
		void ReleaseVertexShader( const char* a_file );

		// frag shader
		friend FragShader;
		void RegisterFragShader( FragShader* a_fp, const char* a_file );
		void ReleaseFragShader( const char* a_file );
    };
}
