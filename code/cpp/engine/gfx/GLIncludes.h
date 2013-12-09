
#pragma once

#include <system/Config.h>

#if GL_ES

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

// If OpenGL ES. remap so same as vanilla
#define GL_MAP_READ_BIT					GL_MAP_READ_BIT_EXT
#define GL_MAP_WRITE_BIT				GL_MAP_WRITE_BIT_EXT
#define GL_MAP_UNSYNCHRONIZED_BIT		GL_MAP_UNSYNCHRONIZED_BIT_EXT
#define GL_MAP_INVALIDATE_RANGE_BIT		GL_MAP_INVALIDATE_RANGE_BIT_EXT
#define GL_UNSIGNED_INT_24_8			GL_UNSIGNED_INT_24_8_OES
#define GL_HALF_FLOAT					GL_HALF_FLOAT_OES


#define glMapBufferRange		glMapBufferRangeEXT
#define glUnmapBuffer			glUnmapBufferOES
#define glGenVertexArrays		glGenVertexArraysOES
#define glBindVertexArray		glBindVertexArrayOES
#define glDeleteVertexArrays	glDeleteVertexArraysOES

#define glClearDepth		glClearDepthf

#else
	#if defined(__APPLE__)
		// Not OpenGL ES
		//#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
		//#include <OpenGL/gl.h>
		#include <OpenGL/gl3.h>
		//#include <OpenGL/gl3ext.h>
	#else
		// window sopengl
		#include <GL/glew.h>
	#endif
#endif // GL_ES
