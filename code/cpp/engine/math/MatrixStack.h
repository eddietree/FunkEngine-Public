
#pragma once

#include <stack>

#include "matrix.h"

namespace funk
{
	struct MatrixStackEntry
	{
		matrix mat;
	};
	
	class MatrixStack
	{
	private:
		
		bool m_dirty_flag;
		std::stack<MatrixStackEntry> m_stack;
		
	public:
		
		MatrixStack();
		
		void Push();
		void Pop();
		
		void LoadIdentity();
		void Load( const matrix& a_mat );
		
		void Scale( v2 a_scale );
		void Scale( v3 a_scale );
		void Translate( v2 a_translate );
		void Translate( v3 a_translate );
		void Rotate( float a_rot_radians, v3 a_axis );
		void Reflect( v3 a_plane_normal ); // reflection not about origin is not linear
		void Mul( const matrix & a_mat );
		
		const matrix Get() const { return m_stack.top().mat; }
		matrix &Get() { return m_stack.top().mat; }
		
		// Dirty
		bool IsDirty() const { return m_dirty_flag; }
		void SetDirtyOff() { m_dirty_flag = false; }
		
	private:
		
	};
	
}
