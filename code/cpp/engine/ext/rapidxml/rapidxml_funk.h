
#pragma once

// configuration defines
// defaults are mostly ok

//#define RAPIDXML_NO_STDLIB
#define RAPIDXML_NO_EXCEPTIONS
//#define RAPIDXML_STATIC_POOL_SIZE (64 * 1024)
//#define RAPIDXML_DYNAMIC_POOL_SIZE (64 * 1024)
//#define RAPIDXML_ALIGNMENT sizeof(void *)

#include <common/Debug.h>

#include <ext/rapidxml/rapidxml.hpp>

namespace rapidxml
{
    void parse_error_handler(const char *what, void *where)
    {
        ASSERT(false);
    }
}
