
#include "Memory.h"

#include <new>
//#include <malloc.h>
#include <memory.h>
#include <stddef.h>

#ifdef _MSC_VER

#include <new.h>

// CRT Debug Heap Details
// http://msdn.microsoft.com/en-us/library/974tc9t1.aspx

#include <crtdbg.h>

struct FunkMemoryLeakDetector
{
    FunkMemoryLeakDetector()
    {
        _CrtMemCheckpoint(&start_memstate);
    }

    ~FunkMemoryLeakDetector()
    {
        _CrtMemCheckpoint(&end_memstate);
        _CrtMemDifference(&diff_memstate, &start_memstate, &end_memstate);
        _CrtDumpMemoryLeaks();
    }

    // Memory block types: Free, Normal, CRT, Ignore, Client
    _CrtMemState start_memstate;
    _CrtMemState end_memstate;
    _CrtMemState diff_memstate;
};

static FunkMemoryLeakDetector memory_leak_detector;

void funk_abort(int signal)
{
	DebugBreak();
	abort();
}

void funk_memory_init()
{
    _set_new_mode(0);

	signal(SIGABRT, funk_abort);
	_set_abort_behavior(0, _WRITE_ABORT_MSG);
}

#endif // _MSC_VER

// operator new
// http://www.cplusplus.com/reference/new/operator%20new/

// C++98

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#endif // _MSC_VER

void* operator new (std::size_t size) throw (std::bad_alloc) { return funk_malloc(size, "", 0); }
void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) throw() { return funk_malloc(size, "", 0); }
//void* operator new (std::size_t size, void* ptr) throw() { return ptr; }

void operator delete (void* ptr) throw() { return funk_free(ptr, "", 0); }
void operator delete (void* ptr, const std::nothrow_t& nothrow_constant) throw() { return funk_free(ptr, "", 0); }
//void operator delete (void* ptr, void* voidptr2) throw() { }

void* operator new[] (std::size_t size) throw (std::bad_alloc) { return funk_malloc(size, "", 0); }
void* operator new[] (std::size_t size, const std::nothrow_t& nothrow_value) throw() { return funk_malloc(size, "", 0); }
//void* operator new[] (std::size_t size, void* ptr) throw() { return ptr; }

void operator delete[] (void* ptr) throw() { return funk_free(ptr, "", 0); }
void operator delete[] (void* ptr, const std::nothrow_t& nothrow_constant) throw() { return funk_free(ptr, "", 0); }
//void operator delete[] (void* ptr, void* voidptr2) throw() { }

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

void* funk_malloc(std::size_t n, const char* file, std::size_t line)
{
    return (malloc)(n);
}

void funk_free(void* p, const char* file, std::size_t line)
{
    (free)(p);
}
