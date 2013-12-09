
#pragma once

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include <gfx/Texture2d.h>
#include <math/v2i.h>
#include <math/v2.h>

namespace funk
{
	class Fbo : public HandledObj<Fbo>
	{

	private:
		bool m_bound_as_target; // bound as render target
		v2i m_dimen;

		// GL ids
		unsigned int m_fbo_id;
		unsigned int m_fbo_depth_id;

		// render target slots
		static const int MAX_NUM_SLOTS = 8;
		StrongHandle<Texture2d> m_rt_texs[MAX_NUM_SLOTS];

	public:
		Fbo( int width, int height );
		~Fbo();

		// dimen
		int Width() const { return m_dimen.x; }
		int Height() const { return m_dimen.y; }
		v2i GetDimen() const { return m_dimen; }
		v2 GetDimenf() const { return v2((float)m_dimen.x, (float)m_dimen.y); }
		bool IsBound() const { return m_bound_as_target; }
		bool IsTexSlotOccupied( int slot = 0 ) const { return m_rt_texs[slot] != nullptr; }

		void Begin(bool a_set_viewport = true);
		void End();

		// make sure everything is OK
		void Validate();

		// generates default render targets
		// (or you can call SetRenderTargetTex(tex) to bind your own tex
		void GenerateDepthBuffer();
		StrongHandle<Texture2d> GenerateRenderTargetTex( int slot = 0, Texture2dDesc texParams = Texture2dDesc() );

		// render target slots
		void RemoveRenderTargetTex( int slot );
		bool IsSlotValid( int slot ) const { return m_rt_texs[slot].IsNull(); }
		void SetRenderTargetTex( StrongHandle<Texture2d> tex, int slot = 0 ); // can set to NULL to remove
        void SetRenderTargetTexNoBind( StrongHandle<Texture2d> tex, int slot = 0 ); // can set to NULL to remove
		StrongHandle<Texture2d> GetTex( int slot = 0 ) const { return m_rt_texs[slot]; }

		GM_BIND_TYPEID(Fbo);
		
	private:

		void BindFrameBuffer();
		void UnbindFrameBuffer();

	};

	GM_BIND_DECL(Fbo);
}
