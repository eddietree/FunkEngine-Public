
#define FUNK_PCH_LIBC
#define FUNK_PCH_WINDOWS_H
#define FUNK_PCH_EXTLIB

// timing tests:
// original: 68s~
// pch libc: 23s~
// pch libc+windows: 24s~
// pch libc+windows+extlib: 16s~
// pch libc+extlib: 16s~
// pch libc+windows+extlib+funkheaders: 13s~

#ifdef FUNK_PCH_LIBC

#ifdef _MSC_VER
#undef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#define __uncaught_exception() (false)
#endif

#include <cassert> 
#include <cctype> 
#include <cerrno> 
#include <cfloat> 
#include <ciso646> 
#include <climits> 
#include <clocale> 
#include <cmath> 
#include <csetjmp> 
#include <csignal> 
#include <cstdarg> 
//#include <cstdbool> 
#include <cstddef> 
#include <cstdint> 
#include <cstdio> 
#include <cstdlib> 
#include <cstring> 
#include <ctime> 
//#include <cuchar> 
#include <cwchar> 
#include <cwctype> 

#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <algorithm>
#include <chrono>
#include <codecvt>
#include <complex>
#include <exception>
#include <functional>
//#include <initializer_list>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <new>
#include <numeric>
#include <random>
#include <ratio>

// NOTE: unsupported without exceptions
//#include <regex>

#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <valarray>

#endif

#ifdef FUNK_PCH_WINDOWS_H

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>

#endif

#ifdef FUNK_PCH_EXTLIB

#include <gfx/GLIncludes.h>
#include <audio/ALIncludes.h>
#include <gm/gmThread.h>
#include <gm/gmBind.h>

#endif

