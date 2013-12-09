#include "Ray.h"

#include <math.h>

namespace funk
{
	const float EPSILON = 1.e-5f;
	
	bool Ray::IntersectTri( v3 a_pt0, v3 a_pt1, v3 a_pt2 )
	{
		v3 tri_normal = cross( a_pt1 - a_pt0, a_pt2 - a_pt0 );
		const float vdotn = dot( tri_normal, m_dir );
		
        // the ray direction is parallel to triangle plane, return no intersection
		
        if ( fabs(vdotn) <= EPSILON )
			return false;
		
        // signed distance to intersection point
		
        const float sd = dot( a_pt0-m_pos , tri_normal ) / vdotn;
		
        if ( sd < 0.f )
			return false;
		
        const v3 i = m_pos + m_dir * sd;
		
		if ( dot( cross( a_pt1-a_pt0, i-a_pt0 ), tri_normal ) < 0.f ) return false;
		if ( dot( cross( a_pt2-a_pt1, i-a_pt1 ), tri_normal ) < 0.f ) return false;
		if ( dot( cross( a_pt0-a_pt2, i-a_pt2 ), tri_normal ) < 0.f ) return false;
		
        return NewHit( sd, tri_normal );
	}
	
	bool Ray::IntersectTri( v3 a_pt0, v3 a_pt1, v3 a_pt2, const matrix& a_mat )
	{
		v4 pt0 = a_mat * v4(a_pt0,1.0f);
		v4 pt1 = a_mat * v4(a_pt1,1.0f);
		v4 pt2 = a_mat * v4(a_pt2,1.0f);
		
		return IntersectTri(pt0.xyz(), pt1.xyz(), pt2.xyz() );
	}
	
	bool Ray::IntersectQuad( v3 a_pt0, v3 a_pt1, v3 a_pt2,  v3 a_pt3 )
	{
		v3 normal = normalize( cross(a_pt1-a_pt0, a_pt2-a_pt0) );
		const float vdotn = dot(m_dir, normal );
		
        // the ray direction is parallel to triangle plane, return no intersection
		
        if ( fabs(vdotn) <= EPSILON )
			return false;
		
        // signed distance to intersection point
		
        const float sd = ( ( dot( a_pt0 - m_pos, normal ) ) / vdotn );
		
        if ( sd < 0.f )
			return false;
		
        const v3 i = m_pos + m_dir * sd;
		
        if ( dot( cross(a_pt1 - a_pt0, i - a_pt0 ), normal ) < 0.f ) return false;
        if ( dot( cross(a_pt2 - a_pt1, i - a_pt1 ), normal ) < 0.f ) return false;
        if ( dot( cross(a_pt3 - a_pt2, i - a_pt2 ), normal ) < 0.f ) return false;
        if ( dot( cross(a_pt0 - a_pt3, i - a_pt3 ), normal ) < 0.f ) return false;
		
        return NewHit( sd, normal );
	}
	
	bool Ray::IntersectQuad( v3 a_pt0, v3 a_pt1, v3 a_pt2,  v3 a_pt3, const matrix& a_mat )
	{
		v4 pt0 = a_mat * v4(a_pt0,1.0f);
		v4 pt1 = a_mat * v4(a_pt1,1.0f);
		v4 pt2 = a_mat * v4(a_pt2,1.0f);
		v4 pt3 = a_mat * v4(a_pt3,1.0f);
		
		return IntersectQuad(pt0.xyz(), pt1.xyz(), pt2.xyz(), pt3.xyz() );
	}
	
	bool Ray::IntersectPlane( Plane a_plane )
	{
		v3 normal = a_plane.GetNormal();
		const float vdotn = dot( m_dir, normal );
		
        if ( fabs(vdotn) <= EPSILON )
			return false;
        
        return NewHit( - dot( m_pos - m_pos, normal ) / vdotn, normal );
	}
	
	bool Ray::IntersectBox( Box3 a_box, matrix a_mat_transform )
	{
		// translate by position
		a_mat_transform = a_mat_transform * matrix::Translation( a_box.GetCenter() );
		
		v3 halfsize = a_box.Size()*0.5f;
		
		float tmin = -FLT_MAX;
        float tmax = FLT_MAX;
		
        v4 nmin;
        v4 nmax;
		
        const v4 p = a_mat_transform.w - v4(m_pos,1.0f);
		
        for ( size_t i=0; i!=3; ++i )
        {
			v4 box_basis_vector = a_mat_transform[i];
			
			const float e = dot( box_basis_vector, p );
			float f = dot( box_basis_vector, v4(m_dir,0.0f) );
			
			if ( fabs(f) > EPSILON )
			{
				f = 1.0f/f;
				
				float t1 = ( e + halfsize[i] ) * f;
				float t2 = ( e - halfsize[i] ) * f;
				
				float flip = 1.f;
				
				if ( t1 > t2 )
				{
					// swap
					float tmp = t1;
					t1 = t2;
					t2 = tmp;
					
					flip = -flip;
				}
				
				if ( t1 > tmin ) { tmin = t1; nmin = a_mat_transform[i] * flip; }
				if ( t2 < tmax ) { tmax = t2; nmax =-a_mat_transform[i] * flip; }
				
				if ( tmin > tmax )	return false;
				if ( tmax < 0.f )	return false;
			}
			else if ( ( ( -e - halfsize[i] ) > 0.f ) ||
					  ( ( -e + halfsize[i] ) < 0.f ) )
			{
				// ray parallel to the stab
				return false;
			}
        }
		
        if ( tmin > 0.f )
			return NewHit( tmin, nmin.xyz() );

        return NewHit( tmax, nmax.xyz() );
	}
	
	bool Ray::IntersectDisk( v3 a_center, float a_radius, v3 a_normal )
	{
		const float vdotn = dot( m_dir, a_normal );
		
        // the ray direction is parallel to triangle plane, return no intersection
		
        if ( fabs(vdotn) <= EPSILON )
			return false;
		
        // signed distance to intersection point
		
        const float sd = ( dot(a_center, a_normal) - dot(m_pos, a_normal) ) / vdotn;
		
        if ( sd < 0.f )
			return false;
		
        const v4 i = m_pos + m_dir * sd;
		
        if ( lengthSqr( i - a_center ) > a_radius*a_radius )
			return false;
		
        return NewHit( sd, a_normal );
	}
	
	bool Ray::IntersectSphere( v3 center, float radius )
	{
		// http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
		
		v3 ray_pos_transformed = m_pos - center;
		
		// Compute A, B and C coefficients
		float a = dot(m_dir, m_dir);
		float b = 2 * dot(m_dir, ray_pos_transformed);
		float c = dot(ray_pos_transformed, ray_pos_transformed) - (radius * radius);
		
		//Find discriminant
		float disc = b * b - 4 * a * c;
		
		// if discriminant is negative there are no real roots, so return
		// false as ray misses sphere
		if (disc < 0)
			return false;
		
		// compute q as described above
		float distSqrt = sqrtf(disc);
		float q;
		if (b < 0)
			q = (-b - distSqrt)/2.0f;
		else
			q = (-b + distSqrt)/2.0f;
		
		// compute t0 and t1
		float t0 = q / a;
		float t1 = c / q;
		
		// make sure t0 is smaller than t1
		if (t0 > t1)
		{
			// if t0 is bigger than t1 swap them around
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}
				
		// if t1 is less than zero, the object is in the ray's negative direction
		// and consequently the ray misses the sphere
		if (t1 < 0)
			return false;
			
		float t;
		
		// if t0 is less than zero, the intersection point is at t1
		if (t0 < 0)
		{
			t = t1;
		}
		// else the intersection point is at t0
		else
		{
			t = t0;
		}
		
		v3 sphere_pos = ray_pos_transformed + m_dir*t;
		v3 normal = normalize(sphere_pos);
		
		return NewHit(t, normal );
	}
	
	bool Ray::NewHit( float a_t, v3 a_normal )
	{
		if ( a_t < 0.0f ) return false;
		
		m_hit = true;
		m_hit_t = a_t;
		
		m_hit_pt = m_pos + m_dir * a_t;
		m_hit_normal = a_normal;
		
		return true;
	}
	
	Ray Ray::Transform( matrix a_mat )
	{
		v4 pos = v4(m_pos, 1.0f);
		v4 dir = v4(m_dir, 0.0f);
		
		pos = a_mat * pos;
		dir = a_mat * dir;
		
		return Ray(pos.xyz(), dir.xyz());
	}
}