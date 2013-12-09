#include <common/Debug.h>
#include <CoreFoundation/CoreFoundation.h>
#include <system/Config.h>

namespace funk
{
    const char * GetFullPath( const char * a_filename )
    {
        NSString* base_path = [NSString stringWithUTF8String:a_filename];
		NSString* resource_path = [[NSBundle mainBundle] resourcePath];
		NSString* full_path = [resource_path stringByAppendingPathComponent:base_path];
        
		// debug mode, use working directory for files..
#if !TARGET_OS_IPHONE && !defined(FUNK_FINAL)
		NSString* current_dir = [[NSFileManager defaultManager] currentDirectoryPath];
		full_path = [NSString stringWithFormat:@"%@/%@", current_dir, base_path];
#endif
        
        return (const char*)[full_path UTF8String];;
    }
    
    NSString* GetFullPathNSString( const char * a_filename )
    {
        const char * full_path = GetFullPath( a_filename );
        return [NSString stringWithUTF8String:full_path];
    }

	const char * ReadFile( const char* a_filename, int * out_num_bytes, bool a_binary )
	{
        NSString* full_path = GetFullPathNSString(a_filename);
		
		if ( !full_path )
		{
			LOG( "Cannot find file: '%s'", a_filename );
			ASSERT ( !"File not found" );
		}
        
        if ( a_binary )
        {
            //NSData *databuffer = [file readDataOfLength: (NSUInteger)fsize];
            NSData *databuffer = [NSData dataWithContentsOfFile:full_path];
            *out_num_bytes = (int)[databuffer length];
            ASSERT(*out_num_bytes > 0);
        
            char *bytes_data_ptr = (char *)[databuffer bytes];
            return bytes_data_ptr;
        }
        
        else
        {
            NSString * file_contents = [NSString stringWithContentsOfFile:full_path encoding:NSUTF8StringEncoding error:nil];
            CHECK(file_contents !=nil, "File does not exist: %s", a_filename);
            
            const char* src = (GLchar *)[file_contents UTF8String];
            NSUInteger numberOfBytes = [file_contents lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
            *out_num_bytes = (int)numberOfBytes;
		
            ASSERT(numberOfBytes > 0);
            return src;
        }
	}
    
    bool FileExists( const char* a_filename )
	{
        NSString* full_path = GetFullPathNSString(a_filename);
        
        if ( !full_path ) return false;
        
        NSFileHandle *file = [NSFileHandle fileHandleForReadingAtPath: full_path];
        return file != nil;
	}
}