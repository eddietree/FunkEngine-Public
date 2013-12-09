#pragma once

#include <math/v3.h>
#include <math/v2.h>
#include <vector>

namespace funk
{
	struct LeapFinger
	{
		int id;
		int hand_id;
		int hand_index;  // indices into LeapFrame.hands[]

		float age; // time visible
		float dist_to_boundary;

		v2 finger_size;

		v3 dir;		// pointing direction
		v3 tip_pos; // just the tip
		v3 tip_pos_stabilized;
		v3 tip_vel;
	};

	struct LeapDeviceProperties
	{
		float frustum_x_axis_angle; // full_angle (in radians)
		float frustum_z_axis_angle;
		float frustum_y_height;

		bool is_connected; // is device connected?
	};

	struct LeapInteractionBox
	{
		v3 center;
		v3 dimen;
	};

	struct LeapHand
	{
		int id;

		float age; // time visible
		float dist_to_boundary;

		// fingers
		int fingers_index[5]; // indices into LeapFrame.fingers[]
		int fingers_count;
		
		v3 dir; // dir from palm towards fingers

		// palm
		v3 palm_normal;
		v3 palm_pos;
		v3 palm_vel;

		// sphere (the ball that the hand palms)
		v3		sphere_center;
		float	sphere_radius;

		// from previous frame
		// TODO: translation and rotation since previous frame..
	};

	struct LeapVec3
	{
		float x,y,z;


		v3 toV3() const { return v3(x,y,z); }
	};

	enum LeapGestureType
	{
		LEAPGESTURE_CIRCLE		= 0,
		LEAPGESTURE_SWIPE		= 1,
		LEAPGESTURE_SCREEN_TAP	= 2,
		LEAPGESTURE_KEY_TAP		= 3,
	};

	enum LeapGestureState
	{
		LEAPGESTURE_STATE_INVALID	=-1,
		LEAPGESTURE_STATE_START		= 0,
		LEAPGESTURE_STATE_UPDATE	= 1,
		LEAPGESTURE_STATE_STOP		= 2,
	};

	struct LeapGesture
	{
		LeapGestureType type;
		LeapGestureState state;
		float duration; // seconds
		int id;

		union
		{
			// circle gestures
			struct
			{
				int	finger_id;
				LeapVec3 center;
				LeapVec3 normal;
				float progress; // number of revolutions
				float radius;
			} circle;

			// swipe gesture
			struct
			{
				int finger_id;
				LeapVec3 dir;
				LeapVec3 pos;
				LeapVec3 start_pos;
				float speed; // number of revolutions
			} swipe;

			// screentap gesture
			struct
			{
				int	finger_id;
				LeapVec3 dir;
				LeapVec3 pos;
			} screen_tap;

			// keytap gesture
			struct
			{
				int	finger_id;
				LeapVec3 dir;
				LeapVec3 pos;
			} key_tap;
		} data;
	};

	struct LeapFrame
	{
		std::vector<LeapHand>		hands;
		std::vector<LeapFinger>		fingers;
		std::vector<LeapGesture>	gestures;
	};
}