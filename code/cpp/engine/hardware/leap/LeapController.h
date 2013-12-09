#pragma once

#include "LeapIncludes.h"

#if defined(ENABLE_LEAP)

#include "LeapTypes.h"

#include <common/Singleton.h>
#include <gm/gmVariable.h>

namespace funk
{
	class LeapController : public Singleton<LeapController>
	{

	private:

		Leap::Controller m_ctrl;
		
		// frames
		int m_frame_index;
		std::vector< LeapFrame > m_frames;

		LeapDeviceProperties m_device_props;
		LeapInteractionBox m_interaction_box;

		gmTableObject * m_gm_leap_manager;

	public:

		void InitGM();
		void Update();

		const LeapFrame & GetCurrentFrame() const { return m_frames[m_frame_index]; }

	private:

		void Validate();
		void UpdateFrameData();
		void UpdateDeviceProperties();

		// copy data from api
		void CopyHandsData( LeapFrame & a_frame_internal, Leap::Frame & a_frame_external );
		void CopyGesturesData( LeapFrame & a_frame_internal, Leap::Frame & a_frame_external );

		// gamemonkey
		void UpdateFrameGM(); // update gamemonkey tables with frame data
		void UpdateDevicePropertiesGM();
		void UpdateInteractionBoxGM();
		void UpdateFingersGM( gmTableObject* a_frame_table, const LeapFrame & a_frame );
		void UpdateHandsGM( gmTableObject* a_frame_table, const LeapFrame & a_frame );
		void UpdateGesturesGM( gmTableObject* a_frame_table, const LeapFrame & a_frame );
		gmTableObject * AllocFreeObj(const char* name);

		friend Singleton<LeapController>;
		LeapController();
		~LeapController();
	};
}

#endif //ENABLE_LEAP