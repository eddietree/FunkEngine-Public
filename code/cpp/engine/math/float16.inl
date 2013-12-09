// https://gist.github.com/martinkallman/5049614

// Fast half-precision to single-precision floating point conversion
//  - Supports signed zero and denormals-as-zero (DAZ)
//  - Does not support infinities or NaN
//  - Few, partially pipelinable, non-branching instructions,
//  - Core opreations ~6 clock cycles on modern x86-64
inline void CalcFloat32(float* __restrict out, const uint16_t in) 
{
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
 
    t1 = in & 0x7fff;                       // Non-sign bits
    t2 = in & 0x8000;                       // Sign bit
    t3 = in & 0x7c00;                       // Exponent
        
    t1 <<= 13;                              // Align mantissa on MSB
    t2 <<= 16;                              // Shift sign bit into position
 
    t1 += 0x38000000;                       // Adjust bias
 
    t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero
 
    t1 |= t2;                               // Re-insert sign bit
 
    *((uint32_t*)out) = t1;
};
 
// Fast single-precision to half-precision floating point conversion
//  - Supports signed zero, denormals-as-zero (DAZ), flush-to-zero (FTZ),
//    clamp-to-max
//  - Does not support infinities or NaN
//  - Few, partially pipelinable, non-branching instructions,
//  - Core opreations ~10 clock cycles on modern x86-64
inline void CalcFloat16(uint16_t* __restrict out, const float in) 
{
    uint32_t inu = *((uint32_t*)&in);
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
 
    t1 = inu & 0x7fffffff;                 // Non-sign bits
    t2 = inu & 0x80000000;                 // Sign bit
    t3 = inu & 0x7f800000;                 // Exponent
        
    t1 >>= 13;                             // Align mantissa on MSB
    t2 >>= 16;                             // Shift sign bit into position
 
    t1 -= 0x1c000;                         // Adjust bias
 
    t1 = (t3 > 0x38800000) ? 0 : t1;       // Flush-to-zero
    t1 = (t3 < 0x8e000000) ? 0x7bff : t1;  // Clamp-to-max
    t1 = (t3 == 0 ? 0 : t1);               // Denormals-as-zero
 
    t1 |= t2;                              // Re-insert sign bit
 
    *((uint16_t*)out) = t1;
};

inline float16::float16(float a_val)
{
	CalcFloat16(&m_data, a_val);
}

inline float16::float16(uint16_t a_val)
{
	m_data = a_val;
}

inline float float16::ToFloat() const
{
	float result;
	CalcFloat32(&result, m_data);
	return result;
}

inline bool	float16::operator==( const float16 o ) const
{
	return m_data == o.m_data;
}

inline bool float16::operator!=( const float16 o ) const
{
	return m_data != o.m_data;
}