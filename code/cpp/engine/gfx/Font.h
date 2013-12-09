
#pragma once

#include "Texture2d.h"

#include <math/v2.h>
#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include <gfx/Vbo.h>

namespace funk
{
	class Font : public HandledObj<Font>
	{
	private:
		StrongHandle<Texture2d> m_tex;
		StrongHandle<Vbo> m_vbo;
		
		struct Vert
		{
			v2 pos;
			v2 uv;
		};
		
		std::vector<Vert> m_vertsBuffer;
		int m_vboIndex; // index where to push data
		
		static const int NUM_CHARS = 256;
		
		// character map data
		v2				m_charSizeUVs[NUM_CHARS];
		v2				m_deltaUV;
		unsigned char	m_charWidths[NUM_CHARS];
		int				m_fontSize;
		
	public:
		Font();
		void Init( const char * file, int fontSize );
		
		void Begin(int a_tex_slot = 0);
		void End();
		void Print( const char * text, v2 pos );

		v2i CalcDimen( const char* a_text ) const;
		int CalcCharWidth( unsigned char c ) const { return m_charWidths[c]; }
		int CalcHeight() const { return m_fontSize; };

		StrongHandle<Texture2d> GetTex() const { return m_tex; }

		GM_BIND_TYPEID(Font);

	private:

		void InitVbo();
		int CalcWidth( const char * a_text ) const;
		
	};

	GM_BIND_DECL( Font );
}
