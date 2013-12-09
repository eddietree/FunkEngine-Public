#pragma once

namespace funk
{
	struct VerletConstraint
	{
		// node index
		int start;
		int end;
		
		// parent, helps with angle
		int parent;
		
		// rest properties
		float restLen;
		float restAngleRel; // relative to parent
		float angleStiffness;
		
		VerletConstraint( int _start, int _end, int _parent )
		:	start(_start), end(_end), parent(_parent),
			restLen(1.0f), restAngleRel(0.0f), angleStiffness(0.03f)
		{;}
	};
	
	template<typename T>
	struct VerletNode
	{
		T pos;
		T pos_prev;

		float rigidity;
		int	name_index; // look up into m_node_names
		
		VerletNode( T _pos, float _rigidity, int _name_index )
			: pos(_pos), rigidity(_rigidity), name_index(_name_index) {;}
	};
}
