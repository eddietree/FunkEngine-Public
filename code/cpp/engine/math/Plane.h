
#pragma once


#include "v3.h"
#include "v4.h"

namespace funk
{

class Plane
{
public:
	
	v4 data; // ax+by+cz+d = 0
	
public:
	
	Plane( v3 a_normal, v3 a_pt )
	{
		a_normal = normalize(a_normal);
		float d = -dot(a_normal, a_pt) ;
		data = v4(a_normal, d);
	}
	
	Plane( float a, float b, float c, float d ) : data(a,b,c,d) {}
	Plane( v4 a_plane ) : data(a_plane) {}
	
	v3 GetNormal() const { return data.xyz(); }
	
	
	float DistToPt( v3 a_pt )
	{
		float dist = dot(a_pt, data.xyz()) + data.w;
		return dist;
	}
};

}
