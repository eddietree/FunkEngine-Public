#include "TextureUtil.h"

#include "GLIncludes.h"
#include "GLTypes.h"
#include "GLDebug.h"
#include <common/Debug.h>
#include <system/FileUtil.h>

namespace funk
{	
	// helper - grabs texture properties
	void PopulateTexture2dDesc( const CGImageRef & cg_img, Texture2dDesc & out_desc )
	{
		// desc
		out_desc.dimen.x = (int)CGImageGetWidth(cg_img);
		out_desc.dimen.y = (int)CGImageGetHeight(cg_img);
		
		int bits_per_component = (int)CGImageGetBitsPerComponent(cg_img);
		switch( bits_per_component )
		{
			case 4:	out_desc.data_type = DATATYPE_UNSIGNED_SHORT_4_4_4_4; break;
			case 8:	out_desc.data_type = DATATYPE_UNSIGNED_BYTE; break;
			default: ERROR_MSG("Unsupported format!"); break;
		}
		
		// determine BPP and alpha
		CGImageAlphaInfo alpha_info		= CGImageGetAlphaInfo(cg_img);
		CGColorSpaceRef color_space		= CGImageGetColorSpace( cg_img );
		CGColorSpaceModel color_model	= CGColorSpaceGetModel( color_space );
		bool img_has_alpha = alpha_info != kCGImageAlphaNone;

		// texture format
		switch ( color_model )
		{
			case kCGColorSpaceModelMonochrome:
				out_desc.format = img_has_alpha ? TEXFORMAT_RG : TEXFORMAT_RED;
				break;
				
			case kCGColorSpaceModelRGB:
				out_desc.format = img_has_alpha ? TEXFORMAT_RGBA : TEXFORMAT_RGB;
				break;
				
			default:
				ERROR_MSG("Unsupported color space.");
				break;
		}
	}
	
	uint32_t LoadImage2d( const char* a_filename, Texture2dDesc & out_desc )
	{
		ASSERT(a_filename);
		
        const char * full_path_c_str = GetFullPath( a_filename );
        NSString * full_path = [NSString stringWithUTF8String:full_path_c_str];
		
	#if TARGET_OS_IPHONE
		// get image ref
		UIImage* ui_img = [UIImage imageWithContentsOfFile:full_path];
		ASSERT(ui_img);
		CGImageRef cg_img = ui_img.CGImage;
		ASSERT(cg_img);
	#else
		
		// MAC OSX
		// https://developer.apple.com/library/mac/#documentation/graphicsimaging/Conceptual/ImageIOGuide/imageio_source/ikpg_source.html
		NSURL *url = [NSURL fileURLWithPath:full_path];
		CFDictionaryRef   myOptions = NULL;
		CFStringRef       myKeys[2];
		CFTypeRef         myValues[2];
		myKeys[0]	= kCGImageSourceShouldCache;
		myValues[0] = (CFTypeRef)kCFBooleanTrue;
		myKeys[1]	= kCGImageSourceShouldAllowFloat;
		myValues[1] = (CFTypeRef)kCFBooleanTrue;
		myOptions	= CFDictionaryCreate(NULL, (const void **) myKeys,
									   (const void **) myValues, 2,
									   &kCFTypeDictionaryKeyCallBacks,
									   & kCFTypeDictionaryValueCallBacks);
								
		CGImageSourceRef ui_img = CGImageSourceCreateWithURL((CFURLRef)url, myOptions);
		ASSERT(ui_img);
		CGImageRef cg_img =  CGImageSourceCreateImageAtIndex(ui_img, 0, NULL);
		ASSERT(cg_img);
	#endif
		
		// grab texture data!
		PopulateTexture2dDesc( cg_img, out_desc );
		CFDataRef data_ref = CGDataProviderCopyData(CGImageGetDataProvider(cg_img));
		void* texture_data = (void*)CFDataGetBytePtr( data_ref );
		ASSERT( texture_data );
		
		// generate id 
		uint32_t gl_tex_id = 0;
		glGenTextures(1, &gl_tex_id);
		ASSERT( gl_tex_id );
		
		// send texture data to GPU
		glBindTexture( GL_TEXTURE_2D, gl_tex_id );
		glTexImage2D( GL_TEXTURE_2D, 0, out_desc.format, out_desc.dimen.x, out_desc.dimen.y, 0, out_desc.format, out_desc.data_type, texture_data );
		glBindTexture( GL_TEXTURE_2D, 0 );
		
		CheckGLError();
		
		// free the data
		CFRelease(data_ref);
		
		return gl_tex_id;
	}
}