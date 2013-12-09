
#pragma once

#include "MatrixStack.h"
#include <common/Singleton.h>

namespace funk
{
	class MVP : public Singleton<MVP>
	{
		
	private:
		
		// {Model, View, Projection}
		MatrixStack m_mat_stacks[3];
		
		matrix m_mvp_cache;
		
	public:
		
		MatrixStack& Model()	{ return m_mat_stacks[0]; }
		MatrixStack& View()		{ return m_mat_stacks[1]; }
		MatrixStack& Proj()		{ return m_mat_stacks[2]; }
		
		const MatrixStack& Model()  const { return m_mat_stacks[0]; }
		const MatrixStack& View()	const { return m_mat_stacks[1]; }
		const MatrixStack& Proj()	const { return m_mat_stacks[2]; }
		
		MatrixStack& GetStack(int a_index) { return m_mat_stacks[a_index]; }
		const MatrixStack& GetStack(int a_index)  const { return m_mat_stacks[a_index]; }
		
		// grab mvp matrix (column so actually PVM)
		matrix Get();
		
	private:
		
		friend Singleton<MVP>;
		MVP();
		~MVP();
		
	};
	
}
