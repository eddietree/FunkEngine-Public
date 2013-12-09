#include "ShaderManager.h"

#include <common/Debug.h>
#include <common/Util.h>
#include <imgui/Imgui.h>

namespace funk
{
	ShaderManager::ShaderManager() 
	{
		LOG("ShaderManager initialized");
	}
	
	ShaderManager::~ShaderManager()
	{
		// before it closes, all the shaders must have already been cleaned up
		ASSERT( m_map_programs.empty() );
		ASSERT( m_map_vs.empty() );
		ASSERT( m_map_fs.empty() );
	}

	StrongHandle<ShaderProgram> ShaderManager::GetProgram( const char* a_vp_filename, const char* a_fp_filename )
	{
		uint32_t hash_vp = HashString(a_vp_filename);
		uint32_t hash_fp = HashString(a_fp_filename);
		uint64_t hash_combined = HashUint32Pair(hash_vp, hash_fp);
		
		auto it_prog = m_map_programs.find(hash_combined);
		
		// Shader Program found!
		if ( it_prog != m_map_programs.end() )
		{
			return it_prog->second->GetHandle();
		}
		
		StrongHandle<VertexShader> vp;
		StrongHandle<FragShader> fp;
		
		// vertex shader
		auto it_vp = m_map_vs.find(hash_vp);
		if ( it_vp != m_map_vs.end() )
		{
			vp = it_vp->second->GetHandle();
		}
		else
		{
			// creat new!
			vp = new VertexShader();
			vp->Init(a_vp_filename);
		}
		
		// frag shader
		auto it_fp = m_map_fs.find(hash_fp);
		if ( it_fp != m_map_fs.end() )
		{
			fp = it_fp->second->GetHandle();
		}
		else
		{
			// creat new!
			fp = new FragShader();
			fp->Init(a_fp_filename);
		}
		
		ASSERT( vp != nullptr );
		ASSERT( fp != nullptr );
		
		// build new shader
		StrongHandle<ShaderProgram> shader_program = new ShaderProgram;
		shader_program->Init(vp, fp);
		
		return shader_program;
	}
	
	void ShaderManager::RegisterVertexShader( VertexShader* a_vp, const char* a_file )
	{
		ASSERT( a_vp );
		ASSERT( a_file );
		
		uint32_t hash = HashString(a_file);
		ASSERT( m_map_vs.find(hash) == m_map_vs.end() );
		m_map_vs.insert( std::make_pair(hash,a_vp) );
	}
	
	void ShaderManager::RegisterFragShader( FragShader* a_fp, const char* a_file )
	{
		ASSERT( a_fp );
		ASSERT( a_file );
		
		uint32_t hash = HashString(a_file);
		ASSERT( m_map_fs.find(hash) == m_map_fs.end() );
		m_map_fs.insert( std::make_pair(hash,a_fp) );
	}
	
	void ShaderManager::RegisterShaderProgram( ShaderProgram* a_shader, const char* a_vp, const char* a_fp )
	{
		ASSERT( a_shader );
		ASSERT( a_vp );
		ASSERT( a_fp );
		
		uint64_t hash = HashStringPair(a_vp, a_fp);
		ASSERT( m_map_programs.find(hash) == m_map_programs.end() );
		m_map_programs.insert( std::make_pair(hash,a_shader) );
	}
	
	void ShaderManager::ReleaseVertexShader( const char* a_file )
	{
		ASSERT( a_file );
		uint32_t hash = HashString(a_file);
		
		auto it = m_map_vs.find(hash);
		ASSERT( it != m_map_vs.end() );
		m_map_vs.erase(it);
	}
	
	void ShaderManager::ReleaseFragShader( const char* a_file )
	{
		ASSERT( a_file );
		uint32_t hash = HashString(a_file);
		
		auto it = m_map_fs.find(hash);
		ASSERT( it != m_map_fs.end() );
		m_map_fs.erase(it);
	}
	
	void ShaderManager::ReleaseShaderProgram( const char* a_vp, const char* a_fp )
	{
		ASSERT( a_vp );
		ASSERT( a_fp );
		
		uint64_t hash = HashStringPair(a_vp, a_fp);
		auto it = m_map_programs.find(hash);
		ASSERT( it != m_map_programs.end() );
		m_map_programs.erase(it);
	}
	
	void ShaderManager::ReloadAll()
	{
		// first, need to detach shaders from shader-program
		for( auto it = m_map_programs.begin(); it != m_map_programs.end(); ++it )
		{
			it->second->DetachShaders();
		}
		
		// reload all VP
		for( auto it = m_map_vs.begin(); it != m_map_vs.end(); ++it )
		{
			it->second->Reload();
		}
		
		// reload all FP
		for( auto it = m_map_fs.begin(); it != m_map_fs.end(); ++it )
		{
			it->second->Reload();
		}
		
		// relink all shaders
		for( auto it = m_map_programs.begin(); it != m_map_programs.end(); ++it )
		{
			it->second->LinkShaders();
		}
		
		TRACE("Reloaded %d vs, %d fs and %d shader-programs!\n",
			  (int)m_map_vs.size(),
			  (int)m_map_fs.size(),
			  (int)m_map_programs.size()
		);
	}
	
	void ShaderManager::Gui()
	{
		static bool show_gui = false;
		
		Imgui::Separator();
		Imgui::CheckBox("Show Shaders", show_gui );
		
		if ( show_gui )
		{
			Imgui::Header("Shaders");
			Imgui::FillBarInt("Shader Programs", m_map_programs.size(), 0 , 16 );
			Imgui::FillBarInt("Vertex Shaders", m_map_vs.size(), 0 , 16 );
			Imgui::FillBarInt("Frag Shaders", m_map_fs.size(), 0 , 16 );
			if ( Imgui::Button("Reload Shaders (F6)") ) ReloadAll();
		}
	}
}