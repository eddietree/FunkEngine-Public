
#pragma once

class gmMachine;
class gmUserArray;
class gmTableObject;

namespace funk
{
	void gmBindImguiLib( gmMachine * a_machine );

	void ImguiOutputTable(gmTableObject* table, gmUserArray* selectArr, bool showFunctions );
}
