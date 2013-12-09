
#pragma once

#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>
#include "CamBase.h"

namespace funk
{
	class Cam3d : public CamBase, public HandledObj<Cam3d>
	{
	public:
		Cam3d();

		void SetViewAngle( float fov, float aspectRatio );
		void Begin();
		void End();

		GM_BIND_TYPEID(Cam3d);

	private:
		float m_fov;
		float m_aspect;	
	};

	GM_BIND_DECL( Cam3d );
}
