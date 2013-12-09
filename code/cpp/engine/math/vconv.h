
#pragma once

#include <math/v2.h>
#include <math/v2i.h>

namespace funk
{
    inline v2 tov2(v2i v) { return v2(float(v.x), float(v.y)); }
    inline v2i tov2i(v2 v) { return v2i(int(v.x), int(v.y)); }
}
