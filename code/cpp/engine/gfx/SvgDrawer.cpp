#include "SvgDrawer.h"

#include <common/Debug.h>

namespace funk
{

SvgDrawer::SvgDrawer()
{
}

void SvgDrawer::Draw()
{
	m_vbo->Begin();

	for( size_t i = 0; i < m_draw_cmds.size(); ++i )
	{
		const SvgDrawCmd &cmd = m_draw_cmds[i];
		m_vbo->Draw( cmd.prim, cmd.vert_offset, cmd.vert_count );
	}

	m_vbo->End();
}

void SvgDrawer::SetVbo( StrongHandle<Vbo> a_vbo )
{
	ASSERT( a_vbo != nullptr );
	m_vbo = a_vbo;
}

void SvgDrawer::AddDrawCmd( const SvgDrawCmd & a_cmd )
{
	m_draw_cmds.push_back( a_cmd );
}

}