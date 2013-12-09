#include "Verlet3DBody.h"

#include <common/Debug.h>
#include <math.h>

#include <common/Util.h>
#include <gfx/ImmGfx.h>
#include <gm/gmBind.h>

namespace funk
{

Verlet3DBody::Verlet3DBody()
{;}

void Verlet3DBody::Update( float dt )
{
	// copy previous positions (used in velocity)
	for( Verlet3DNode & node : m_nodes )
	{
		node.pos_prev = node.pos;
	}

	// update all nodes
	for ( int iter = 0; iter < m_numIter; ++iter )
	{
		for ( size_t i = 0; i < m_constraints.size(); ++i )
		{
			VerletConstraint & c = m_constraints[i];
			Verlet3DNode & startNode = m_nodes[ c.start ];
			Verlet3DNode & endNode = m_nodes[ c.end ];
			const float restLen = c.restLen;
			const float restAngle = c.restAngleRel;

			// endpts
			v3 v0 = startNode.pos;
			v3 v1 = endNode.pos;

			/*// parentangle constraint
			float parentAngle = 0.0f;
			if ( c.parent != -1 )
			{
				const VerletConstraint & parentConstraint = m_constraints[c.parent];
				const v3 parentStart = m_nodes[parentConstraint.start].pos;
				const v3 parentEnd = m_nodes[parentConstraint.end].pos;
				const v3 parentBranchVec = parentEnd - parentStart;
				parentAngle = atan2f( parentBranchVec.y, parentBranchVec.x );
			}	

			// goal angle
			float goalAngle = parentAngle + restAngle;
			v3 goalPos = v0+v3( cosf(goalAngle), sinf(goalAngle) ) * restLen;
			v1 = lerp( v1, goalPos, c.angleStiffness );*/

			// goal position
			v3 delta = v1 - v0;
			float deltaLen = length(delta);
			float diff = (deltaLen - restLen)/deltaLen;
			v3 half_delta_diff = delta * 0.5f * diff;

			v0 += half_delta_diff * (1.0f-startNode.rigidity);
			v1 -= half_delta_diff * (1.0f-endNode.rigidity);

			// storage
			startNode.pos = v0;
			endNode.pos = v1;
		}
	}
}

void Verlet3DBody::BakeConstraints()
{
	for ( size_t i = 0; i < m_constraints.size(); ++i )
	{
		VerletConstraint &c = m_constraints[i];

		const Verlet3DNode & nodeStart = m_nodes[ c.start ];
		const Verlet3DNode & nodeEnd = m_nodes[ c.end ];
		const v3 & v0 = nodeStart.pos;
		const v3 & v1 = nodeEnd.pos;
		const v3 dir = v1 - v0;
		const v3 dirNorm = normalize(dir);

		c.restLen = length(v1-v0);

		// calc relative angle relative to parent
		if ( c.parent != -1 )
		{
			const VerletConstraint & parentConstraint = m_constraints[c.parent];
			const v3 parentStart = m_nodes[parentConstraint.start].pos;
			const v3 parentEnd = m_nodes[parentConstraint.end].pos;
			const v3 parentBranchVec = normalize(parentEnd-parentStart);

			float dotProd = dot(parentBranchVec, normalize(dir));
			float relAngle = 0.0f;

			if ( dotProd <= 0.999f)
			{
				relAngle = acosf(dotProd);
				relAngle *= cross( v3(parentBranchVec), v3(dirNorm)).z;
			}
			
			c.restAngleRel = relAngle;
		}
		else
		{
			c.restAngleRel = atan2f( dirNorm.y, dirNorm.x );
		}
	}
}

void Verlet3DBody::DrawDebug()
{
	ImmGfx & imm = ImmGfx::Ref();
	
	imm.BeginDefaultShader();	
	imm.BeginDraw();

	// draw constraints
	imm.PolygonBegin( PRIM_LINES );
	for( const VerletConstraint& constraint : m_constraints )
	{
		v3 vert_0 = m_nodes[ constraint.start ].pos;
		v3 vert_1 = m_nodes[ constraint.end ].pos;
		
		imm.Vertex( vert_0 );
		imm.Vertex( vert_1 );
	}
	imm.PolygonEnd();

	// draw nodes
	imm.SetDefaultShaderPointSize(10.0f);
	imm.PolygonBegin( PRIM_POINTS );
	for( const Verlet3DNode& node : m_nodes )
	{
		imm.Vertex( node.pos );
	}
	imm.PolygonEnd();

	imm.EndDraw();
	imm.EndDefaultShader();
}

GM_REG_NAMESPACE(Verlet3DBody)
{
	GM_MEMFUNC_CONSTRUCTOR(Verlet3DBody)
	{
		GM_CHECK_NUM_PARAMS(0);
		StrongHandle<Verlet3DBody> body = new Verlet3DBody();		
		GM_PUSH_USER_HANDLED( Verlet3DBody, body.Get() );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(AddNode)
	{
		GM_CHECK_NUM_PARAMS_EITHER( 2, 3 );
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_STRING_PARAM( a_name, 1 );
		GM_FLOAT_PARAM( a_rigidity, 2, 0.0f );

		GM_GET_THIS_PTR(Verlet3DBody, ptr);
		ptr->AddNode( a_pos, a_name, a_rigidity );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(AddConstraint)
	{
		GM_CHECK_NUM_PARAMS_EITHER( 2, 3 );
		GM_CHECK_STRING_PARAM( a_node_name_0, 0 );
		GM_CHECK_STRING_PARAM( a_node_name_1, 1 );
		GM_INT_PARAM( a_parent_index, 2, -1 );

		GM_GET_THIS_PTR(Verlet3DBody, ptr);
		a_thread->PushInt( ptr->AddConstraint( a_node_name_0, a_node_name_1, a_parent_index ) );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(CalcPosOnConstraint)
	{
		GM_CHECK_NUM_PARAMS_EITHER( 2, 3 );
		GM_CHECK_STRING_PARAM( a_node_name_0, 0 );
		GM_CHECK_STRING_PARAM( a_node_name_1, 1 );
		GM_CHECK_FLOAT_PARAM( a_alpha, 2 );

		GM_GET_THIS_PTR(Verlet3DBody, ptr);
		a_thread->PushV3( ptr->CalcPosOnConstraint( a_node_name_0, a_node_name_1, a_alpha ) );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetNodePos)
	{
		GM_CHECK_NUM_PARAMS_EITHER( 2, 3 );
		GM_CHECK_STRING_PARAM( a_node_name, 0 );
		GM_CHECK_V2_PARAM( a_pos, 1 );

		GM_GET_THIS_PTR(Verlet3DBody, ptr);
		ptr->SetNodePos( a_node_name, a_pos );

		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_VOID_VOID( Verlet3DBody, DrawDebug )
	GM_GEN_MEMFUNC_VOID_FLOAT( Verlet3DBody, Update )
	GM_GEN_MEMFUNC_V3_STRING( Verlet3DBody, GetNodePos )
	GM_GEN_MEMFUNC_V3_STRING( Verlet3DBody, GetNodePrevPos )
	GM_GEN_MEMFUNC_INT_VOID( Verlet3DBody, NumConstraints )
	GM_GEN_MEMFUNC_INT_VOID( Verlet3DBody, NumNodes )
	GM_GEN_MEMFUNC_VOID_INT( Verlet3DBody, SetNumIterations )
	GM_GEN_MEMFUNC_VOID_VOID( Verlet3DBody, BakeConstraints )

}

GM_REG_MEM_BEGIN(Verlet3DBody)
GM_REG_MEMFUNC( Verlet3DBody, DrawDebug )
GM_REG_MEMFUNC( Verlet3DBody, Update )
GM_REG_MEMFUNC( Verlet3DBody, AddNode )
GM_REG_MEMFUNC( Verlet3DBody, GetNodePos )
GM_REG_MEMFUNC( Verlet3DBody, GetNodePrevPos )
GM_REG_MEMFUNC( Verlet3DBody, SetNodePos )
GM_REG_MEMFUNC( Verlet3DBody, NumNodes )
GM_REG_MEMFUNC( Verlet3DBody, AddConstraint )
GM_REG_MEMFUNC( Verlet3DBody, CalcPosOnConstraint )
GM_REG_MEMFUNC( Verlet3DBody, NumNodes )
GM_REG_MEMFUNC( Verlet3DBody, SetNumIterations )
GM_REG_MEMFUNC( Verlet3DBody, BakeConstraints )

GM_REG_HANDLED_DESTRUCTORS(Verlet3DBody)
GM_REG_MEM_END()
GM_BIND_DEFINE(Verlet3DBody)

}