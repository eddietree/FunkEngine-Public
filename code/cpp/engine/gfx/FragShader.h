
#pragma once

#include <string>
#include <stdint.h>
#include <common/HandledObj.h>

namespace funk
{
    class FragShader : public HandledObj<FragShader>
    {
	private:
		
		uint32_t m_id;
		std::string m_filename;

    public:

		FragShader();
		~FragShader();
		
		void Init( const char* a_filename );
		void Reload();
		
		const std::string& GetFileName() const { return m_filename; }
		uint32_t Id() const { return m_id; }
    };    
}
