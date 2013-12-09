#include "TextureUtil.h"

#if defined(_WIN32)


#include "GLIncludes.h"
#include "GLTypes.h"
#include "GLDebug.h"
#include <common/Debug.h>

// DevIL
#include <il/il.h>
#include <il/ilu.h>

namespace funk
{	
	uint32_t LoadImage2d( const char* a_filename, Texture2dDesc & out_desc )
	{
		ILuint ih;
		ilGenImages(1, &ih);
		ilBindImage(ih);
		ilLoadImage(a_filename);
	
		ILuint errorid = ilGetError() ;
		CHECK( errorid == IL_NO_ERROR, "Texture '%s' load error: %s", a_filename, iluErrorString(errorid) ) ;

		out_desc.dimen.x = (int)ilGetInteger( IL_IMAGE_WIDTH );
		out_desc.dimen.y = (int)ilGetInteger( IL_IMAGE_HEIGHT );

		ASSERT(out_desc.dimen.x > 0);
		ASSERT(out_desc.dimen.y > 0);
	
		ILuint format = ilGetInteger ( IL_IMAGE_FORMAT );
		ILuint dataType = ilGetInteger ( IL_IMAGE_TYPE );

		// texture format
		switch (format)
		{
			case IL_RGB:
				out_desc.internal_format = TEXFORMAT_INTERNAL_RGB8;
				out_desc.format = TEXFORMAT_RGB;
				break;
			case IL_RGBA:
				out_desc.internal_format = TEXFORMAT_INTERNAL_RGBA8;
				out_desc.format = TEXFORMAT_RGBA;
				break;
			case IL_LUMINANCE:
				out_desc.internal_format = TEXFORMAT_INTERNAL_R8;
				out_desc.format = TEXFORMAT_RED;
				break;
			case IL_LUMINANCE_ALPHA:
				out_desc.internal_format = TEXFORMAT_INTERNAL_RG8;
				out_desc.format = TEXFORMAT_RG;
				break;
			default:
				ERROR_MSG("Unrecognized texture format!");
				break;
		}

		// texture data type
		switch (dataType)
		{
			case IL_BYTE:
				out_desc.data_type = DATATYPE_BYTE;
				break;
			case IL_UNSIGNED_BYTE:
				out_desc.data_type = DATATYPE_UNSIGNED_BYTE;
				break;
			case IL_SHORT:
				out_desc.data_type = DATATYPE_SHORT;
				break;
			case IL_UNSIGNED_SHORT:
				out_desc.data_type = DATATYPE_UNSIGNED_SHORT;
				break;
			case IL_INT:
				out_desc.data_type = DATATYPE_INT;
				break;
			case IL_UNSIGNED_INT:
				out_desc.data_type = DATATYPE_UNSIGNED_INT;
				break;
			case IL_FLOAT:
				out_desc.data_type = DATATYPE_FLOAT;
				break;
			case IL_HALF:
				out_desc.data_type = DATATYPE_HALF_FLOAT;
				break;
			default:
				ERROR_MSG("Unrecognized texture data type!");
				break;
		}

		uint32_t id = 0;
		glGenTextures( 1, &id );
		ASSERT( id != 0 );

		glBindTexture( GL_TEXTURE_2D, id );
		glTexImage2D(GL_TEXTURE_2D, 0, out_desc.format, out_desc.dimen.x, out_desc.dimen.y, 0, out_desc.format, out_desc.data_type, ilGetData() );
		glBindTexture( GL_TEXTURE_2D, 0 );

		ilDeleteImages(1, &ih);

		return id;
	}
}

#endif