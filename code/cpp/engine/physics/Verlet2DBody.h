#pragma once

#include <math/v2.h>

#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>

#include "VerletTypes.h"
#include "VerletBodyTemplate.h"

namespace funk
{
	typedef VerletNode<funk::v2> Verlet2DNode;

	class Verlet2DBody : public VerletBody<v2>,  public funk::HandledObj<Verlet2DBody>
	{
	public:

		Verlet2DBody();

		void Update( float dt );
		void DrawDebug();
		void BakeConstraints();

		GM_BIND_TYPEID(Verlet2DBody);

	private:

	};

	GM_BIND_DECL( Verlet2DBody );
}
