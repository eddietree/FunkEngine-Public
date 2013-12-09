
#pragma once

#include "v3.h"
#include "v4.h"
#include "matrix.h"
#include "Plane.h"
#include "Box3.h"

namespace funk
{

class Ray
{
private:
	
	v3 m_pos; // when doing matrix transform, w=1.0
	v3 m_dir; // when doing matrix transform, w=0.0
	
	v3 m_hit_pt;
	v3 m_hit_normal;
	float m_hit_t;
	
	bool m_hit;
	
public:
	Ray() : m_pos(v3(0.0f)), m_dir(v3(0.0f,0.0f,1.0f)), m_hit(false) {}
	Ray( v3 a_pos, v3 a_dir ) : m_pos(a_pos), m_dir(normalize(a_dir)), m_hit(false) {}
	
	bool IntersectTri( v3 a_pt0, v3 a_pt1, v3 a_pt2 ); // submit ccw
	bool IntersectTri( v3 a_pt0, v3 a_pt1, v3 a_pt2, const matrix& a_mat_transform ); // submit ccw
	bool IntersectQuad( v3 a_pt0, v3 a_pt1, v3 a_pt2,  v3 a_pt3 );
	bool IntersectQuad( v3 a_pt0, v3 a_pt1, v3 a_pt2,  v3 a_pt3, const matrix& a_mat_transform );
	bool IntersectPlane( Plane a_plane );
	bool IntersectDisk( v3 a_center, float a_radius, v3 a_normal );
	bool IntersectBox( Box3 a_box, matrix a_mat_transform = matrix::Identity() );
	bool IntersectSphere( v3 center, float radius );
	
	v3 GetHitPt() const { return m_hit_pt; }
	v3 GetHitNormal() const { return m_hit_normal; }
	v3 GetBase() const { return m_pos; }
	v3 GetDir() const { return m_dir; }
	
	bool NewHit( float a_t, v3 a_normal );
	
	// for matrix transform
	Ray Transform( matrix a_mat );
};


}
