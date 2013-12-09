#include "LeapController.h"

#if defined(ENABLE_LEAP)

#include <common/Debug.h>
#include <vm/VirtualMachine.h>
#include <gm/gmMachine.h>
#include <gm/gmCall.h>

namespace funk
{
	const int LEAP_NUM_FRAMES = 8;
	const int LEAP_MAX_NUM_FINGERS_PER_HAND = 5;
	const int LEAP_MAX_NUM_HANDS = 4;
	const int LEAP_MAX_NUM_GESTURES = 64;
	const float LEAP_SCALE = 0.05f;

	v3 toV3( Leap::Vector a_vec )
	{
		v3 result = v3( a_vec.x, a_vec.y, a_vec.z );
		//ASSERT( checkV3(result) );
		if (!checkV3(result) ) return v3(0.0f);
		return result;
	}

	inline LeapVec3 toLeapVec3( Leap::Vector a_vec )
	{
		LeapVec3 result = {a_vec.x, a_vec.y, a_vec.z};
		return result;
	}

	LeapController::LeapController()
	{
		m_ctrl.enableGesture(Leap::Gesture::TYPE_CIRCLE);
		m_ctrl.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
		m_ctrl.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
		m_ctrl.enableGesture(Leap::Gesture::TYPE_SWIPE);

		m_frame_index = 0;
		m_frames.resize(LEAP_NUM_FRAMES);
		memset( &m_device_props, 0, sizeof(m_device_props) );

		for ( size_t i = 0; i < m_frames.size(); ++i )
		{
			LeapFrame &frame = m_frames[i];
			frame.hands.reserve( LEAP_MAX_NUM_HANDS );
			frame.fingers.reserve( LEAP_MAX_NUM_FINGERS_PER_HAND );
			frame.gestures.reserve( LEAP_MAX_NUM_GESTURES );
		}

		LOG("LeapController initialized");
	}

	LeapController::~LeapController()
	{
	}

	void LeapController::Update()
	{
		UpdateDeviceProperties();
		UpdateDevicePropertiesGM();

		UpdateFrameData();
		UpdateFrameGM();
	}

	void LeapController::UpdateDeviceProperties()
	{
		if ( m_ctrl.isConnected() && !m_ctrl.devices().isEmpty() )
		{
			const Leap::Device & device = m_ctrl.devices()[0];

			m_device_props.frustum_x_axis_angle = device.horizontalViewAngle();
			m_device_props.frustum_z_axis_angle = device.verticalViewAngle();
			m_device_props.frustum_y_height = device.range() * LEAP_SCALE;
			m_device_props.is_connected = true;
		}
		else
		{
			m_device_props.is_connected = false;
		}
	}

	void LeapController::UpdateFrameData()
	{
		if ( m_ctrl.isConnected() )
		{
			// internal grab frame
			Leap::Frame frame_ext = m_ctrl.frame();

			// internal frame
			int frame_prev_index = m_frame_index;
			int frame_curr_index = ( m_frame_index + 1 ) % LEAP_NUM_FRAMES;
			
			// setup current frame
			LeapFrame & prev_frame = m_frames[frame_prev_index];
			LeapFrame & curr_frame = m_frames[frame_curr_index];
			
			CopyHandsData( curr_frame, frame_ext );
			CopyGesturesData( curr_frame, frame_ext);

			// update inteaction box
			m_interaction_box.center = toV3( frame_ext.interactionBox().center() ) * LEAP_SCALE;
			m_interaction_box.dimen = v3( frame_ext.interactionBox().width(), frame_ext.interactionBox().height(), frame_ext.interactionBox().depth() ) * LEAP_SCALE;

			//TRACE("center: (%f,%f,%f), dimen: (%f,%f,%f)\n", m_interaction_box.center.x, m_interaction_box.center.y, m_interaction_box.center.z, m_interaction_box.dimen.x, m_interaction_box.dimen.y, m_interaction_box.dimen.z );

			// update frame index
			m_frame_index = frame_curr_index;
		}	
	}

	void LeapController::CopyHandsData( LeapFrame & a_frame_internal, Leap::Frame & a_frame_external )
	{
		const Leap::Device & device = m_ctrl.devices()[0];

		a_frame_internal.hands.clear();
		a_frame_internal.hands.resize(a_frame_external.hands().count());
		a_frame_internal.fingers.clear();

		for ( int i_hand = 0; i_hand < a_frame_external.hands().count(); ++i_hand )
		{
			const Leap::Hand & hand_ext = a_frame_external.hands()[i_hand];

			// setup hand
			LeapHand & hand = a_frame_internal.hands[i_hand];
			hand.id				= hand_ext.id();
			hand.fingers_count	= 0;
			hand.dir			= toV3( hand_ext.direction() );
			hand.palm_normal	= toV3( hand_ext.palmNormal() );
			hand.palm_pos		= toV3( hand_ext.palmPosition() ) * LEAP_SCALE;
			hand.palm_vel		= toV3( hand_ext.palmVelocity() ) * LEAP_SCALE;
			hand.sphere_center	= toV3( hand_ext.sphereCenter() ) * LEAP_SCALE;
			hand.sphere_radius	= hand_ext.sphereRadius() * LEAP_SCALE;
			hand.age			= hand_ext.timeVisible();
			hand.dist_to_boundary = device.distanceToBoundary( hand_ext.palmPosition() ) * LEAP_SCALE;

			int hand_ext_finger_count = hand_ext.fingers().count();

			// clamp the number of fingers if LEAP bugs out and returns  hand with more than 5 fingers
			hand_ext_finger_count = max( hand_ext_finger_count, LEAP_MAX_NUM_FINGERS_PER_HAND );
			//ASSERT( hand_ext_finger_count <= LEAP_MAX_NUM_FINGERS_PER_HAND );

			for( int i_finger = 0; i_finger < hand_ext_finger_count; ++i_finger )
			{
				const Leap::Finger & finger_ext = hand_ext.fingers()[i_finger];

				if ( finger_ext.isValid() )
				{
					// alloc new finger
					a_frame_internal.fingers.resize( a_frame_internal.fingers.size()+1 );
					LeapFinger &finger  = a_frame_internal.fingers.back();

					finger.id			= finger_ext.id();
					finger.hand_id		= hand.id;
					finger.hand_index	= (int)i_hand;
					finger.finger_size	= v2( finger_ext.width(), finger_ext.length() ) * LEAP_SCALE;
					finger.dir			= toV3( finger_ext.direction() );
					finger.tip_pos		= toV3( finger_ext.tipPosition() ) * LEAP_SCALE;
					finger.tip_vel		= toV3( finger_ext.tipVelocity() ) * LEAP_SCALE;
					finger.age			= finger_ext.timeVisible();
					finger.tip_pos_stabilized = toV3( finger_ext.stabilizedTipPosition() ) * LEAP_SCALE;
					finger.dist_to_boundary = device.distanceToBoundary( finger_ext.tipPosition() ) * LEAP_SCALE;

					// add finger
					hand.fingers_index[hand.fingers_count] = (int)a_frame_internal.fingers.size()-1;
					++hand.fingers_count;
				}
			}
		}
	}

	void LeapController::CopyGesturesData( LeapFrame & a_frame_internal, Leap::Frame & a_frame_external )
	{
		a_frame_internal.gestures.clear();

		const int num_gestures = min( a_frame_external.gestures().count(), LEAP_MAX_NUM_GESTURES );

		for ( int i_gesture = 0; i_gesture < num_gestures; ++i_gesture )
		{
			const Leap::Gesture & gesture_ext = a_frame_external.gestures()[i_gesture];

			if (!gesture_ext.isValid()) continue;

			// alloc new gesture
			a_frame_internal.gestures.resize( a_frame_internal.gestures.size()+1 );
			ASSERT( a_frame_internal.gestures.size() < LEAP_MAX_NUM_GESTURES );

			// initialize gesture
			LeapGesture & gesture = a_frame_internal.gestures.back();
			gesture.duration = gesture_ext.durationSeconds();
			gesture.id = gesture_ext.id();

			// set gesture state state
			switch( gesture_ext.state() )
			{
				case Leap::Gesture::STATE_START:	gesture.state = LEAPGESTURE_STATE_START; break;
				case Leap::Gesture::STATE_UPDATE:	gesture.state = LEAPGESTURE_STATE_UPDATE; break;
				case Leap::Gesture::STATE_STOP:		gesture.state = LEAPGESTURE_STATE_STOP; break;
				default: { ASSERT(false); }
			}

			switch( gesture_ext.type() )
			{
				case Leap::Gesture::TYPE_CIRCLE:
				{
					gesture.type = LEAPGESTURE_CIRCLE;

					Leap::CircleGesture subgesture(gesture_ext);
					gesture.data.circle.finger_id = subgesture.pointable().id();
					gesture.data.circle.center = toLeapVec3(subgesture.center() * LEAP_SCALE);
					gesture.data.circle.normal = toLeapVec3(subgesture.normal());
					gesture.data.circle.progress = subgesture.progress();
					gesture.data.circle.radius = subgesture.radius() * LEAP_SCALE;

					break;
				}

				case Leap::Gesture:: TYPE_SWIPE:
				{
					gesture.type = LEAPGESTURE_SWIPE;

					Leap::SwipeGesture subgesture(gesture_ext);
					gesture.data.swipe.finger_id = subgesture.pointable().id();
					gesture.data.swipe.dir = toLeapVec3(subgesture.direction());
					gesture.data.swipe.pos = toLeapVec3(subgesture.position() * LEAP_SCALE);
					gesture.data.swipe.start_pos = toLeapVec3(subgesture.startPosition() * LEAP_SCALE);
					gesture.data.swipe.speed = subgesture.speed() * LEAP_SCALE;

					break;
				}

				case Leap::Gesture::TYPE_SCREEN_TAP:
				{
					gesture.type = LEAPGESTURE_SCREEN_TAP;

					Leap::ScreenTapGesture subgesture(gesture_ext);
					gesture.data.screen_tap.finger_id = subgesture.pointable().id();
					gesture.data.screen_tap.dir = toLeapVec3(subgesture.direction());
					gesture.data.screen_tap.pos = toLeapVec3(subgesture.position() * LEAP_SCALE);

					break;
				}

				case Leap::Gesture::TYPE_KEY_TAP:
				{
					gesture.type = LEAPGESTURE_KEY_TAP;

					Leap::KeyTapGesture subgesture(gesture_ext);
					gesture.data.key_tap.finger_id = subgesture.pointable().id();
					gesture.data.key_tap.dir = toLeapVec3(subgesture.direction());
					gesture.data.key_tap.pos = toLeapVec3(subgesture.position() * LEAP_SCALE);

					break;
				}
                    
                case Leap::Gesture::TYPE_INVALID:
                {
                    ASSERT(false);
                }
			}
		}
	}

	void LeapController::InitGM()
	{
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);
		
		// grab the leap manager object
		gmVariable global_obj_key = gmVariable(vm->AllocStringObject("g_leap"));
		m_gm_leap_manager = vm->GetGlobals()->Get(global_obj_key).GetTableObjectSafe();
		ASSERT( m_gm_leap_manager );
	}

	gmTableObject * LeapController::AllocFreeObj(const char* name)
	{
		// get vm
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		// pool table
		gmTableObject *allocs = m_gm_leap_manager->Get(vm , name).GetTableObjectSafe();
		int alloc_index = allocs->Count()-1;

		// no more left
		if ( alloc_index < 0 ) 
			return nullptr;

		// grab the obj
		gmTableObject * alloc_obj = allocs->Get( alloc_index ).GetTableObjectSafe();
		ASSERT(alloc_obj);

		// remove from pool table
		gmVariable null_var;
		null_var.Nullify();
		allocs->Set( vm, alloc_index, null_var );

		return alloc_obj;		
	}

	void LeapController::Validate()
	{
		// for debugging purposes. make sure the number of fingers in pool and used are same

		// get vm
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		// grab current frame in GM
		gmTableObject * curr_frame_table = m_gm_leap_manager->Get(vm, "curr_frame").GetTableObjectSafe();
		gmTableObject * curr_fingers_table = curr_frame_table->Get(vm, "fingers").GetTableObjectSafe();

		// grab current frame in GM
		gmTableObject * prev_frame_table = m_gm_leap_manager->Get(vm, "prev_frame").GetTableObjectSafe();
		gmTableObject * prev_fingers_table = prev_frame_table->Get(vm, "fingers").GetTableObjectSafe();

		// pool
		gmTableObject *pool = m_gm_leap_manager->Get(vm , "pool_fingers").GetTableObjectSafe();
		assert(pool);

		const int total_num_fingers = 20;

		int pool_count = pool->Count();
		int curr_count = curr_fingers_table->Count();
		int prev_count = prev_fingers_table->Count();

		ASSERT( pool_count + curr_count + prev_count == total_num_fingers );
	}

	void LeapController::UpdateFrameGM()
	{
		// get vm
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		// call LeapManager.PreUpdateData
		gmFunctionObject* preupdate_func = m_gm_leap_manager->Get(vm, "PreUpdateData").GetFunctionObjectSafe();
		ASSERT(preupdate_func);
		gmCall call_pre;
		call_pre.BeginFunction( vm, preupdate_func, gmVariable(m_gm_leap_manager) );
		call_pre.End();

		// grab current frame in GM
		gmTableObject * curr_frame_table = m_gm_leap_manager->Get(vm, "curr_frame").GetTableObjectSafe();
		ASSERT(curr_frame_table);

		// data we are using to populate GM Tables
		const LeapFrame & frame = m_frames[m_frame_index];

		UpdateInteractionBoxGM();
		UpdateFingersGM( curr_frame_table, frame );
		UpdateHandsGM( curr_frame_table, frame );
		UpdateGesturesGM( curr_frame_table, frame );

		// call LeapManager.PostUpdateData
		gmFunctionObject* postupdate_func = m_gm_leap_manager->Get(vm, "PostUpdateData").GetFunctionObjectSafe();
		ASSERT(postupdate_func);
		gmCall call_post;
		call_post.BeginFunction( vm, postupdate_func, gmVariable(m_gm_leap_manager) );
		call_post.End();
	}

	void LeapController::UpdateInteractionBoxGM()
	{
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		gmTableObject * box_table = m_gm_leap_manager->Get(vm, "interaction_box").GetTableObjectSafe();
		ASSERT(box_table);

		box_table->Set( vm, "center", gmVariable( m_interaction_box.center ) );
		box_table->Set( vm, "dimen", gmVariable( m_interaction_box.dimen ) );
	}

	void LeapController::UpdateDevicePropertiesGM()
	{
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		gmTableObject * device_prop_table = m_gm_leap_manager->Get(vm, "device_props").GetTableObjectSafe();
		ASSERT(device_prop_table);

		device_prop_table->Set( vm, "frustum_x_axis_angle", gmVariable( m_device_props.frustum_x_axis_angle ) );
		device_prop_table->Set( vm, "frustum_z_axis_angle", gmVariable( m_device_props.frustum_z_axis_angle ) );
		device_prop_table->Set( vm, "frustum_y_height", gmVariable( m_device_props.frustum_y_height ) );
		device_prop_table->Set( vm, "is_connected", gmVariable( m_device_props.is_connected ? 1:0 ) );
	}

	void LeapController::UpdateFingersGM( gmTableObject* a_frame_table, const LeapFrame & a_frame )
	{
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		gmTableObject * fingers_table = a_frame_table->Get(vm, "fingers").GetTableObjectSafe();
		ASSERT(fingers_table);

		// populate fingers
		for( size_t i = 0; i < a_frame.fingers.size(); ++i )
		{
			const LeapFinger & finger = a_frame.fingers[i];

			// create the finger
			gmTableObject * finger_obj = AllocFreeObj("pool_fingers");
			
			// if none in pool
			if ( finger_obj == nullptr ) 
				continue;

			finger_obj->Set( vm, "id", gmVariable( finger.id ) );
			finger_obj->Set( vm, "hand_id", gmVariable( finger.hand_id ) );
			finger_obj->Set( vm, "hand_index", gmVariable( finger.hand_index ) );
			finger_obj->Set( vm, "finger_size", gmVariable( finger.finger_size ) );
			finger_obj->Set( vm, "dir", gmVariable( finger.dir ) );
			finger_obj->Set( vm, "tip_pos", gmVariable( finger.tip_pos ) );
			finger_obj->Set( vm, "tip_pos_stabilized", gmVariable( finger.tip_pos_stabilized ) );
			finger_obj->Set( vm, "tip_vel", gmVariable( finger.tip_vel ) );
			finger_obj->Set( vm, "age", gmVariable( finger.age ) );
			finger_obj->Set( vm, "dist_to_boundary", gmVariable( finger.dist_to_boundary ) );

			fingers_table->Set( vm, gmVariable(finger.id), gmVariable(finger_obj) );
		}
	}

	void LeapController::UpdateHandsGM( gmTableObject* a_frame_table, const LeapFrame & a_frame )
	{
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		gmTableObject * hands_table = a_frame_table->Get(vm, "hands").GetTableObjectSafe();
		ASSERT(hands_table);
		gmTableObject * fingers_table = a_frame_table->Get(vm, "fingers").GetTableObjectSafe();
		ASSERT(fingers_table);

		// populate hands
		for( size_t i = 0; i < a_frame.hands.size(); ++i )
		{
			const LeapHand & hand = a_frame.hands[i];

			// create the finger
			gmTableObject * hand_obj = AllocFreeObj("pool_hands");

			// if none in pool
			if ( hand_obj == nullptr )
				continue;

			// populate finger ids
			gmTableObject * hand_obj_fingers_ids = hand_obj->Get( vm, "fingers" ).GetTableObjectSafe();
			hand_obj_fingers_ids->ClearTable(vm, false);

			// go through each hand
			ASSERT(hand.fingers_count >= 0);
			const int hand_num_fingers = min( hand.fingers_count, LEAP_MAX_NUM_FINGERS_PER_HAND );
			for ( int j = 0; j < hand_num_fingers; ++j )
			{
				int finger_index = hand.fingers_index[j];
				const LeapFinger & finger = a_frame.fingers[finger_index];
				int finger_id = finger.id;
				
				// grab finger object
				gmVariable finger_obj = fingers_table->Get(gmVariable(finger_id));
				
				// quick fix to ignore finger if null
				if ( finger_obj.IsNull() ) continue;
				ASSERT(!finger_obj.IsNull());

				// make sure the id doesnt already exist
				ASSERT( hand_obj_fingers_ids->Get(gmVariable(finger_id)).IsNull() );
				
				hand_obj_fingers_ids->Set(vm, gmVariable(finger_id), finger_obj );
			}

			hand_obj->Set( vm, "id", gmVariable( hand.id ) );
			hand_obj->Set( vm, "dir", gmVariable( hand.dir ) );
			hand_obj->Set( vm, "palm_normal", gmVariable( hand.palm_normal ) );
			hand_obj->Set( vm, "palm_pos", gmVariable( hand.palm_pos ) );
			hand_obj->Set( vm, "palm_vel", gmVariable( hand.palm_vel ) );
			hand_obj->Set( vm, "sphere_center", gmVariable( hand.sphere_center ) );
			hand_obj->Set( vm, "sphere_radius", gmVariable( hand.sphere_radius ) );
			hand_obj->Set( vm, "age", gmVariable( hand.age ) );
			hand_obj->Set( vm, "dist_to_boundary", gmVariable( hand.dist_to_boundary ) );

			hands_table->Set( vm, gmVariable(hand.id), gmVariable(hand_obj) );
		}
	}

	void LeapController::UpdateGesturesGM( gmTableObject* a_frame_table, const LeapFrame & a_frame )
	{
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		gmTableObject * gestures_table = a_frame_table->Get(vm, "gestures").GetTableObjectSafe();
		ASSERT(gestures_table);

		// populate hands
		for( size_t i = 0; i < a_frame.gestures.size(); ++i )
		{
			const LeapGesture & gesture = a_frame.gestures[i];

			// create the finger
			gmTableObject * gesture_obj = AllocFreeObj("pool_gestures");

			// if none in pool
			if ( gesture_obj == nullptr )
				continue;

			gesture_obj->ClearTable(vm);

			// set basic gesture properties
			gesture_obj->Set( vm, "id", gmVariable( gesture.id ) );
			gesture_obj->Set( vm, "duration", gmVariable( gesture.duration ) );
			gesture_obj->Set( vm, "type", gmVariable( gesture.type ) );
			gesture_obj->Set( vm, "state", gmVariable( gesture.state ) );

			switch( gesture.type )
			{
				case LEAPGESTURE_CIRCLE:
				{
					gesture_obj->Set( vm, "finger_id", gmVariable( gesture.data.circle.finger_id ) );
					gesture_obj->Set( vm, "center", gmVariable( gesture.data.circle.center.toV3() ) );
					gesture_obj->Set( vm, "normal", gmVariable( gesture.data.circle.normal.toV3() ) );
					gesture_obj->Set( vm, "progress", gmVariable( gesture.data.circle.progress) );
					gesture_obj->Set( vm, "radius", gmVariable( gesture.data.circle.radius ) );
					break;
				}

				case LEAPGESTURE_SWIPE:
				{
					gesture_obj->Set( vm, "finger_id", gmVariable( gesture.data.swipe.finger_id ) );
					gesture_obj->Set( vm, "dir", gmVariable( gesture.data.swipe.dir.toV3() ) );
					gesture_obj->Set( vm, "pos", gmVariable( gesture.data.swipe.pos.toV3() ) );
					gesture_obj->Set( vm, "start_pos", gmVariable( gesture.data.swipe.start_pos.toV3() ) );
					gesture_obj->Set( vm, "speed", gmVariable( gesture.data.swipe.speed ) );
					break;
				}

				case LEAPGESTURE_SCREEN_TAP:
				{
					gesture_obj->Set( vm, "finger_id", gmVariable( gesture.data.screen_tap.finger_id ) );
					gesture_obj->Set( vm, "dir", gmVariable( gesture.data.screen_tap.dir.toV3() ) );
					gesture_obj->Set( vm, "pos", gmVariable( gesture.data.screen_tap.pos.toV3() ) );
					break;
				}

				case LEAPGESTURE_KEY_TAP:
				{
					gesture_obj->Set( vm, "finger_id", gmVariable( gesture.data.key_tap.finger_id ) );
					gesture_obj->Set( vm, "dir", gmVariable( gesture.data.key_tap.dir.toV3() ) );
					gesture_obj->Set( vm, "pos", gmVariable( gesture.data.key_tap.pos.toV3() ) );
					break;
				}

				default:
				{
					ASSERT(false);
				}
			}

			gestures_table->Set( vm, gmVariable(gesture.id), gmVariable(gesture_obj) );
		}
	}
}

#endif