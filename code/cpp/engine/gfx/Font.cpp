#include "Font.h"

#include "GLIncludes.h"
#include "GLDebug.h"
#include <system/DataFile.h>
#include <gm/gmBind.h>

namespace funk
{

const int MAX_CHARS = 4096;
const int MAX_VERTS_PER_CHAR = 6;
const int MAX_VERTS = MAX_CHARS*MAX_VERTS_PER_CHAR;

inline void GetCharRowCol( unsigned char c, int &row, int &col )
{
	row = c >> 4;
	col = c % 16;
}

Font::Font()
{}

void Font::Init( const char * file, int fontSize )
{
	ASSERT(file);
	ASSERT(fontSize > 0);
	
	// for some reason, need to pad by this
	fontSize += 2;

	// load character map data..
	char file_datamap[128];
	sprintf(file_datamap, "%s.dat", file);

	DataFile file_data;
    file_data.Load(file_datamap);
	memcpy( m_charWidths, file_data.GetData(), sizeof(m_charWidths) );
	
	// load texture
	m_tex = new Texture2d();
	m_tex->Init(file);
	v2i tex_dimen = m_tex->Sizei();
	m_fontSize = fontSize;
	
	// filters
	m_tex->Bind();
	m_tex->SetFilter( TEXFILTER_NEAREST, TEXFILTER_NEAREST );
	m_tex->Unbind();

	// calc uv tex
	float uvY = (float)m_fontSize / tex_dimen.y;
	for ( int i = 0; i < NUM_CHARS; ++i )
	{
		float u = (float)m_charWidths[i] / tex_dimen.x;
		m_charSizeUVs[i] = v2(u,uvY);
	}

	CheckGLError();
	InitVbo();
	
	CheckGLError();
}

void Font::InitVbo()
{
	m_vboIndex = 0;

	StrongHandle< VertexLayout > layout = new VertexLayout;
	layout->BeginEntries( sizeof(Vert) );
	layout->AddEntry( DATATYPE_FLOAT, 2, offsetof(Vert,pos) );
	layout->AddEntry( DATATYPE_FLOAT, 2, offsetof(Vert,uv) );
	layout->EndEntries();
	
	m_vbo = new Vbo();
	m_vbo->Init( nullptr, MAX_VERTS, sizeof(Vert), layout, BUFFER_STREAM );
}

void Font::Print( const char * text, v2 pos )
{
	// early outs
	if ( text == NULL ) return;
	const int len = (int)strlen( text );
	if ( len == 0 ) return;

	// make sure has enough verts to handle
	ASSERT(len < MAX_CHARS);

	pos.x = floorf(pos.x);
	pos.y = floorf(pos.y);	

	// build verts
	const int maxBatchNumVerts = len*MAX_VERTS_PER_CHAR;
	m_vertsBuffer.resize(maxBatchNumVerts);
	int numCharsCount = 0;

	const float uvDivisor = 1.0f / 16.0f;
	float xOffset = 0.0f;
	
	for ( int i = 0; i < len; ++i )
	{
		const unsigned char c = text[i];
		const v2 charDimen = v2( (float)m_charWidths[c], (float)m_fontSize );
		const v2 deltaUV = m_charSizeUVs[c];
		
		// skip spaces
		if ( c == ' ' )
		{
			xOffset += charDimen.x;
			continue;
		}
		else if ( c == '\t' ) // tabs are 4 spaces!
		{
			const int tab_num_spaces = 4;
			float tab_dimen = (float)m_charWidths[' '] * tab_num_spaces;
			xOffset += tab_dimen;
			continue;
		}

		// calc start uv
		int row, col;
		GetCharRowCol( c, row, col );

		const v2 uv = v2( col*uvDivisor, row*uvDivisor );
		const int vertIndex = numCharsCount * MAX_VERTS_PER_CHAR;
		
		v2 uvs[4] =
		{
			v2( uv.x, uv.y+deltaUV.y ),
			v2( uv.x+deltaUV.x, uv.y+deltaUV.y ),
			v2( uv.x+deltaUV.x, uv.y ),
			v2( uv.x, uv.y )
		};
		
		float pos_x = pos.x + xOffset;
		v2 positions[4] =
		{
			v2( pos_x, pos.y-charDimen.y  ),
			v2( pos_x + charDimen.x, pos.y-charDimen.y ),
			v2( pos_x + charDimen.x, pos.y),
			v2( pos_x, pos.y),
		};
		
		m_vertsBuffer[vertIndex+0].uv = uvs[0];
		m_vertsBuffer[vertIndex+1].uv = uvs[1];
		m_vertsBuffer[vertIndex+2].uv = uvs[2];
		m_vertsBuffer[vertIndex+3].uv = uvs[0];
		m_vertsBuffer[vertIndex+4].uv = uvs[2];
		m_vertsBuffer[vertIndex+5].uv = uvs[3];
		
		m_vertsBuffer[vertIndex+0].pos = positions[0];
		m_vertsBuffer[vertIndex+1].pos = positions[1];
		m_vertsBuffer[vertIndex+2].pos = positions[2];
		m_vertsBuffer[vertIndex+3].pos = positions[0];
		m_vertsBuffer[vertIndex+4].pos = positions[2];
		m_vertsBuffer[vertIndex+5].pos = positions[3];

		xOffset += charDimen.x;
		++numCharsCount;
	}
	
	if ( numCharsCount == 0 ) return;
	
	int batchNumVerts = numCharsCount*MAX_VERTS_PER_CHAR;

	// wrap around if not enough room
	if ( m_vboIndex + batchNumVerts > MAX_CHARS ) m_vboIndex = 0;

	// send and render
	m_vbo->SubData( (unsigned char*)&m_vertsBuffer[0], batchNumVerts*sizeof(Vert), m_vboIndex*sizeof(Vert) );
	m_vbo->Draw( PRIM_TRIANGLES, m_vboIndex, batchNumVerts );

	// move index pointer along
	m_vboIndex += batchNumVerts;
}
	
void Font::Begin( int a_tex_slot )
{
	m_tex->Bind(a_tex_slot);
	m_vbo->Begin();
}
	
void Font::End()
{
	m_vbo->End();
	m_tex->Unbind();
}

v2i Font::CalcDimen( const char* a_text ) const
{
	return v2i(CalcWidth(a_text), CalcHeight() );
}

int Font::CalcWidth( const char * a_text ) const
{
	int width = 0;

	int len = (int)strlen(a_text);
	for( int i = 0; i < len; ++i )
	{
		unsigned char c = a_text[i];
		width += m_charWidths[c];
	}

	return width;
}

GM_REG_NAMESPACE(Font)
{
	GM_MEMFUNC_CONSTRUCTOR(Font)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(filename, 0);
		GM_CHECK_INT_PARAM(fontSize, 1);

		StrongHandle<Font> font = new Font();
		font->Init(filename, fontSize );
		GM_PUSH_USER_HANDLED( Font, font.Get() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Print)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(str, 0);
		GM_CHECK_V2_PARAM(pos, 1);

		GM_GET_THIS_PTR(Font, ptr);
		ptr->Print(str, pos);
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(CalcDimen)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(str, 0);

		GM_GET_THIS_PTR(Font, ptr);
		v2i dimen = ptr->CalcDimen(str);
		a_thread->PushV2( v2((float)dimen.x, (float)dimen.y) );

		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(GetTex)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(Font, ptr);
		GM_PUSH_USER_HANDLED( Texture2d, ptr->GetTex() );
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Begin)
	{
		GM_INT_PARAM( a_bind_slot, 0, 0)
		GM_GET_THIS_PTR(Font, ptr);
		ptr->Begin(a_bind_slot);
		
		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_VOID_VOID( Font, End )
}

GM_REG_MEM_BEGIN(Font)
	GM_REG_MEMFUNC( Font, Print )
	GM_REG_MEMFUNC( Font, CalcDimen )
	GM_REG_MEMFUNC( Font, GetTex )
	GM_REG_MEMFUNC( Font, Begin )
	GM_REG_MEMFUNC( Font, End )
GM_REG_HANDLED_DESTRUCTORS(Font)
GM_REG_MEM_END()
GM_BIND_DEFINE(Font)

}