#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <stdint.h>

#include "VerletTypes.h"

namespace funk
{
	template<typename T>
	class VerletBody
	{
	public:

		VerletBody();
		
		void SetNumIterations( int iter ) { m_numIter = iter; }

		// creation (returns index)
		int AddNode( T pos, const char* name = nullptr, float rigidity = 0.0f );
		int AddNode( T pos, float rigidity ) { return AddNode(pos, nullptr, rigidity); }

		int AddConstraint( int startNodeIndex, int endNodeIndex, int parent_constraint_index = -1 );
		int AddConstraint( const char* a_node0, const char* a_node1, int parent_index = -1 );
		VerletConstraint& GetConstraint( const char* a_node0, const char* a_node1 );
		
		// calculates a position of a constraint, lerped between the nodes
		T CalcPosOnConstraint( int a_constraint_index, float a_alpha ) const;
		T CalcPosOnConstraint( const char* a_node0, const char* a_node1, float a_alpha ) const;
		
		// nodes
		size_t NumNodes() const  { return m_nodes.size(); }
		VerletNode<T> &Node( int index ) { return m_nodes[index]; }
		VerletNode<T> &Node( const char* name  ) { return m_nodes[FindNodeIndex(name)]; }
		const VerletNode<T> &Node( int index ) const { return m_nodes[index]; }
		const VerletNode<T> &Node( const char* name ) const { return m_nodes[FindNodeIndex(name)]; }
		int FindNodeIndex( const char* name ) const;
		T GetNodePos( const char* name ) const { return Node(name).pos; } 
		T GetNodePrevPos( const char* name ) const { return Node(name).pos_prev; } 
		void SetNodePos( const char* name, T a_pos ) { Node(name).pos = a_pos; } 
		
		// constraints
		size_t NumConstraints() const  { return m_constraints.size(); }
		VerletConstraint &Constraint( int index ) { return m_constraints[index]; }

	protected:

		int m_numIter;
		std::vector<VerletNode<T>> m_nodes;
		std::vector<VerletConstraint> m_constraints;
		
		std::unordered_map<uint64_t, int> m_map_constraint_to_index; // map[hashString(name0)^hashString(name1)] = constraint_index
		std::unordered_map<uint32_t, int> m_map_node_names_to_index; // map[hashString(name)] = node_index
		std::vector<std::string> m_node_names;
	};
}

#include "VerletBodyTemplate.tpp"