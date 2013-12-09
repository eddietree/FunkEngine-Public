#pragma once

#include "FmodIncludes.h"

#include <gfx/LineGraph.h>

#if defined(FMOD_ENABLED)

namespace funk
{
    class FmodFFT
	{
	private:

		static const int FFT_NUM_BINS = 64;

		FMOD::System *m_system;
		FMOD::DSP * m_dsp_fft;

		// zoom in one a single bin
		StrongHandle<LineGraph> m_bin_line_graph;

		float m_spectrum_data[FFT_NUM_BINS];

	public:

		FmodFFT() : m_system(nullptr), m_dsp_fft(nullptr) {;}

		float GetVal( int a_bin_index ) const { return m_spectrum_data[a_bin_index]; }

		void Init( FMOD::System *a_system );
		void Release();
		void Update();
		void Gui();
    };
}

#endif // FMOD_ENABLED