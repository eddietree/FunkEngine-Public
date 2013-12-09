#include "FileModificationChecker.h"

#if defined(_WIN32)

#include <common/Debug.h>
#include <Windows.h>

namespace funk
{
	struct FileModificationCheckerImpl
	{
		HANDLE file;
        FILETIME time;
	};

    // TODO: maybe some error cases need better handling
    FileModificationChecker::FileModificationChecker(const char* filename)
    {
		m_impl = new FileModificationCheckerImpl;
        m_impl->file = ::CreateFile(filename, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        ASSERT(m_impl->file != INVALID_HANDLE_VALUE);

        ::GetSystemTimeAsFileTime(&m_impl->time);
    }

    FileModificationChecker::~FileModificationChecker()
    {
        ::CloseHandle(m_impl->file);
		delete m_impl;
    }

    bool FileModificationChecker::IsModified() const
    {
        FILETIME creation, access, write;
        BOOL success = ::GetFileTime(m_impl->file, &creation, &access, &write);
        ASSERT(success);
        return ::CompareFileTime(&write, &m_impl->time) > 0;
    }

    void FileModificationChecker::Sync() 
    {
        FILETIME creation, access, write;
        BOOL success = ::GetFileTime(m_impl->file, &creation, &access, &write);
        m_impl->time = write;
    }
}

#endif // defined(_WIN32)