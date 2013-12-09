#pragma once

#include <stdint.h>

namespace funk
{
	class float16
	{
	private:

		uint16_t m_data;

	public:

		explicit float16( float a_val );
		explicit float16( uint16_t a_val );
		explicit float16( const float16 &a_other) { m_data = a_other.m_data; }

		// converts to float32
		float ToFloat() const;

		inline uint16_t GetData() const { return m_data; }

		bool operator==( const float16 o ) const;
		bool operator!=( const float16 o ) const;
	};

	#include "float16.inl"
}