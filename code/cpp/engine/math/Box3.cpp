#include "Box3.h"

namespace funk
{
	bool Box3::ContaintsPt( v3 a_pt ) const
	{
		float min_x = m_center.x - m_dimen.x*0.5f;
		float max_x = m_center.x + m_dimen.x*0.5f;
		float min_y = m_center.y - m_dimen.y*0.5f;
		float max_y = m_center.y + m_dimen.y*0.5f;
		float min_z = m_center.z - m_dimen.z*0.5f;
		float max_z = m_center.z + m_dimen.z*0.5f;
		
		return a_pt.x >= min_x && a_pt.x <= max_x
			&& a_pt.y >= min_y && a_pt.y <= max_y
			&& a_pt.z >= min_z && a_pt.z <= max_z;
	}
}