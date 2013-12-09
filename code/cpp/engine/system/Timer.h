#pragma once

#include <stdint.h>

namespace funk
{
	typedef uint64_t PerformanceCounter;

	PerformanceCounter GetMachinePerformanceCounter();
	float GetElapsedTimeMillsec( PerformanceCounter a_start, PerformanceCounter a_end );

	class Timer
	{
	public:
		Timer();

		void Start();
		float GetTimeSecs() const;
		float GetTimeMs() const;

	private:

		PerformanceCounter m_start;
	};
}
