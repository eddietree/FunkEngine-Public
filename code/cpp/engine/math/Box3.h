
#pragma once

#include "v3.h"

namespace funk
{

class Box3
{
private:
	v3 m_center;
	v3 m_dimen; // full dimension
	
public:
	
	Box3( v3 a_dimen, v3 a_center = v3(0.0f) ) : m_center(a_center), m_dimen(a_dimen) {;}
	
	v3 Size() const { return m_dimen; }
	v3 GetCenter() const { return m_center; }
	
	bool ContaintsPt( v3 a_pt ) const;
};

}
