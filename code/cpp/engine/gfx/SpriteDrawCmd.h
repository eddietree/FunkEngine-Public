#pragma once

#include <math/v2.h>
#include <stdint.h>

namespace funk
{
	typedef uint32_t SpriteTextureId;

	struct SpriteDrawCmd
	{
		SpriteTextureId tex_id;

		// position
		v2 pos; // bottom left
		v2 dimen; 
		float rotation; // radians, pivots from center

		// draw	
		uint32_t color; // rgba
		v2 uv0; // texcoords
		v2 uv1;
	};

	struct SpriteVert
	{
		v2 pos_center; // center
		v2 pos_local; // 
		v2 texcoord_uv;
		uint32_t color;
		float rotation; // radians
	};
}
