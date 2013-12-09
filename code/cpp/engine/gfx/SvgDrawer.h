
#pragma once

#include "GLIncludes.h"

#include <common/HandledObj.h>
#include <math/v2.h>

#include "Vbo.h"
#include "GLTypes.h"
#include <stdint.h>

namespace funk
{
	struct SvgVert
	{
		v2 pos;
		uint32_t color;
		uint32_t padding; // for full 128-bit alignment
	};

	struct SvgDrawCmd
	{
		GLPrimtive prim;
		uint32_t vert_offset;
		uint32_t vert_count;
	};

	class SvgDrawer : public HandledObj<SvgDrawer>
	{
	private:
		StrongHandle<Vbo> m_vbo;
		std::vector< SvgDrawCmd > m_draw_cmds;

	public:
		SvgDrawer();

		void Draw();
		size_t GetNumDrawCmds() const { return m_draw_cmds.size(); }

	private:

		friend class SvgReader;
		void SetVbo( StrongHandle<Vbo> a_vbo );
		void AddDrawCmd( const SvgDrawCmd & a_cmd );

	};
}
