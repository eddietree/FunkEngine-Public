#include "gmExtLibGfx.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"
#include <gm/gmBind.h>

#include <common/Debug.h>
#include <system/Window.h>
#include <math/v2.h>
#include <gfx/ObjLoader.h>
#include <gfx/GLIncludes.h>
#include <gfx/GLDebug.h>
#include <gfx/Texture2d.h>
#include <gfx/RenderState.h>

namespace funk
{
struct gmfGfxLib
{	
	static int GM_CDECL gmfClearColor(gmThread * a_thread)
	{
		int numParams = a_thread->GetNumParams();
		
		if ( numParams == 1 )
		{
			GM_CHECK_V3_PARAM(color,0);
			RenderState::Ref().SetClearColor( color.x, color.y, color.z, 1.0f );
		}
		else if ( numParams == 2 )
		{
			GM_CHECK_V3_PARAM(color,0);
			GM_CHECK_FLOAT_OR_INT_PARAM( alpha, 1);
			RenderState::Ref().SetClearColor( color.x, color.y, color.z, alpha );
		}
		else if ( numParams == 3 )
		{
			GM_CHECK_FLOAT_OR_INT_PARAM( r, 0 );
			GM_CHECK_FLOAT_OR_INT_PARAM( g, 1 );
			GM_CHECK_FLOAT_OR_INT_PARAM( b, 2 );
			RenderState::Ref().SetClearColor( r, g, b, 1.0f );
		}
		else if ( numParams == 4 )
		{
			GM_CHECK_FLOAT_OR_INT_PARAM( r, 0 );
			GM_CHECK_FLOAT_OR_INT_PARAM( g, 1 );
			GM_CHECK_FLOAT_OR_INT_PARAM( b, 2 );
			GM_CHECK_FLOAT_OR_INT_PARAM( a, 3 );
			RenderState::Ref().SetClearColor( r, g, b, a );
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		return GM_OK;
	}
	
	static int GM_CDECL gmfClearDepth(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		
		GM_CHECK_FLOAT_OR_INT_PARAM( depth, 0 );
		RenderState::Ref().SetClearDepth( depth );
		
		return GM_OK;
	}
	
	static int GM_CDECL gmfClear(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		
		GM_CHECK_INT_PARAM( flags, 0 );
		glClear( flags );
		
		return GM_OK;
	}
    
    static int GM_CDECL gmfFlush(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(0);
		glFlush();
		return GM_OK;
	}
    
    static int GM_CDECL gmfFinish(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(0);
		glFinish();
		return GM_OK;
	}
	
	static int GM_CDECL gmfViewport(gmThread * a_thread)
	{
		int numParams = a_thread->GetNumParams();
		
		int x = 0;
		int y = 0;
		int width = 1;
		int height = 1;
		
		if ( numParams == 2 )
		{
			GM_CHECK_V2_PARAM( pos,	0 );
			GM_CHECK_V2_PARAM( dimen, 1 );
			
			x = (int)pos.x;
			y = (int)pos.y;
			width = (int)dimen.x;
			height = (int)dimen.y;
		}
		else if ( numParams == 4 )
		{
			GM_CHECK_FLOAT_OR_INT_PARAM( xPos, 0 );
			GM_CHECK_FLOAT_OR_INT_PARAM( yPos, 1 );
			GM_CHECK_FLOAT_OR_INT_PARAM( w, 2 );
			GM_CHECK_FLOAT_OR_INT_PARAM( h, 3 );
			
			x = (int)xPos;
			y = (int)yPos;
			width = (int)w;
			height = (int)h;
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		// make sure it is within the window
		const v2i windowDimen = Window::Get()->Sizei();

		ASSERT( x >= 0 );
		ASSERT( y >= 0 );
		ASSERT( (x + width) <= windowDimen.x );
		ASSERT( (y + height) <= windowDimen.y );
		
		RenderState::Ref().SetViewport( x, y, width, height );
		return GM_OK;
	}
	
	static int GM_CDECL gmfScissor(gmThread * a_thread)
	{
		int numParams = a_thread->GetNumParams();
		
		int x = 0;
		int y = 0;
		int width = 1;
		int height = 1;
		
		if ( numParams == 2 )
		{
			GM_CHECK_V2_PARAM( pos,	0 );
			GM_CHECK_V2_PARAM( dimen, 1 );
			
			x = (int)pos.x;
			y = (int)pos.y;
			width = (int)dimen.x;
			height = (int)dimen.y;
		}
		else if ( numParams == 4 )
		{
			GM_CHECK_FLOAT_OR_INT_PARAM( xPos, 0 );
			GM_CHECK_FLOAT_OR_INT_PARAM( yPos, 1 );
			GM_CHECK_FLOAT_OR_INT_PARAM( w, 2 );
			GM_CHECK_FLOAT_OR_INT_PARAM( h, 3 );
			
			x = (int)xPos;
			y = (int)yPos;
			width = (int)w;
			height = (int)h;
		}
		else
		{
			return GM_EXCEPTION;
		}
		
		// make sure it is within the window
		//const v2i windowDimen = Window::Get()->Sizei();
		//ASSERT( x >= 0 && y >= 0 && (x+width)<=windowDimen.x && (x+height)<=windowDimen.y );
		
		RenderState::Ref().SetScissor( x, y, width, height );
		
		return GM_OK;
	}

	static int GM_CDECL gmfSetLineWidth(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_FLOAT_OR_INT_PARAM(width, 0);
		RenderState::Ref().SetLineWidth( max(0.01f,width) );
		return GM_OK;
	}

	static int GM_CDECL gmfEnable(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(val, 0);

		if ( val != 0 ) RenderState::Ref().EnableFlag((GLStateFlag)val);
		return GM_OK;
	}

	static int GM_CDECL gmfDisable(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(val, 0);

		if ( val != 0 ) RenderState::Ref().DisableFlag((GLStateFlag)val);
		return GM_OK;
	}

	static int GM_CDECL gmfSetBlendFunc(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_INT_PARAM(val0, 0);
		GM_CHECK_INT_PARAM(val1, 1);

		RenderState::Ref().SetBlendFunc(val0, val1);
		return GM_OK;
	}

	static int GM_CDECL gmfSetBlendEq(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(val0, 0);

		RenderState::Ref().SetBlendFunc(val0, val0);
		return GM_OK;
	}

	static int GM_CDECL gmfSetWireFrame(gmThread * a_thread)
	{
		GM_CHECK_INT_PARAM(bWireFrame, 0);
		GM_INT_PARAM( mode, 1, GL_FRONT_AND_BACK );
		
		// GL_ES does not have polygon mode
		#if !GL_ES
		glPolygonMode( mode, bWireFrame ? GL_LINE : GL_FILL );
		#endif

		return GM_OK;
	}

	static int GM_CDECL gmfSetPolygonOffset(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_FLOAT_PARAM(factor, 0);
		GM_CHECK_FLOAT_PARAM(units, 1);

		RenderState::Ref().SetPolygonOffset(factor,units);
		return GM_OK;
	}
	
	static int GM_CDECL gmfCullFace(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(mode, 0);

		RenderState::Ref().SetCullFaceMode(mode);
		return GM_OK;
	}

	static int GM_CDECL gmfFrontFace(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(face, 0);

		RenderState::Ref().SetFrontFace(face);
		return GM_OK;
	}

	static int GM_CDECL gmfDepthMask(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(bDepthMask, 0);

		RenderState::Ref().SetDepthMask( bDepthMask != 0 );
		return GM_OK;
	}

	static int GM_CDECL gmfLoadObj(gmThread * a_thread)
	{
		GM_CHECK_STRING_PARAM(file, 0);
		GM_INT_PARAM( swizzleZY, 1, 0 );
		GM_INT_PARAM( genNormals, 2, 0 );

		funk::ObjLoader loader;
		loader.LoadFile(file);
		loader.SetSwizzleZY( swizzleZY != 0 );
		loader.SetGenerateNormals( genNormals != 0 );

		StrongHandle<Vbo> vbo = loader.CreateVbo();
		GM_PUSH_USER_HANDLED( Vbo, vbo.Get() );

		return GM_OK;
	}

	static int GM_CDECL gmfGenerateWhiteTex(gmThread * a_thread)
	{
		// white color
        const unsigned char white_data[4] = { 255, 255, 255, 255 };

		// tex description
		Texture2dDesc desc;
		desc.data_type	= DATATYPE_UNSIGNED_BYTE;
		desc.format		= TEXFORMAT_RGBA;
		desc.dimen		= v2i(1);

		// create tex
		StrongHandle<Texture2d> tex = new Texture2d();
        tex->Init(desc);
        tex->Bind();
        tex->SubData((void*)white_data, 1, 1);
        tex->Unbind();

		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( Texture2d, tex.Get() );
		return GM_OK;
	}

	static int GM_CDECL gmfGenerateBlackTex(gmThread * a_thread)
	{
		// white color
        const unsigned char white_data[4] = { 0, 0, 0, 255 };

		// tex description
		Texture2dDesc desc;
		desc.data_type	= DATATYPE_UNSIGNED_BYTE;
		desc.format		= TEXFORMAT_RGBA;
		desc.dimen		= v2i(1);

		// create tex
		StrongHandle<Texture2d> tex = new Texture2d();
        tex->Init(desc);
        tex->Bind();
        tex->SubData((void*)white_data, 1, 1);
        tex->Unbind();

		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( Texture2d, tex.Get() );
		return GM_OK;
	}
	
	static int GM_CDECL gmfCheckGLError(gmThread * a_thread)
	{
		CheckGLError();
		
		return GM_OK;
	}
};

static gmFunctionEntry s_gmGfxLib[] = 
{	
	// OpenGL
	GM_LIBFUNC_ENTRY(Scissor, Gfx)
	GM_LIBFUNC_ENTRY(Viewport, Gfx)
	GM_LIBFUNC_ENTRY(CheckGLError, Gfx)
	
	// Clear
	GM_LIBFUNC_ENTRY(Clear, Gfx)
	GM_LIBFUNC_ENTRY(ClearDepth, Gfx)
	GM_LIBFUNC_ENTRY(ClearColor, Gfx)

	// Render state
	GM_LIBFUNC_ENTRY(SetLineWidth, Gfx)
	GM_LIBFUNC_ENTRY(SetBlendFunc, Gfx)
	GM_LIBFUNC_ENTRY(SetBlendEq, Gfx)
	GM_LIBFUNC_ENTRY(SetPolygonOffset, Gfx)
	GM_LIBFUNC_ENTRY(SetWireFrame, Gfx)
	GM_LIBFUNC_ENTRY(DepthMask, Gfx)
	GM_LIBFUNC_ENTRY(CullFace, Gfx)
	GM_LIBFUNC_ENTRY(FrontFace, Gfx)
	GM_LIBFUNC_ENTRY(Enable, Gfx)
	GM_LIBFUNC_ENTRY(Disable, Gfx)
    
   	GM_LIBFUNC_ENTRY(Finish, Gfx)
   	GM_LIBFUNC_ENTRY(Flush, Gfx)

	// Models
	GM_LIBFUNC_ENTRY(LoadObj, Gfx)

	// Textures
	GM_LIBFUNC_ENTRY(GenerateWhiteTex, Gfx)
	GM_LIBFUNC_ENTRY(GenerateBlackTex, Gfx)
};

void gmBindGfxLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmGfxLib, sizeof(s_gmGfxLib) / sizeof(s_gmGfxLib[0]), "Gfx" );
}

}