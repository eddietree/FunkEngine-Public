
#pragma once

#include <common/Singleton.h>
#include <gm/gmVariable.h>
#include "TouchDefines.h"

#include <unordered_map>
#include <vector>
#include <stdint.h>

namespace funk
{	
	class TouchInputManager : public Singleton<TouchInputManager>
	{
	private:
 
		struct TouchLayerData
		{
			// view/proj matrix used by the camera..
			matrix mat_view;
			matrix mat_proj;
			
			matrix mat_view_inv;
			matrix mat_proj_inv;
		};
		
		struct TouchLayerListenerData
		{
			// each layer has a list of callbacks
			std::vector<TouchListenerCallBack> callbacks;
		};
      
		// each event has a list of layers
		struct TouchEventListenerData
		{		
			// event holds a list of layers
			std::vector<TouchLayerListenerData> layers;
		};
		
		
		gmTableObject * m_gm_touches_table;
		gmTableObject * m_gm_input_manager;
		
		// maps TouchInput.id -> TouchInput
		std::unordered_map<uint32_t, TouchInput> m_map_id_to_input;
		
		// all the event listeners
		std::vector<TouchEventListenerData> m_touch_event_listeners;
		std::vector<TouchLayerData> m_layer_data;

	public:
		
		void InitGM();
		
		// called by ViewController
		void OnEventTouchBegin( TouchInput a_touch );
		void OnEventTouchMove( TouchInput a_touch );
		void OnEventTouchEnd( TouchInput a_touch );
		void OnEventTouchCancel( TouchInput a_touch );
		
		// for listening to events
		void RegisterListenTouchEvent( const TouchListenerCallBack & a_callback );
		void ClearTouchListeners();
		void SetLayerCameraMatrices( TouchLayer a_layer, const matrix& a_mat_view, const matrix& a_mat_proj );

		void DrawDebug();
		
		const std::unordered_map<uint32_t, TouchInput> & GetTouchMap() const { return m_map_id_to_input; }
		
	private:
		
		void InitListeners();
		void HandleCollisionCallbacks( gmVariable a_touch_table_obj, const TouchInput& a_touch, TouchEventType a_event_type  );
		
		gmVariable GmUpdateAndCallbackTouch( const char* a_func_name, const TouchInput & a_touch );
		void GmCallback(const char* a_func_name, gmVariable & a_touch_table_param );
		
		friend Singleton<TouchInputManager>;
		TouchInputManager();
		~TouchInputManager();
	};
}
