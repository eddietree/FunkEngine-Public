#include "FmodFFT.h"

#if defined(FMOD_ENABLED)

#include <common/Debug.h>
#include <imgui/Imgui.h>

namespace funk
{
	void FmodFFT::Init( FMOD::System *a_system )
	{
		ASSERT( m_system == nullptr );
		ASSERT( m_dsp_fft == nullptr );

		ASSERT( a_system != nullptr );
		m_system = a_system;

		// master group
		FMOD::ChannelGroup *master_channel_group = nullptr;
		FMOD_ERROR_CHECK( m_system->getMasterChannelGroup( &master_channel_group ) );
		ASSERT( master_channel_group != nullptr );

		// create DSP
		FMOD_ERROR_CHECK( m_system->createDSPByType( FMOD_DSP_TYPE_FFT, &m_dsp_fft ) );		
		ASSERT( m_dsp_fft != nullptr );
		FMOD_ERROR_CHECK( m_dsp_fft->setParameterInt( FMOD_DSP_FFT_WINDOWSIZE, FFT_NUM_BINS ) );
		FMOD_ERROR_CHECK( m_dsp_fft->setParameterInt( FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_HAMMING ) );

		// attach DSP
		FMOD_ERROR_CHECK( master_channel_group->addDSP( 0, m_dsp_fft, 0 ) );
		//FMOD_ERROR_CHECK( master_channel_group->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &dsp_head) );
		//FMOD_ERROR_CHECK( dsp_head->addInput( m_dsp_fft, &m_dsp_connection) );	

		LOG("FmodSystem Initialized FFT DSP");

		m_bin_line_graph = new LineGraph( 0.0f, 1.0f, v2i(300, 150) );
	}

	void FmodFFT::Release()
	{
		ASSERT( m_system != nullptr );
		ASSERT( m_dsp_fft != nullptr );

		// master group
		FMOD::ChannelGroup *master_channel_group = nullptr;
		FMOD_ERROR_CHECK( m_system->getMasterChannelGroup( &master_channel_group ) );
		ASSERT( master_channel_group != nullptr );

		// release DSP
		master_channel_group->removeDSP( m_dsp_fft );
		FMOD_ERROR_CHECK( m_dsp_fft->release() );

		m_system = nullptr;
		m_dsp_fft = nullptr;
	}

	void FmodFFT::Update()
	{
		// grab ptr to fft data
		float *bin_data = nullptr;
		FMOD_ERROR_CHECK( m_dsp_fft->getParameterData( FMOD_DSP_FFT_SPECTRUM , (void**)&bin_data, nullptr, nullptr, 0 ) );

		// copy to buffer
		unsigned int data_len_bytes = FFT_NUM_BINS * sizeof(bin_data[0]);
		memcpy( m_spectrum_data, bin_data, data_len_bytes );

		//FMOD_ERROR_CHECK( m_dsp_fft->getParameterInt( FMOD_DSP_FFT_WINDOWSIZE, &num_bins, nullptr, 0) );
		//FMOD_ERROR_CHECK( m_dsp_fft->getParameterInt( FMOD_DSP_FFT_WINDOWTYPE, &window_type, nullptr, 0) );
		//FMOD_ERROR_CHECK( m_dsp_fft->getParameterData( FMOD_DSP_FFT_SPECTRUM , (void**)&bin_data, &available_bins_count, nullptr, 0 ) );
		//FMOD_ERROR_CHECK( m_dsp_fft->getParameterFloat( FMOD_DSP_FFT_DOMINANT_FREQ, &m_dominant_freq, nullptr, 0 ) );
	}

	void FmodFFT::Gui()
	{
		static bool show_spectrum = false;
		static int display_num_bins = FFT_NUM_BINS;
		static int index_zoom_in_bin = 0;

		// draw spectrum
		show_spectrum = Imgui::CheckBox( "Show Spectrum", show_spectrum );
		if ( show_spectrum )
		{
			// hack to show in new window
			Imgui::End();
			Imgui::Begin("Fmod FFT");

			// bins
			Imgui::SliderInt( "Num Bins", display_num_bins, 1, FFT_NUM_BINS );
			Imgui::BarGraph( m_spectrum_data, display_num_bins, 0.0f, 1.0f, index_zoom_in_bin );
			
			// line graph
			Imgui::Separator();
			Imgui::SliderInt( "Select Bin", index_zoom_in_bin, 0, FFT_NUM_BINS-1 );
			Imgui::FillBarFloat( "Data", m_spectrum_data[index_zoom_in_bin], 0.0f, 1.0f );
			m_bin_line_graph->PushVal( m_spectrum_data[index_zoom_in_bin] );
			Imgui::LineGraph( m_bin_line_graph.Ref() );
		}
	}
}

#endif // FMOD_ENABLED