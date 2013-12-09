#include "ShaderUtil.h"

#include "GLIncludes.h"
#include "GLTypes.h"
#include "GLDebug.h"
#include <common/Debug.h>
#include <system/DataFile.h>
#include <system/FileModificationChecker.h>

#include <chrono>
#include <thread>

namespace funk
{	
	uint32_t CompileShaderGLHelper( const char * a_filename, int a_gl_shader_type )
	{
		ASSERT( a_filename );
		
		// grab file contents
		DataFile file_data;
        file_data.Load(a_filename);
		const char * source = (const char*)file_data.GetData();
		ASSERT( source );
		
	#if GL_ES
		const char * source_lines[] =
		{
			"#version 100\n",
			"precision highp float;",
			source,
		};
	#else
		// used in non-ES systems (MacOSX, PC)
		const char * source_lines[] =
		{
			"#version 120\n",
			"#define lowp\n",
			"#define mediump\n",
			"#define highp\n",
			source,
		};
	#endif
		
		int num_lines = sizeof(source_lines)/sizeof(source_lines[0]);

		GLuint shader_id = glCreateShader( a_gl_shader_type );
		glShaderSource(shader_id, num_lines, source_lines, nullptr);
		//glShaderSource(shader_id, 1, &source, NULL);
		glCompileShader(shader_id);
		
		// make sure compiled
		GLint status;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
		if ( status == GL_FALSE )
		{
			LOG( "Failed to compile shader '%s'", a_filename )

			GLint logLength;
			char buffer[512];
			glGetShaderInfoLog(shader_id, sizeof(buffer), &logLength, buffer);
			if ( logLength > 0 )
			{
				LOG("Shader compile '%s' log:\n%s", a_filename, buffer);
				glDeleteShader( shader_id );
			}

			return 0;
		}	
		
		return shader_id;
	}

	uint32_t CompileShaderGL( const char * a_filename, int a_gl_shader_type, bool a_retry_until_success )
	{
		uint32_t id = CompileShaderGLHelper(a_filename, a_gl_shader_type );

		// will loop forever until the shader builds
		#if !GL_ES && !defined(FUNK_FINAL)

		if ( a_retry_until_success )
		{
			while( id == 0 )
			{
				FileModificationChecker checker = FileModificationChecker(a_filename);

				while (checker.IsModified() == false)
				{
					// wait just a tad
					const int num_sleep_ms = 100;
					std::this_thread::sleep_for(std::chrono::milliseconds(num_sleep_ms));
				}

				checker.Sync();

				id = CompileShaderGLHelper(a_filename, a_gl_shader_type );
			}
		}
		#else
		ASSERT(id != 0);
		#endif // GL_ES

		return id;
	}
}