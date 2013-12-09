#pragma once

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include "GLTypes.h"

#include <stdint.h>

namespace funk
{
	class Ibo : public HandledObj<Ibo>
	{
	public:
		Ibo();
		~Ibo();

		void Init( void* a_data, uint32_t a_num_elems, uint32_t a_bytes_per_elem = sizeof(uint32_t), GLDataType a_data_type = DATATYPE_UNSIGNED_INT, GLBufferType a_buffer_type = BUFFER_STATIC );
		
		// that's right, all the verts
		static const uint32_t RENDER_ALL_VERTS = 0;

		void Begin();
		void End();
		void Draw(GLPrimtive a_render_primitive = PRIM_TRIANGLES, uint32_t a_num_elems = RENDER_ALL_VERTS );

		uint32_t Id() const { return m_id; }

		GM_BIND_TYPEID(Ibo);

	private:
		uint32_t m_buffer_type;
		uint32_t m_id;
		uint32_t m_num_elems;
		GLDataType m_data_type;
	};

	GM_BIND_DECL(Ibo);
}
