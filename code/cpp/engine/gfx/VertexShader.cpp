#include "VertexShader.h"
#include "ShaderUtil.h"
#include "GLIncludes.h"
#include "ShaderManager.h"

#include <common/Debug.h>

namespace funk
{
	VertexShader::VertexShader() : m_id(0)
	{
	}
	
	VertexShader::~VertexShader()
	{
		if ( m_id )	ShaderManager::Ref().ReleaseVertexShader(m_filename.c_str());
	
		glDeleteShader(m_id);
	}
	
	void VertexShader::Init(const char *a_filename)
	{
		ASSERT(!m_id);
	
		ASSERT(a_filename);
		m_filename = a_filename;
		ShaderManager::Ref().RegisterVertexShader(this, a_filename);
		
		m_id = CompileShaderGL(a_filename,GL_VERTEX_SHADER);
		ASSERT(m_id);
	}
	
	void VertexShader::Reload()
	{
		ASSERT(m_id);
		glDeleteShader(m_id);
		
		m_id = CompileShaderGL(m_filename.c_str(), GL_VERTEX_SHADER);
		ASSERT(m_id);
	}
}