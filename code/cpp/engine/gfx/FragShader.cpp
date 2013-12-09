#include "FragShader.h"
#include "ShaderUtil.h"
#include "GLIncludes.h"
#include "ShaderManager.h"

#include <common/Debug.h>

namespace funk
{
	FragShader::FragShader() : m_id(0)
	{
		
	}
	
	FragShader::~FragShader()
	{
		if ( m_id )	ShaderManager::Ref().ReleaseFragShader(m_filename.c_str());
		
		glDeleteShader(m_id);
	}
	
	void FragShader::Init(const char *a_filename)
	{
		ASSERT(a_filename);
		m_filename = a_filename;
		ShaderManager::Ref().RegisterFragShader(this, a_filename);
		
		m_id = CompileShaderGL(a_filename,GL_FRAGMENT_SHADER);
		ASSERT(m_id);
	}
	
	void FragShader::Reload()
	{
		ASSERT(m_id);
		glDeleteShader(m_id);
		
		m_id = CompileShaderGL(m_filename.c_str(), GL_FRAGMENT_SHADER);
		ASSERT(m_id);
	}
}