
#pragma once

#include <stdint.h>
#include <common/HandledObj.h>
#include <math/v2i.h>
#include <math/v2.h>
#include <gm/gmBindHeader.h>

#include "GLTypes.h"

namespace funk
{
	struct Texture2dDesc
	{
		GLTextureInternalFormat	internal_format; // RGBA8, RGBA16, RGBA32F, RGBA16F
		GLTextureFormat			format;	// RGBA, RGB, LUMIN, LUMIN_ALPHA
		GLDataType				data_type; // supports: UNSIGNED_BYTE(32bpp) or UNSIGNED_SHORT_4_4_4_4 (16bpp)
		v2i						dimen;

		// default
		Texture2dDesc() : 
			internal_format(TEXFORMAT_INTERNAL_RGBA8), format(TEXFORMAT_RGBA), data_type(DATATYPE_UNSIGNED_BYTE), dimen(1) {;}
	};
	
    class Texture2d : public HandledObj<Texture2d>
    {
	private:
		
		uint32_t		m_id;
		Texture2dDesc	m_desc;
		int				m_bind_slot;

    public:
		
		Texture2d();
		~Texture2d();
		
		void Init( const char* a_filename ); // init from file
		void Init( const Texture2dDesc a_desc ); // init blank texture
	
		void Bind( int a_bind_slot = 0 );
		void Unbind();
		void SubData( void* a_data, int width, int height, int offsetX=0, int offsetY=0, int level=0 );
		
		// sampling.
		// make sure to bind/unbind when you set these properties
		void SetWrap( GLTextureWrapType a_u_type, GLTextureWrapType a_v_type );
		void SetFilter( GLTextureFilterType a_min_filter, GLTextureFilterType a_mag_filter );
		
		// get properties
		uint32_t Id() const { return m_id; }
		int		GetBindSlot() const { return m_bind_slot; }
		v2i		Sizei() const { return m_desc.dimen; }
		v2		Sizef() const { return v2((float)m_desc.dimen.x, (float)m_desc.dimen.y); }
		const Texture2dDesc & GetDesc() const { return m_desc; }
		
		GM_BIND_TYPEID(Texture2d);
	
	private:
		void SetDefaultSampler();
    };
	
	GM_BIND_DECL(Texture2d);
}
