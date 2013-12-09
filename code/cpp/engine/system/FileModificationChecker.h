#pragma once

namespace funk
{
	struct FileModificationCheckerImpl;

    #if defined(_WIN32)
    class FileModificationChecker
    {
    public:
        FileModificationChecker(const char* filename);
        ~FileModificationChecker();

        bool IsModified() const;
        void Sync();

    private:

		FileModificationCheckerImpl* m_impl;
    };
    #else
    
    
    class FileModificationChecker
    {
    public:
        FileModificationChecker(const char* filename){;}
        bool IsModified() const { return false; }
        void Sync() {;}
        
    private:
    };
    #endif
}
