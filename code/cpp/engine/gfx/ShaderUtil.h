#pragma once

#include <stdint.h>

namespace funk
{	
	// returns GLuint of the shader
	uint32_t CompileShaderGL( const char * a_filename, int a_gl_shader_type, bool a_retry_until_success = true );
}
