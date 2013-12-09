#ifndef FUNK_FINAL

#include "GLDebug.h"
#include "GLIncludes.h"
#include <common/Debug.h>

void CheckGLError()
{
	GLenum err = glGetError();
	
	if ( err != GL_NO_ERROR )
	{
		switch(err)
		{
			#define GL_CASE_ERR( gl_code ) case gl_code : { ERROR_MSG("GL Error: %s", #gl_code ); LOG_ERROR("GL Error: %s", #gl_code ); break; }

			GL_CASE_ERR( GL_INVALID_ENUM );
			GL_CASE_ERR( GL_INVALID_VALUE );
			GL_CASE_ERR( GL_INVALID_OPERATION );
			GL_CASE_ERR( GL_INVALID_FRAMEBUFFER_OPERATION );
			GL_CASE_ERR( GL_OUT_OF_MEMORY );

			#undef GL_CASE_ERR
			
			default: ERROR_MSG("GL Error: unrecognized code: %d", err );
		}
		
		ERROR_MSG("GL Error occured");
	}
}

void CheckGLBufferBindingID(int target, int id)
{
	return;
    GLenum target_enum = (GLenum)target;

// TODO: is there a better way to ifdef this?
//       dont really want glGetIntegerv being called if the result is unused

    ASSERT( target_enum >= GL_ARRAY_BUFFER_BINDING );
    ASSERT( target_enum <= GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING );

    // we want to be able to verify the current buffer binding id is 0 too
    //CHECK( id != 0, "Invalid check on reserved id 0" );

    int bound_id = 0;

    glGetIntegerv( target_enum, &bound_id );

    CHECK( bound_id == id, "Incorrect buffer binding id" );
}

#endif // FUNK_FINAL