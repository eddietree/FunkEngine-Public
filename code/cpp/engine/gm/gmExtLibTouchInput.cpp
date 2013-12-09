#include "gmExtLibTouchInput.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"

#include <math/MVP.h>
#include <gm/gmBind.h>

#include <system/TouchInputManager.h>

namespace funk
{
struct gmfTouchInputLib
{	
	GM_MEMFUNC_DECL(RegisterBox)
	{
		ASSERT( a_thread->GetNumParams() >= 5 );
		
		gmVariable null_var;
		null_var.Nullify();
		
		// get func params
		GM_CHECK_INT_PARAM( a_event_type, 0 );
		GM_CHECK_INT_PARAM( a_layer, 1);
		GM_CHECK_V3_PARAM( a_box_dimen, 2);
		GM_CHECK_V3_PARAM( a_box_center, 3);
		
		gmVariable& a_func_callback = a_thread->Param(4);
		const gmVariable& a_func_obj = a_thread->Param(5, null_var );
		const gmVariable& a_func_param = a_thread->Param(6, null_var );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		// build callback
		TouchListenerCallBack callback_data;
		callback_data.callback_obj = a_func_obj;
		callback_data.callback_func = a_func_callback;
		callback_data.callback_param = a_func_param;
		callback_data.layer = (TouchLayer)a_layer;
		callback_data.event = (TouchEventType)a_event_type;
		callback_data.collision_type = COLLISIONTYPE_BOX;
		callback_data.collision_data.box_center = a_box_center;
		callback_data.collision_data.box_center = a_box_dimen;
		callback_data.collision_model_matrix = MVP::Ref().Model().Get();
		
		TouchInputManager::Ref().RegisterListenTouchEvent(callback_data);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(RegisterPlane)
	{
		ASSERT( a_thread->GetNumParams() >= 5 );
		
		gmVariable null_var;
		null_var.Nullify();
		
		// get func params
		GM_CHECK_INT_PARAM( a_event_type, 0 );
		GM_CHECK_INT_PARAM( a_layer, 1);
		GM_CHECK_V3_PARAM( a_plane_normal, 2);
		GM_CHECK_FLOAT_PARAM( a_plane_d, 3);
		
		gmVariable& a_func_callback = a_thread->Param(4);
		const gmVariable& a_func_obj = a_thread->Param(5, null_var );
		const gmVariable& a_func_param = a_thread->Param(6, null_var );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		// build callback
		TouchListenerCallBack callback_data;
		callback_data.callback_obj = a_func_obj;
		callback_data.callback_func = a_func_callback;
		callback_data.callback_param = a_func_param;
		callback_data.layer = (TouchLayer)a_layer;
		callback_data.event = (TouchEventType)a_event_type;
		callback_data.collision_type = COLLISIONTYPE_PLANE;
		callback_data.collision_data.plane = v4(a_plane_normal, a_plane_d);
		callback_data.collision_model_matrix = MVP::Ref().Model().Get();
		
		TouchInputManager::Ref().RegisterListenTouchEvent(callback_data);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(RegisterDisk)
	{
		ASSERT( a_thread->GetNumParams() >= 6 );
		
		gmVariable null_var;
		null_var.Nullify();
		
		// get func params
		GM_CHECK_INT_PARAM( a_event_type, 0 );
		GM_CHECK_INT_PARAM( a_layer, 1);
		GM_CHECK_V3_PARAM( a_disk_center, 2);
		GM_CHECK_V3_PARAM( a_disk_normal, 3);
		GM_CHECK_FLOAT_PARAM( a_disk_radius, 4);
		
		gmVariable& a_func_callback = a_thread->Param(5);
		const gmVariable& a_func_obj = a_thread->Param(6, null_var );
		const gmVariable& a_func_param = a_thread->Param(7, null_var );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		// build callback
		TouchListenerCallBack callback_data;
		callback_data.callback_obj = a_func_obj;
		callback_data.callback_func = a_func_callback;
		callback_data.callback_param = a_func_param;
		callback_data.layer = (TouchLayer)a_layer;
		callback_data.event = (TouchEventType)a_event_type;
		callback_data.collision_type = COLLISIONTYPE_DISK;
		callback_data.collision_data.disk_center = a_disk_center;
		callback_data.collision_data.disk_normal = a_disk_normal;
		callback_data.collision_data.disk_radius = a_disk_radius;
		callback_data.collision_model_matrix = MVP::Ref().Model().Get();
		
		TouchInputManager::Ref().RegisterListenTouchEvent(callback_data);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(RegisterQuad)
	{
		ASSERT( a_thread->GetNumParams() >= 6 );
		
		gmVariable null_var;
		null_var.Nullify();
		
		// get func params
		GM_CHECK_INT_PARAM( a_event_type, 0 );
		GM_CHECK_INT_PARAM( a_layer, 1);
		GM_CHECK_V3_PARAM( a_quad_pt0, 2);
		GM_CHECK_V3_PARAM( a_quad_pt1, 3);
		GM_CHECK_V3_PARAM( a_quad_pt2, 4);
		GM_CHECK_V3_PARAM( a_quad_pt3, 5);
		
		gmVariable& a_func_callback = a_thread->Param(6);
		const gmVariable& a_func_obj = a_thread->Param(7, null_var );
		const gmVariable& a_func_param = a_thread->Param(8, null_var );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		// build callback
		TouchListenerCallBack callback_data;
		callback_data.callback_obj = a_func_obj;
		callback_data.callback_func = a_func_callback;
		callback_data.callback_param = a_func_param;
		callback_data.layer = (TouchLayer)a_layer;
		callback_data.event = (TouchEventType)a_event_type;
		callback_data.collision_type = COLLISIONTYPE_QUAD;
		callback_data.collision_data.quad_pt0 = a_quad_pt0;
		callback_data.collision_data.quad_pt1 = a_quad_pt1;
		callback_data.collision_data.quad_pt2 = a_quad_pt2;
		callback_data.collision_data.quad_pt3 = a_quad_pt3;
		callback_data.collision_model_matrix = MVP::Ref().Model().Get();
		
		TouchInputManager::Ref().RegisterListenTouchEvent(callback_data);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(RegisterQuad2d)
	{
		ASSERT( a_thread->GetNumParams() >= 6 );
		
		gmVariable null_var;
		null_var.Nullify();
		
		// get func params
		GM_CHECK_INT_PARAM( a_event_type, 0 );
		GM_CHECK_INT_PARAM( a_layer, 1);
		GM_CHECK_V2_PARAM( a_quad_pt0, 2);
		GM_CHECK_V2_PARAM( a_quad_pt1, 3);
		GM_CHECK_V2_PARAM( a_quad_pt2, 4);
		GM_CHECK_V2_PARAM( a_quad_pt3, 5);
		
		gmVariable& a_func_callback = a_thread->Param(6);
		const gmVariable& a_func_obj = a_thread->Param(7, null_var );
		const gmVariable& a_func_param = a_thread->Param(8, null_var );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		// build callback
		TouchListenerCallBack callback_data;
		callback_data.callback_obj = a_func_obj;
		callback_data.callback_func = a_func_callback;
		callback_data.callback_param = a_func_param;
		callback_data.layer = (TouchLayer)a_layer;
		callback_data.event = (TouchEventType)a_event_type;
		callback_data.collision_type = COLLISIONTYPE_QUAD;
		callback_data.collision_data.quad_pt0 = v3(a_quad_pt0.x, a_quad_pt0.y, 0.0f);
		callback_data.collision_data.quad_pt1 = v3(a_quad_pt1.x, a_quad_pt1.y, 0.0f);
		callback_data.collision_data.quad_pt2 = v3(a_quad_pt2.x, a_quad_pt2.y, 0.0f);
		callback_data.collision_data.quad_pt3 = v3(a_quad_pt3.x, a_quad_pt3.y, 0.0f);
		callback_data.collision_model_matrix = MVP::Ref().Model().Get();
		
		TouchInputManager::Ref().RegisterListenTouchEvent(callback_data);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(RegisterTri)
	{
		ASSERT( a_thread->GetNumParams() >= 6 );
		
		gmVariable null_var;
		null_var.Nullify();
		
		// get func params
		GM_CHECK_INT_PARAM( a_event_type, 0 );
		GM_CHECK_INT_PARAM( a_layer, 1);
		GM_CHECK_V3_PARAM( a_tri_pt0, 2);
		GM_CHECK_V3_PARAM( a_tri_pt1, 3);
		GM_CHECK_V3_PARAM( a_tri_pt2, 4);
		
		gmVariable& a_func_callback = a_thread->Param(5);
		const gmVariable& a_func_obj = a_thread->Param(6, null_var );
		const gmVariable& a_func_param = a_thread->Param(7, null_var );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		// build callback
		TouchListenerCallBack callback_data;
		callback_data.callback_obj = a_func_obj;
		callback_data.callback_func = a_func_callback;
		callback_data.callback_param = a_func_param;
		callback_data.layer = (TouchLayer)a_layer;
		callback_data.event = (TouchEventType)a_event_type;
		callback_data.collision_type = COLLISIONTYPE_TRIANGLE;
		callback_data.collision_data.tri_pt0 = a_tri_pt0;
		callback_data.collision_data.tri_pt1 = a_tri_pt1;
		callback_data.collision_data.tri_pt2 = a_tri_pt2;
		callback_data.collision_model_matrix = MVP::Ref().Model().Get();
		
		TouchInputManager::Ref().RegisterListenTouchEvent(callback_data);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(RegisterSphere)
	{
		ASSERT( a_thread->GetNumParams() >= 5 );
		
		gmVariable null_var;
		null_var.Nullify();
		
		// get func params
		GM_CHECK_INT_PARAM( a_event_type, 0 );
		GM_CHECK_INT_PARAM( a_layer, 1);
		GM_CHECK_V3_PARAM( a_sphere_center, 2);
		GM_CHECK_FLOAT_PARAM( a_sphere_radius, 3);
		
		gmVariable& a_func_callback = a_thread->Param(4);
		const gmVariable& a_func_obj = a_thread->Param(5, null_var );
		const gmVariable& a_func_param = a_thread->Param(6, null_var );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		// build callback
		TouchListenerCallBack callback_data;
		callback_data.callback_obj = a_func_obj;
		callback_data.callback_func = a_func_callback;
		callback_data.callback_param = a_func_param;
		callback_data.layer = (TouchLayer)a_layer;
		callback_data.event = (TouchEventType)a_event_type;
		callback_data.collision_type = COLLISIONTYPE_SPHERE;
		callback_data.collision_data.sphere_center = a_sphere_center;
		callback_data.collision_data.sphere_radius = a_sphere_radius;
		callback_data.collision_model_matrix = MVP::Ref().Model().Get();
		
		TouchInputManager::Ref().RegisterListenTouchEvent(callback_data);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(CopyCameraToLayer)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_layer, 0 );
		
		ASSERT( a_layer < TOUCHLAYER_COUNT );
		
		TouchInputManager::Ref().SetLayerCameraMatrices(
			(TouchLayer)a_layer,
			MVP::Ref().View().Get(),
			MVP::Ref().Proj().Get()
		);
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawDebug)
	{
		GM_CHECK_NUM_PARAMS(0);
		TouchInputManager::Ref().DrawDebug();
		return GM_OK;
	}
};

static gmFunctionEntry s_gmTouchInputLib[] =
{
	// register collision primitives
	GM_LIBFUNC_ENTRY(RegisterBox, TouchInput)
	GM_LIBFUNC_ENTRY(RegisterSphere, TouchInput)
	GM_LIBFUNC_ENTRY(RegisterPlane, TouchInput)
	GM_LIBFUNC_ENTRY(RegisterTri, TouchInput)
	GM_LIBFUNC_ENTRY(RegisterDisk, TouchInput)
	GM_LIBFUNC_ENTRY(RegisterQuad, TouchInput)
	GM_LIBFUNC_ENTRY(RegisterQuad2d, TouchInput)
	
	// copies current camera to layer
	GM_LIBFUNC_ENTRY(CopyCameraToLayer, TouchInput)
	
	GM_LIBFUNC_ENTRY(DrawDebug, TouchInput)
};

void gmBindTouchInputLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmTouchInputLib, sizeof(s_gmTouchInputLib) / sizeof(s_gmTouchInputLib[0]), "TouchInput" );
}

}