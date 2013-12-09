#pragma once

#include <math/v3.h>

#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>

#include "VerletTypes.h"
#include "VerletBodyTemplate.h"

namespace funk
{
	typedef VerletNode<funk::v3> Verlet3DNode;

	class Verlet3DBody : public VerletBody<v3>,  public funk::HandledObj<Verlet3DBody>
	{
	public:

		Verlet3DBody();

		void Update( float dt );
		void DrawDebug();
		void BakeConstraints();

		GM_BIND_TYPEID(Verlet3DBody);

	private:

	};

	GM_BIND_DECL( Verlet3DBody );
}
