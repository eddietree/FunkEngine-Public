#include "Timer.h"

#include <system/Config.h>

#if TARGET_OS_IPHONE
#include <mach/mach_time.h>
namespace funk
{
	uint64_t GetMachinePerformanceCounter()
	{
		return mach_absolute_time();
	}

	float GetElapsedTimeMillsec( PerformanceCounter a_start, PerformanceCounter a_end )
	{
		PerformanceCounter time_diff = a_end - a_start;

		uint64_t info_data;
		mach_timebase_info_data_t * info_ptr = (mach_timebase_info_data_t*)&info_data;
		return 1e-6 * float(time_diff) * info_ptr->numer / info_ptr->denom;
	}

	Timer::Timer()
	{
		Start();
	}
	
	void Timer::Start()
	{		
		m_start = GetMachinePerformanceCounter();
	}
	
	float Timer::GetTimeSecs() const
	{
		return GetTimeMs() * 1e-3;
	}
	
	float Timer::GetTimeMs() const
	{
		PerformanceCounter time_stop = GetMachinePerformanceCounter();
		return GetElapsedTimeMillsec( m_start, time_stop );
	}
}
#else

#include <SDL2/SDL_timer.h>

namespace funk
{
	PerformanceCounter GetMachinePerformanceCounter()
	{
		return SDL_GetPerformanceCounter();
	}

	float GetElapsedTimeMillsec( PerformanceCounter a_start, PerformanceCounter a_end )
	{
		// NOTE: broken into very small steps to help the compiler through the 32/64/float conversion trauma

		// TODO: move frequency to be global and updated once per frame or so
		const uint64_t freq64 = SDL_GetPerformanceFrequency();
		const uint32_t freq32 = freq64 & 0x00000000FFFFFFFF;
		const float freqf = float( freq32 );

		const uint64_t diff64 = a_end - a_start;
		const uint32_t diff32 = diff64 & 0x00000000FFFFFFFF;
		const float difff = float( diff32 );

		return difff / freqf * 1e3f; 
	}

	Timer::Timer()
	{
		Start();
	}

	void Timer::Start()
	{
		m_start = GetMachinePerformanceCounter();
	}

	float Timer::GetTimeSecs() const
	{
		return GetTimeMs() * 1e-3f;
	}

	float Timer::GetTimeMs() const
	{
		const uint64_t freq64 = SDL_GetPerformanceFrequency();
		const uint32_t freq32 = freq64 & 0x00000000FFFFFFFF;
		const float freqf = float( freq32 );

		const uint64_t diff64 = GetMachinePerformanceCounter() - m_start;
		const uint32_t diff32 = diff64 & 0x00000000FFFFFFFF;
		const float difff = float( diff32 );

		return difff / freqf * 1e3f; 
	}
}
#endif