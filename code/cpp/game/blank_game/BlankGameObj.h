#pragma once

#include <gm/gmTableObject.h>
#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>

namespace blank_game
{
	class BlankGameObj : public funk::HandledObj<BlankGameObj>
	{
		
	private:
		
	public:
		BlankGameObj();
		void Update() {;}
		void Draw() {;}

		int Foo( int params ) { return -1;}
		
		GM_BIND_TYPEID(BlankGameObj);
	};

	GM_BIND_DECL(BlankGameObj);
}