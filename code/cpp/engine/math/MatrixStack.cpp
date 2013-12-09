#include "MatrixStack.h"

#include <common/Debug.h>

namespace funk
{
	const int MVP_MAX_STACK_ENTRIES = 64;

	MatrixStack::MatrixStack() : m_dirty_flag(true)
	{
		MatrixStackEntry entry;
		entry.mat = matrix::Identity();
		m_stack.push(entry);
	}
	
	void MatrixStack::Push()
	{
		ASSERT( m_stack.size() <= MVP_MAX_STACK_ENTRIES );

		MatrixStackEntry entry;
		entry.mat = m_stack.top().mat;
		m_stack.push(entry);
	}
	
	void MatrixStack::Pop()
	{
		m_dirty_flag = true;
		
		ASSERT(!m_stack.empty());
		m_stack.pop();
	}
	
	void MatrixStack::LoadIdentity()
	{
		m_dirty_flag = true;
		Load( matrix::Identity() );
	}
	
	void MatrixStack::Load( const matrix& a_mat )
	{
		m_dirty_flag = true;
		Get() = a_mat;
	}

	void MatrixStack::Scale( v3 a_scale )
	{
		m_dirty_flag = true;
		Get() *= matrix::Scale( a_scale );
	}
	
	void MatrixStack::Scale( v2 a_scale )
	{
		m_dirty_flag = true;
		v3 scale = v3(a_scale.x, a_scale.y, 1.0f );
		Get() *= matrix::Scale( scale );
	}
	
	void MatrixStack::Translate( v2 a_translate )
	{
		m_dirty_flag = true;
		Get() *= matrix::Translation( a_translate );
	}
	
	void MatrixStack::Translate( v3 a_translate )
	{
		m_dirty_flag = true;
		Get() *= matrix::Translation( a_translate );
	}
	
	void MatrixStack::Rotate( float a_rot_radians, v3 a_axis )
	{
		m_dirty_flag = true;
		Get() *= matrix::Rotation( a_rot_radians, a_axis );
	}

	void MatrixStack::Reflect( v3 a_plane_normal )
	{
		m_dirty_flag = true;
		Get() *= matrix::Reflect( a_plane_normal );
	}
	
	void MatrixStack::Mul( const matrix & a_mat )
	{
		m_dirty_flag = true;
		Get() *= a_mat;
	}
}