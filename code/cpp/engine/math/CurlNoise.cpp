#include "CurlNoise.h"

#include <gm/gmBind.h>

namespace funk
{

CurlNoise::CurlNoise(): m_perlin(4, 2.0f, 1.0f, 0 )
{

}

v3 CurlNoise::Get( funk::v3 p )
{
	const float e = 1e-4f;
	v3 dx = v3(e, 0.0f, 0.0f);
	v3 dy = v3(0.0f, e, 0.0f);
	v3 dz = v3(0.0f, 0.0f, e);

	v3 p_x0 = CalcPotential(p - dx);
	v3 p_x1 = CalcPotential(p + dx);
	v3 p_y0 = CalcPotential(p - dy);
	v3 p_y1 = CalcPotential(p + dy);
	v3 p_z0 = CalcPotential(p - dz);
	v3 p_z1 = CalcPotential(p + dz);

	float x = p_y1.z - p_y0.z - p_z1.y + p_z0.y;
	float y = p_z1.x - p_z0.x - p_x1.z + p_x0.z;
	float z = p_x1.y - p_x0.y - p_y1.x + p_y0.x;

	const float divisor = 1.0f / (2.f*e);
	return normalize(v3(x, y, z) * divisor );
}

funk::v3 CurlNoise::CalcPotential( funk::v3 pos )
{
	float x = m_perlin.Get(pos.x, pos.y, pos.z);
	float y = m_perlin.Get(pos.y-19.1f, pos.z+33.4f, pos.x+47.2f);
	float z = m_perlin.Get(pos.z+74.2f, pos.x-124.5f, pos.y+99.4f);

	return v3(x,y,z);
}

GM_REG_NAMESPACE(CurlNoise)
{
	GM_MEMFUNC_CONSTRUCTOR(CurlNoise)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( CurlNoise, new CurlNoise() );
		return GM_OK;
	}

	GM_GEN_MEMFUNC_V3_V3( CurlNoise, Get )

}

GM_REG_MEM_BEGIN(CurlNoise)
GM_REG_MEMFUNC( CurlNoise, Get )
GM_REG_HANDLED_DESTRUCTORS(CurlNoise)
GM_REG_MEM_END()
GM_BIND_DEFINE(CurlNoise)

}