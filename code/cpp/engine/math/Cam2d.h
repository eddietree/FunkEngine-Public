
#pragma once

#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>
#include <math/v2.h>

#include "CamBase.h"

namespace funk
{
	class Cam2d : public CamBase, public HandledObj<Cam2d>
	{
	public:
		void Begin();
		void End();

		void InitScreenSpaceSize(v2 size);
		void InitScreenSpace();
		void SetBounds( v2 bottomLeft, v2 topRight );

		GM_BIND_TYPEID(Cam2d);
		
	private:

		v2 m_bottomLeft;
		v2 m_topRight;
	};

	GM_BIND_DECL( Cam2d );
}
