#include "VerletBodyTemplate.h"

#include <common/Debug.h>
#include <math/v3.h>
#include <math.h>
#include <common/Util.h>

namespace funk
{

template <typename T>
VerletBody<T>::VerletBody() : m_numIter(2)
{;}
 
template <typename T>
int VerletBody<T>::AddNode( T pos, const char* name, float rigidity )
{
	int name_index = -1;
	
	// add name if user specificed
	if ( name )
	{
		// name does not exist!
		ASSERT( FindNodeIndex(name) == -1 );
		
		// add node
		m_node_names.push_back(name);
		name_index = (int)(m_node_names.size()-1);
		
		// add the node to map
		uint32_t name_hash = HashString(name);
		m_map_node_names_to_index.insert( std::make_pair(name_hash, (uint32_t)m_nodes.size() ) );
	}
	
	m_nodes.push_back( VerletNode<T>(pos, rigidity, name_index) );
	return (int)m_nodes.size()-1;
}

template <typename T>
int VerletBody<T>::FindNodeIndex( const char* name ) const
{
	uint32_t name_hash = HashString(name);
	
	auto it = m_map_node_names_to_index.find(name_hash);
	if ( it == m_map_node_names_to_index.end() ) return -1;
	
	return it->second;
}

template <typename T>
int VerletBody<T>::AddConstraint( int startNodeIndex, int endNodeIndex, int parentIndex )
{
	assert( startNodeIndex >= 0 && startNodeIndex < (int)m_nodes.size() );
	assert( endNodeIndex >= 0 && endNodeIndex < (int)m_nodes.size() );

	m_constraints.push_back( VerletConstraint(startNodeIndex, endNodeIndex, parentIndex) );
	return (int)m_constraints.size()-1;
}
	
template <typename T>
int VerletBody<T>::AddConstraint( const char* a_node0, const char* a_node1, int parent_index )
{
	ASSERT( a_node0 );
	ASSERT( a_node1 );
	
	int node_index0 = FindNodeIndex(a_node0);
	int node_index1 = FindNodeIndex(a_node1);
	ASSERT( node_index0 != -1 );
	ASSERT( node_index1 != -1 );
		
	int constraint_index = AddConstraint( node_index0, node_index1, parent_index );
	
	// add to the hash
	uint64_t node_constraint_hash = HashStringPair(a_node0, a_node1);
	m_map_constraint_to_index.insert( std::make_pair(node_constraint_hash, constraint_index ) );

	return constraint_index;
}
	
template <typename T>
VerletConstraint& VerletBody<T>::GetConstraint( const char* a_node0, const char* a_node1 )
{
	ASSERT(a_node0);
	ASSERT(a_node1);

	uint64_t node_constraint_hash = HashStringPair(a_node0, a_node1);
	auto it = m_map_constraint_to_index.find(node_constraint_hash);
	
	ASSERT( it != m_map_constraint_to_index.end() );
	
	int constraint_index = it->second;
	return m_constraints[constraint_index];
}

template <typename T>
T VerletBody<T>::CalcPosOnConstraint( int a_constraint_index, float a_alpha ) const
{
	ASSERT( a_constraint_index < (int)m_constraints.size() );
	
	const VerletConstraint &constraint = m_constraints[a_constraint_index];
	T pos0 = m_nodes[constraint.start].pos;
	T pos1 = m_nodes[constraint.end].pos;
	
	return lerp(pos0, pos1, a_alpha);
}
	
template <typename T>
T VerletBody<T>::CalcPosOnConstraint( const char* a_node0, const char* a_node1, float a_alpha ) const
{
	uint64_t node_constraint_hash = HashStringPair(a_node0, a_node1);
	auto it = m_map_constraint_to_index.find(node_constraint_hash);
	
	ASSERT( it != m_map_constraint_to_index.end() );
	
	int constraint_index = it->second;
	return CalcPosOnConstraint(constraint_index, a_alpha );
}

}

