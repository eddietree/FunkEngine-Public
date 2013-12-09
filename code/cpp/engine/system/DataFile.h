
#pragma once

namespace funk
{
	class DataFile
	{
	private:

		char m_ext[8]; // file extension (ex. "mp3")
		int m_num_bytes;
		char* m_data;

	public:
		DataFile();
		~DataFile();		

		// returns number of bytes
		void Load( const char* a_filename, bool binary = false );

		inline void* GetData() const { return (void*)m_data; }
		inline int GetNumBytes() const { return m_num_bytes; }
		inline const char* GetExtension() const { return &m_ext[0]; }

	private:
		void Release();
	};
}
