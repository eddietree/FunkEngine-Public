
#pragma once

#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>
#include <common/StrongHandle.h>

#include "v3.h"
#include "Perlin.h"

namespace funk
{
	class CurlNoise : public funk::HandledObj<CurlNoise>
	{
	public:
				
		CurlNoise();
		funk::v3 Get( funk::v3 pos );

		GM_BIND_TYPEID(CurlNoise);

	private:

		funk::v3 CalcPotential(funk::v3 pos);
		
		Perlin m_perlin;

	};

	GM_BIND_DECL(CurlNoise);
}
