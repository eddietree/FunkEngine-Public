
#pragma once

#include <math/v2.h>
#include <math/v3.h>
#include <math/v4.h>
#include <math/matrix.h>
#include <math/Box3.h>

#include <gm/gmVariable.h>

#include <stdint.h>

namespace funk
{
	enum TouchEventType
	{
		TOUCHEVENT_BEGIN,
		TOUCHEVENT_MOVE,
		TOUCHEVENT_STATIONARY,
		TOUCHEVENT_END,
		TOUCHEVENT_CANCEL,
		
		// used for counting
		TOUCHEVENT_COUNT,
	};
	
	enum TouchCollisionType
	{
		COLLISIONTYPE_BOX,
		COLLISIONTYPE_SPHERE,
        COLLISIONTYPE_PLANE,
		COLLISIONTYPE_TRIANGLE,
		COLLISIONTYPE_DISK,
		COLLISIONTYPE_QUAD,
		
		// used for counting
		COLLISIONTYPE_COUNT,
	};
	
	enum TouchLayer
	{
		TOUCHLAYER_HUD,
		TOUCHLAYER_SCENE,
		TOUCHLAYER_BG,
		
		// used for counting
		TOUCHLAYER_COUNT,
	};
	
	struct TouchInput
	{
		uint32_t	id;
		
		float		timestamp_begin;
		float		timestamp;
		
		TouchEventType	state;
		int			tap_count;

		v2			pos; // current position
		v2			pos_prev; // previous movement (not previous frame)
		v2			pos_start; // on touch start
	};
	
	// users must register callbacks and provide collision data
	struct TouchListenerCallBack
	{		
		// callback: obj_this:callback(intersect_pt, callback_param)
		gmVariable callback_obj;
		gmVariable callback_func;
		gmVariable callback_param;
		
		TouchLayer		layer;
		TouchEventType	event;
		
		// collision
		union CollisionDataUnion
		{
			struct
			{
				v3 box_center;
				v3 box_dimen;
			};
			
			struct // Plane (Ax+By+Cz+D=0)
			{
				v4 plane;
			};
			
			struct // Sphere
			{
				v3 sphere_center;
				float sphere_radius;
			};
			
			struct // Disk
			{
				v3 disk_center;
				v3 disk_normal;
				float disk_radius;
			};
			
			struct // Triangle
			{
				v3 tri_pt0;
				v3 tri_pt1;
				v3 tri_pt2;
			};
			
			struct // Quad
			{
				v3 quad_pt0;
				v3 quad_pt1;
				v3 quad_pt2;
				v3 quad_pt3;
			};
			
			CollisionDataUnion(){}
		} collision_data;
		
		TouchCollisionType	collision_type;
		
		// no scaling!!
		matrix collision_model_matrix;
	};
}
