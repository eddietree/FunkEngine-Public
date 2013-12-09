#include "TouchInputManager.h"
#include <common/Debug.h>
#include <vm/VirtualMachine.h>
#include <system/Window.h>

#include <math/MVP.h>
#include <math/Ray.h>
#include <gfx/ImmGfx.h>

#include <gm/gmMachine.h>
#include <gm/gmCall.h>

namespace funk
{
	 
//#define INPUT_PRINT_DEBUG(type)  TRACE(type": id(%d), pos:(%f,%f), prev:(%f,%f), tapcount:%d, state:%d, timestamp: %f\n", a_touch.id, a_touch.pos.x, a_touch.pos.y, a_touch.pos_prev.x, a_touch.pos_prev.y, a_touch.tap_count, a_touch.state, a_touch.timestamp );
#define INPUT_PRINT_DEBUG(type)
	
	// only want to copy certain properies
	void CopyTouchInput( const TouchInput & src, TouchInput & dst )
	{
		dst.timestamp		= src.timestamp;
		dst.state			= src.state;
		dst.tap_count		= src.tap_count;
		dst.pos				= src.pos;
		dst.pos_prev		= src.pos_prev;

		// don't copy these, because want to maintain from beginning
		//dst.timestamp_begin = src.timestamp_begin;
		//dst.pos_start	= src.pos_start;
	}
	
	TouchInputManager::TouchInputManager()
	{
		InitListeners();
		LOG("TouchInputManager initialized");
	}

	TouchInputManager::~TouchInputManager()
	{
	}
	
	gmVariable TouchInputManager::GmUpdateAndCallbackTouch( const char* a_func_name, const TouchInput & a_touch )
	{
		// get vm
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);
		
		// grab the vars
		gmVariable id_var = gmVariable( (int)a_touch.id );
		gmTableObject * touch_table = m_gm_touches_table->Get(id_var).GetTableObjectSafe();

		// fixes the bug where 
		// - you hold mouse down
		// - you can restart the VM, clears touches
		// - you release mouse, but now original touch event doesnt exist
		if ( touch_table == nullptr ) 
		{
			gmVariable null_var;
			null_var.Nullify();
			return null_var;
		}

		gmVariable table_obj = gmVariable(touch_table);
		
		// update table 
		touch_table->Set( vm, "id", id_var );
		touch_table->Set( vm, "timestamp", gmVariable(a_touch.timestamp) );
		touch_table->Set( vm, "timestamp_begin", gmVariable(a_touch.timestamp_begin) );
		touch_table->Set( vm, "state", gmVariable(a_touch.state) );
		touch_table->Set( vm, "tap_count", gmVariable(a_touch.tap_count) );
		touch_table->Set( vm, "pos", gmVariable(a_touch.pos) );
		touch_table->Set( vm, "pos_prev", gmVariable(a_touch.pos_prev) );

		// trigger function
		GmCallback(a_func_name, table_obj);
		
		return table_obj;
	}
	
	void TouchInputManager::GmCallback(const char* a_func_name, gmVariable & a_touch_table_param )
	{
		// get vm
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);
		
		// trigger function
		gmFunctionObject* func = m_gm_input_manager->Get(vm, a_func_name).GetFunctionObjectSafe();
		ASSERT(func);
		gmCall call;
		call.BeginFunction( vm, func, gmVariable(m_gm_input_manager) );
		call.AddParam( a_touch_table_param );
		call.End();
	}
	
	void TouchInputManager::OnEventTouchBegin( TouchInput a_touch )
	{
		// save initial timestamp
		a_touch.timestamp_begin = a_touch.timestamp_begin;
		
		// should not exist
		ASSERT( m_map_id_to_input.find( a_touch.id ) == m_map_id_to_input.end() );
	
		// add to map
		m_map_id_to_input.insert( std::make_pair(a_touch.id, a_touch) );
		
		// get vm
		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);

		// build a table for new touch
		gmVariable id_var = gmVariable( (int)a_touch.id );
		gmTableObject * touch_table = vm->AllocTableObject();
		touch_table->Set( vm, "id", id_var );
		touch_table->Set( vm, "timestamp", gmVariable(a_touch.timestamp) );
		touch_table->Set( vm, "timestamp_begin", gmVariable(a_touch.timestamp_begin) );
		touch_table->Set( vm, "state", gmVariable(a_touch.state) );
		touch_table->Set( vm, "tap_count", gmVariable(a_touch.tap_count) );
		touch_table->Set( vm, "pos", gmVariable(a_touch.pos) );
		touch_table->Set( vm, "pos_prev", gmVariable(a_touch.pos_prev) );
		touch_table->Set( vm, "pos_start", gmVariable(a_touch.pos_start) );
		
		// trigger function
		gmVariable table_obj = gmVariable(touch_table);
		GmCallback("OnEventTouchBegin", table_obj);
		HandleCollisionCallbacks( table_obj, a_touch, TOUCHEVENT_BEGIN );
		
		INPUT_PRINT_DEBUG("BEGIN");
	}
	
	void TouchInputManager::OnEventTouchMove( TouchInput a_touch )
	{
		// should exist
		auto it = m_map_id_to_input.find(a_touch.id);
		if ( it == m_map_id_to_input.end() ) return;

		TouchInput dst_touch = it->second;
		CopyTouchInput( a_touch, dst_touch );
		
		gmVariable touch_table_obj = GmUpdateAndCallbackTouch("OnEventTouchMove", dst_touch );
		HandleCollisionCallbacks( touch_table_obj, dst_touch, TOUCHEVENT_MOVE );
		
		INPUT_PRINT_DEBUG("MOVED");
	}
	
	void TouchInputManager::OnEventTouchEnd( TouchInput a_touch )
	{
		// should exist
		auto it = m_map_id_to_input.find(a_touch.id);
		TouchInput dst_touch = it->second;
		CopyTouchInput( a_touch, dst_touch );
		
		gmVariable touch_table_obj = GmUpdateAndCallbackTouch("OnEventTouchEnd", dst_touch );
		HandleCollisionCallbacks( touch_table_obj, dst_touch, TOUCHEVENT_END );
		
		// remove
		m_map_id_to_input.erase(it);

		INPUT_PRINT_DEBUG("ENDED");
	}
	
	void TouchInputManager::OnEventTouchCancel( TouchInput a_touch )
	{
		// should exist
		auto it = m_map_id_to_input.find(a_touch.id);
		TouchInput dst_touch = it->second;
		CopyTouchInput( a_touch, dst_touch );
		
		gmVariable touch_table_obj = GmUpdateAndCallbackTouch("OnEventTouchCancel", dst_touch );
		HandleCollisionCallbacks( touch_table_obj, dst_touch, TOUCHEVENT_CANCEL );
		
		// remove
		m_map_id_to_input.erase(it);
		
				
		INPUT_PRINT_DEBUG("CANCL");
	}
	
	void TouchInputManager::HandleCollisionCallbacks( gmVariable a_touch_table_obj, const TouchInput& a_touch, TouchEventType a_event_type  )
	{
		if ( a_touch_table_obj.IsNull() ) return;

		TouchEventListenerData & event = m_touch_event_listeners[a_event_type];
		
		// get the NDC
		v2 window_size = Window::Ref().Sizef();
		v2 ndc_xy	= (a_touch.pos / window_size)*2.0f - v2(1.0f);
		v4 ndc_near	= v4(ndc_xy.x, ndc_xy.y, -1.0f, 1.0f);
		v4 ndc_far = v4(ndc_xy.x, ndc_xy.y,  1.0f, 1.0f);
		
		// go thru each layer
		for( int i = 0; i < (int)event.layers.size(); ++i )
		{
			TouchLayerListenerData & layer = event.layers[i];
			
			// skip layer if no callbacks in this layer
			if ( layer.callbacks.empty() ) continue;
			
			
			// grab view and proj of camera
			const TouchLayerData & layer_data = m_layer_data[i];
			matrix mat_view_inv = layer_data.mat_view;
			matrix mat_proj_inv = layer_data.mat_proj_inv;
			
			// build_ray
			v3 ray_pos0 = (mat_view_inv * mat_proj_inv * ndc_near).xyz();
			v3 ray_pos1 = (mat_view_inv * mat_proj_inv * ndc_far ).xyz();
			Ray ray = Ray(ray_pos0, ray_pos1-ray_pos0 );
			
			// will save the interactions
			int index_closest = -1;
			v3 closest_intersection;
			float dist_to_closest_intersection_sqr = FLT_MAX;
			
			for( int j = 0; j < (int)layer.callbacks.size(); ++j )
			{
				TouchListenerCallBack & callback = layer.callbacks[j];
				
				// check to see if callback's collision data collides with ray
				bool collided = false;
				
				switch( callback.collision_type )
				{
					case COLLISIONTYPE_BOX:
					{
						Box3 box = Box3(callback.collision_data.box_dimen, callback.collision_data.box_center);
						collided = ray.IntersectBox( box, callback.collision_model_matrix );
						break;
					}
						
					case COLLISIONTYPE_SPHERE:
					{
						v3 sphere_center = callback.collision_data.sphere_center;
						v4 sphere_center_xformed = callback.collision_model_matrix * v4(sphere_center,1.0f);
						float sphere_radius = callback.collision_data.sphere_radius;
						collided = ray.IntersectSphere( sphere_center_xformed.xyz(), sphere_radius );
						break;
					}
					
					case COLLISIONTYPE_PLANE:
					{
						v4 plane_eq = callback.collision_data.plane;
						v3 pt_on_plane = v3(-plane_eq.w/plane_eq.x, 0.0f,0.0f);
						v3 pt_on_plane_xformed = (callback.collision_model_matrix*v4(pt_on_plane,1.0f)).xyz();
						v3 plane_normal_xformed = (callback.collision_model_matrix*v4(plane_eq.xyz(),0.0f)).xyz();
						v4 new_plane_eq = v4( plane_normal_xformed, -dot(pt_on_plane,pt_on_plane_xformed));

						collided = ray.IntersectPlane(new_plane_eq);
						break;
					}
					
					case COLLISIONTYPE_DISK:
					{
						v3 disk_center = (callback.collision_model_matrix*v4(callback.collision_data.disk_center,1.0f)).xyz();
						v3 disk_normal  = (callback.collision_model_matrix*v4(callback.collision_data.disk_normal,0.0f)).xyz();
						collided = ray.IntersectDisk( disk_center, callback.collision_data.disk_radius, disk_normal );
						break;
					}
					
					case COLLISIONTYPE_QUAD:
					{
						collided = ray.IntersectQuad( callback.collision_data.quad_pt0, callback.collision_data.quad_pt1, callback.collision_data.quad_pt2, callback.collision_data.quad_pt3, callback.collision_model_matrix );
						break;
					}
					
					case COLLISIONTYPE_TRIANGLE:
					{
						collided = ray.IntersectTri( callback.collision_data.tri_pt0, callback.collision_data.tri_pt1, callback.collision_data.tri_pt2, callback.collision_model_matrix );
						break;
					}
					
					default:
						ERROR_MSG("Unreconigzed collision type!");
						break;
				}
				
				// ray collided with object!!
				if ( collided ) 
				{
					v3 curr_intersection_pt = ray.GetHitPt();
					float dist_to_curr_intersection_sqr = lengthSqr(curr_intersection_pt-ray.GetBase() );
					
					if ( dist_to_curr_intersection_sqr < dist_to_closest_intersection_sqr )
					{
						index_closest = j;
						dist_to_closest_intersection_sqr = dist_to_curr_intersection_sqr;
						closest_intersection = curr_intersection_pt;
					}
				}
			}
			
			// if found one to collide with!
			// do the callback and early out!
			if ( index_closest != -1 )
			{
				// get vm
				gmMachine* vm = &VirtualMachine::Ref().GetVM();
				ASSERT(vm);
		
				TouchListenerCallBack & callback = layer.callbacks[index_closest];
				
				gmFunctionObject* func = callback.callback_func.GetFunctionObjectSafe();
				ASSERT(func);
				
				// do the function call!
				gmCall call;
				call.BeginFunction( vm, func, callback.callback_obj );
				call.AddParam( a_touch_table_obj );
				call.AddParam( gmVariable(closest_intersection) );
				call.AddParam( gmVariable(ray.GetHitNormal()) );
				call.AddParam( callback.callback_param );
				call.End();
		
				// early out
				return;
			}
		}
	}
	
	void TouchInputManager::RegisterListenTouchEvent( const TouchListenerCallBack & a_callback)
	{
		TouchEventListenerData & event = m_touch_event_listeners[a_callback.event];
		event.layers[a_callback.layer].callbacks.push_back(a_callback);
	}
	
	void TouchInputManager::ClearTouchListeners()
	{		
		for( size_t i = 0; i < m_touch_event_listeners.size(); ++i )
		{
			m_touch_event_listeners[i].layers.resize( TOUCHLAYER_COUNT );
			
			for( size_t j = 0; j < m_touch_event_listeners[i].layers.size(); ++j )
			{
				m_touch_event_listeners[i].layers[j].callbacks.clear();
			}
		}
	}

	void TouchInputManager::SetLayerCameraMatrices( TouchLayer a_layer, const matrix& a_mat_view, const matrix& a_mat_proj )
	{
		TouchLayerData & layer_data = m_layer_data[a_layer];
		layer_data.mat_view = a_mat_view;
		layer_data.mat_proj = a_mat_proj;
		
		layer_data.mat_view_inv = a_mat_view.inverseRT();
		layer_data.mat_proj_inv = a_mat_proj.inverse4x4();
	}
	
	void TouchInputManager::InitListeners()
	{
		m_touch_event_listeners.clear();
		m_layer_data.clear();

		m_touch_event_listeners.resize( TOUCHEVENT_COUNT );
		m_layer_data.resize( TOUCHLAYER_COUNT );
		
		const int reserve_callbacks_size = 64;
		for( size_t i = 0; i < m_touch_event_listeners.size(); ++i )
		{
			m_touch_event_listeners[i].layers.resize( TOUCHLAYER_COUNT );
			
			for( size_t j = 0; j < m_touch_event_listeners[i].layers.size(); ++j )
			{
				m_touch_event_listeners[i].layers[j].callbacks.reserve( reserve_callbacks_size );
			}
		}
	}
	
	void TouchInputManager::InitGM()
	{
		ClearTouchListeners();

		gmMachine* vm = &VirtualMachine::Ref().GetVM();
		ASSERT(vm);
		
		// grab the touches table
		gmVariable global_obj_key = gmVariable(vm->AllocStringObject("g_touchInput"));
		m_gm_input_manager = vm->GetGlobals()->Get(global_obj_key).GetTableObjectSafe();
		ASSERT( m_gm_input_manager );
		
		m_gm_touches_table = m_gm_input_manager->Get(vm, "touches" ).GetTableObjectSafe();
		ASSERT( m_gm_touches_table );
	}
	
	void TouchInputManager::DrawDebug()
	{
		MVP & mvp = MVP::Ref();
		ImmGfx & imm = ImmGfx::Ref();
		
		imm.BeginDraw();
		imm.BeginDefaultShader();
	
		for ( size_t i = 0; i < m_touch_event_listeners.size(); ++i )
		{
			const TouchEventListenerData & event_data = m_touch_event_listeners[i];
		
			// go thru each layer
			for ( size_t j = 0; j < event_data.layers.size(); ++j )
			{
				const TouchLayerListenerData & layer_data = event_data.layers[j];
				
				mvp.View().Push();
				mvp.Proj().Push();
				mvp.View().Load( m_layer_data[j].mat_view );
				mvp.Proj().Load( m_layer_data[j].mat_proj );
				mvp.Model().Push();
				
				for ( size_t k = 0; k < layer_data.callbacks.size(); ++k )
				{
					const TouchListenerCallBack & callback = layer_data.callbacks[k];
					
					mvp.Model().Load( callback.collision_model_matrix );
					imm.SetDefaultShaderMVP();
					
					switch( callback.collision_type )
					{
						case COLLISIONTYPE_BOX:
						{
							Box3 box = Box3(callback.collision_data.box_dimen, callback.collision_data.box_center);
							imm.DrawCubeWire( box.Size(), box.GetCenter() );
							break;
						}
							
						case COLLISIONTYPE_PLANE:
							// TODO: ?
							break;
							
						case COLLISIONTYPE_SPHERE:
							//imm.DrawSphereWire( callback.collision_data.sphere_center, callback.collision_data.sphere_radius );
							break;
						
						case COLLISIONTYPE_TRIANGLE:
							imm.DrawTriangleWire(  callback.collision_data.tri_pt0, callback.collision_data.tri_pt1, callback.collision_data.tri_pt2 );
							break;
							
						case COLLISIONTYPE_DISK:
							imm.DrawDiskWire( callback.collision_data.disk_center, callback.collision_data.disk_normal, callback.collision_data.disk_radius );
							break;
							
						case COLLISIONTYPE_QUAD:
							imm.PolygonBegin( PRIM_LINE_LOOP );
							imm.Vertex( callback.collision_data.quad_pt0 );
							imm.Vertex( callback.collision_data.quad_pt1 );
							imm.Vertex( callback.collision_data.quad_pt2 );
							imm.Vertex( callback.collision_data.quad_pt3 );
							imm.PolygonEnd();
							break;
								
						default:
							break;
					}
				}
				
				mvp.Model().Pop();
				mvp.View().Pop();
				mvp.Proj().Pop();
			}
		}
		
		imm.EndDefaultShader();
		imm.EndDraw();
		
	};
}