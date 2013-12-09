#include "Texture2d.h"
#include "TextureUtil.h"
#include "GLIncludes.h"
#include "GLDebug.h"
#include "RenderState.h"

#include <common/Debug.h>
#include <gm/gmBind.h>

namespace funk
{
	Texture2d::Texture2d() : m_id(0), m_bind_slot(-1)
	{
	}
	
	Texture2d::~Texture2d()
	{
		if ( m_id ) glDeleteTextures( 1, &m_id );
	}
	
	void Texture2d::Init( const char* a_filename )
	{
		m_id = LoadImage2d( a_filename, m_desc );
		
		ASSERT(m_id);
		ASSERT(m_desc.dimen != v2i(0) );
	
		Bind();
		SetDefaultSampler();
		Unbind();
	}
	
	void Texture2d::Init( const Texture2dDesc a_desc )
	{
		m_desc = a_desc;
		
		// generate id
		glGenTextures(1, &m_id);
		ASSERT( m_id );
		
		// send texture data to GPU
		glBindTexture( GL_TEXTURE_2D, m_id );
		glTexImage2D(GL_TEXTURE_2D, 0, a_desc.internal_format, a_desc.dimen.x, a_desc.dimen.y, 0, a_desc.format, a_desc.data_type, 0 );
		SetDefaultSampler();
		glBindTexture( GL_TEXTURE_2D, 0 );

		CheckGLError();
	}
	
	void Texture2d::SetDefaultSampler()
	{
		SetWrap( TEXWRAP_CLAMP, TEXWRAP_CLAMP );
		SetFilter( TEXFILTER_LINEAR, TEXFILTER_LINEAR );
	}
	
	void Texture2d::Bind( int a_bind_slot )
	{
		// glEnable( GL_TEXTURE_2D ); // deprecated ES2
		
		// make sure it is valid bind index
		ASSERT( a_bind_slot >= 0 );
		ASSERT( a_bind_slot < (GL_TEXTURE31-GL_TEXTURE0) );
		m_bind_slot = a_bind_slot;
		
		RenderState::Ref().SetActiveTexture(a_bind_slot);
		glBindTexture(GL_TEXTURE_2D, m_id );		
	}
	
	void Texture2d::Unbind()
	{
        CHECK( m_bind_slot != -1, "Texture already bound!" );

		RenderState::Ref().SetActiveTexture(m_bind_slot);
		glBindTexture(GL_TEXTURE_2D, 0 );
		
		m_bind_slot = -1;
	}
	
	void Texture2d::SubData( void* a_data, int width, int height, int offsetX, int offsetY, int level )
	{
		CHECK( m_bind_slot != -1, "Texture not bound!");
		ASSERT( width+offsetX <= m_desc.dimen.x && width >= 0 );
		ASSERT( height+offsetY <= m_desc.dimen.y && height >= 0 );
		
		glTexSubImage2D( GL_TEXTURE_2D, level, offsetX, offsetY, width, height, m_desc.format, m_desc.data_type, a_data );
	}
	
	void Texture2d::SetWrap( GLTextureWrapType a_u_type, GLTextureWrapType a_v_type )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_u_type);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_v_type);
	}
	
	void Texture2d::SetFilter( GLTextureFilterType a_min_filter, GLTextureFilterType a_mag_filter )
	{
		// mag filter cannot be mipmaped
		ASSERT( a_mag_filter == TEXFILTER_NEAREST || a_mag_filter == TEXFILTER_LINEAR );

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_min_filter );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_mag_filter );
	}
	
GM_REG_NAMESPACE(Texture2d)
{
	GM_MEMFUNC_DECL(CreateTexture2d)
	{
		StrongHandle<Texture2d> tex = new Texture2d();

		if ( GM_IS_PARAM_STRING(0) )
		{
			GM_CHECK_STRING_PARAM( a_filename, 0 );
			tex->Init(a_filename);
		}
		else
		{
			GM_CHECK_FLOAT_OR_INT_PARAM( a_width, 0 );
			GM_CHECK_FLOAT_OR_INT_PARAM( a_height, 1 );
			GM_INT_PARAM( a_internal_format, 2, TEXFORMAT_INTERNAL_RGBA8 );
			GM_INT_PARAM( a_pixel_format, 3, TEXFORMAT_RGBA );
			GM_INT_PARAM( a_data_type, 4, DATATYPE_UNSIGNED_BYTE );

			Texture2dDesc desc;
			desc.dimen = v2i( (int)a_width, (int)a_height );
			desc.internal_format = (GLTextureInternalFormat)a_internal_format;
			desc.format = (GLTextureFormat)a_pixel_format;
			desc.data_type = (GLDataType)a_data_type;
			tex->Init(desc);
		}
		
		GM_PUSH_USER_HANDLED( Texture2d, tex.Get() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Bind)
	{
		GM_CHECK_NUM_PARAMS_EITHER(0,1);
		GM_INT_PARAM( a_slot, 0, 0 );

		GM_GET_THIS_PTR( Texture2d, ptr );
		ptr->Bind(a_slot);

		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(SetWrap)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_INT_PARAM( a_u_type, 0 );
		GM_CHECK_INT_PARAM( a_v_type, 1 );
		
		GM_GET_THIS_PTR( Texture2d, ptr );
		ptr->SetWrap( (GLTextureWrapType)a_u_type, (GLTextureWrapType)a_v_type );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(SetFilter)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_INT_PARAM( a_min_filter, 0 );
		GM_CHECK_INT_PARAM( a_mag_filter, 1 );
		
		GM_GET_THIS_PTR( Texture2d, ptr );
		ptr->SetFilter( (GLTextureFilterType)a_min_filter, (GLTextureFilterType)a_mag_filter );
		
		return GM_OK;
	}

	GM_GEN_MEMFUNC_INT_VOID( Texture2d, Id )
	GM_GEN_MEMFUNC_VOID_VOID( Texture2d, Unbind )
	GM_GEN_MEMFUNC_V2_VOID( Texture2d, Sizef )
	GM_GEN_MEMFUNC_V2I_VOID( Texture2d, Sizei )
}

GM_REG_MEM_BEGIN(Texture2d)
	GM_REG_MEMFUNC( Texture2d, Id )
	GM_REG_MEMFUNC( Texture2d, Bind )
	GM_REG_MEMFUNC( Texture2d, Unbind )
	GM_REG_MEMFUNC( Texture2d, SetWrap )
	GM_REG_MEMFUNC( Texture2d, SetFilter )
	GM_REG_MEMFUNC( Texture2d, Sizef )
	GM_REG_MEMFUNC( Texture2d, Sizei )
GM_REG_HANDLED_DESTRUCTORS(Texture2d)
GM_REG_MEM_END()
GM_BIND_DEFINE(Texture2d)
}