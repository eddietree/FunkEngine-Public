#include "Fbo.h"

#include <common/Debug.h>
#include <system/Window.h>

#include <gm/gmBind.h>
#include <gm/gmBindFuncGen.h>

#include <gfx/GLDebug.h>

#include "GLIncludes.h"
#include "RenderState.h"

namespace funk
{
Fbo::Fbo( int width, int height ) 
: m_dimen(width,height), m_fbo_id(0), 
m_fbo_depth_id(0),  m_bound_as_target(false)
{
	glGenFramebuffers(1, &m_fbo_id); // Generate one frame buffer and store the ID in fbo
	ASSERT( m_fbo_id != 0 );

	CheckGLError();
}

Fbo::~Fbo()
{
	CHECK( !m_bound_as_target, "Deleting FBO that is currently bound!" );

	if ( m_fbo_id ) glDeleteFramebuffers( 1, &m_fbo_id );
	if ( m_fbo_depth_id ) glDeleteRenderbuffers( 1, &m_fbo_depth_id );
}

void Fbo::GenerateDepthBuffer()
{
	LOG("Fbo::GenerateDepthBuffer()");

	// attach depth to fbo
	LOG("\tBindFrameBuffer()");
	BindFrameBuffer();

	LOG("\tglGenRenderbuffers()");
	glGenRenderbuffers(1, &m_fbo_depth_id); // Generate one render buffer and store the ID in fbo_depth  
	ASSERT(m_fbo_depth_id > 0);
	LOG("\tglBindRenderbuffer");
	glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_depth_id); // Bind the fbo_depth render buffer  
	LOG("\tglRenderbufferStorage");
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_dimen.x, m_dimen.y); // Set the render buffer storage to be a depth component, with a width and height of the window
	LOG("\tglBindRenderbuffer");
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind the render buffer 
	LOG("\tDepth buffer created");

	CheckGLError();
	
	LOG("\tglFramebufferRenderbuffer");
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fbo_depth_id); // Attach the depth buffer fbo_depth to our frame buffer
	
	LOG("\tUnbindFrameBuffer");
	UnbindFrameBuffer();
}

void Fbo::Validate()
{
	ASSERT( glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE );
}

void Fbo::RemoveRenderTargetTex( int slot )
{
	ASSERT(slot >= 0 && slot < MAX_NUM_SLOTS);

	// unbind from fbo
	BindFrameBuffer();
	unsigned int GLcolorAttachmentSlot = GL_COLOR_ATTACHMENT0 + slot;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GLcolorAttachmentSlot, GL_TEXTURE_2D, 0, 0);
	UnbindFrameBuffer();

	m_rt_texs[slot] = nullptr;
}

StrongHandle<Texture2d> Fbo::GenerateRenderTargetTex( int slot, Texture2dDesc texParams )
{
	ASSERT(slot >= 0 && slot < MAX_NUM_SLOTS);
	
	if ( texParams.dimen.x == 0 ) { texParams.dimen.x = Width(); }
	if ( texParams.dimen.y == 0 ) { texParams.dimen.y = Height(); }

	StrongHandle<Texture2d> tex = new Texture2d;
	tex->Init(texParams);
	SetRenderTargetTex(tex, slot);

	return tex;
}

void Fbo::SetRenderTargetTex( StrongHandle<Texture2d> tex, int slot )
{
	CHECK( !m_bound_as_target, "Cannot bind texture. Fbo currently bound!");

	BindFrameBuffer();
    SetRenderTargetTexNoBind(tex, slot);
	UnbindFrameBuffer();
}
    
void Fbo::SetRenderTargetTexNoBind( StrongHandle<Texture2d> tex, int slot )
{
    ASSERT( tex != nullptr );
	ASSERT( slot >= 0 && slot < MAX_NUM_SLOTS );
    
	unsigned int GLcolorAttachmentSlot = GL_COLOR_ATTACHMENT0 + slot;
	int tex_id = tex->Id();
	m_rt_texs[slot] = tex;
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GLcolorAttachmentSlot, GL_TEXTURE_2D, tex_id, 0);
}

void Fbo::BindFrameBuffer()
{
	CHECK( !m_bound_as_target, "Fbo currently bound!");
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	m_bound_as_target = true;
}

void Fbo::UnbindFrameBuffer()
{
	CHECK( m_bound_as_target, "Fbo not bound!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_bound_as_target = false;
}

void Fbo::Begin(bool a_set_viewport)
{
	BindFrameBuffer();

	// set viewport as FBO tex size
    if (a_set_viewport)	RenderState::Ref().SetViewport( 0, 0, Width(), Height() );
}

void Fbo::End()
{
	UnbindFrameBuffer();

	// set viewport to screen
	//v2i dimen =  Window::Get()->Sizei();
	//RenderState::Ref().SetViewport( 0, 0, dimen.x, dimen.y );
}

GM_REG_NAMESPACE(Fbo)
{
	GM_MEMFUNC_CONSTRUCTOR(Fbo)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_FLOAT_OR_INT_PARAM(width, 0);
		GM_CHECK_FLOAT_OR_INT_PARAM(height, 1);

		StrongHandle<Fbo> p = new Fbo((int)width, (int)height);
		GM_PUSH_USER_HANDLED( Fbo, p.Get() );
		return GM_OK;
	}
    
    GM_MEMFUNC_DECL(Begin)
	{
		GM_INT_PARAM( a_set_viewport, 0, 1 );
		GM_GET_THIS_PTR(Fbo, ptr);
        
		ptr->Begin(a_set_viewport!=0?true:false);
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetTex)
	{
		GM_INT_PARAM( slot, 0, 0 );
		GM_GET_THIS_PTR(Fbo, ptr);

		// return texture if exists
		if ( ptr->IsSlotValid(slot) ) a_thread->PushNull();	
		else GM_PUSH_USER_HANDLED( Texture2d, ptr->GetTex(slot).Get() );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetRenderTargetTex)
	{
		GM_CHECK_USER_PARAM_PTR( Texture2d, tex, 0 );
		GM_INT_PARAM( slot, 1, 0 );
		GM_GET_THIS_PTR(Fbo, ptr);

		ptr->SetRenderTargetTex(tex, slot);
		
		return GM_OK;
	}
    
    GM_MEMFUNC_DECL(SetRenderTargetTexNoBind)
	{
		GM_CHECK_USER_PARAM_PTR( Texture2d, tex, 0 );
		GM_INT_PARAM( slot, 1, 0 );
		GM_GET_THIS_PTR(Fbo, ptr);
        
		ptr->SetRenderTargetTexNoBind(tex, slot);
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GenerateRenderTargetTex)
	{
		GM_CHECK_NUM_PARAMS_RANGE(0, 6);

		// args
		GM_INT_PARAM( a_slot, 0, 0 );
		GM_INT_PARAM( a_width, 1, 0); // if width==0, will use default size
		GM_INT_PARAM( a_height, 2, 0); // if height==0, will use default size
		GM_INT_PARAM( a_internal_format, 3, TEXFORMAT_INTERNAL_RGBA8 );
		GM_INT_PARAM( a_pixel_format, 4, TEXFORMAT_RGBA );
		GM_INT_PARAM( a_data_type, 5, DATATYPE_UNSIGNED_BYTE );
		
		// create texture and bind to slot
		GM_GET_THIS_PTR(Fbo, ptr);

		Texture2dDesc desc;
		desc.dimen = v2i( (int)a_width, (int)a_height );
		desc.internal_format = (GLTextureInternalFormat)a_internal_format;
		desc.format = (GLTextureFormat)a_pixel_format;
		desc.data_type = (GLDataType)a_data_type;

		StrongHandle<Texture2d> tex = ptr->GenerateRenderTargetTex( a_slot, desc );
		GM_PUSH_USER_HANDLED( Texture2d, tex.Get() );

		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID( Fbo, End )
	GM_GEN_MEMFUNC_VOID_VOID( Fbo, Validate )
	GM_GEN_MEMFUNC_INT_VOID( Fbo, Width )
	GM_GEN_MEMFUNC_INT_VOID( Fbo, Height)
	GM_GEN_MEMFUNC_V2_VOID( Fbo, GetDimenf)
	GM_GEN_MEMFUNC_VOID_INT( Fbo, RemoveRenderTargetTex )
	GM_GEN_MEMFUNC_VOID_VOID( Fbo, GenerateDepthBuffer )
	GM_GEN_MEMFUNC_BOOL_VOID( Fbo, IsBound )
	GM_GEN_MEMFUNC_BOOL_INT( Fbo, IsTexSlotOccupied )
}

GM_REG_MEM_BEGIN(Fbo)
GM_REG_MEMFUNC( Fbo, Begin )
GM_REG_MEMFUNC( Fbo, End )
GM_REG_MEMFUNC( Fbo, Validate )
GM_REG_MEMFUNC( Fbo, Width )
GM_REG_MEMFUNC( Fbo, Height )
GM_REG_MEMFUNC( Fbo, GetDimenf )
GM_REG_MEMFUNC( Fbo, GetTex )
GM_REG_MEMFUNC( Fbo, IsBound )
GM_REG_MEMFUNC( Fbo, IsBound )
GM_REG_MEMFUNC( Fbo, SetRenderTargetTex )
GM_REG_MEMFUNC( Fbo, SetRenderTargetTexNoBind )
GM_REG_MEMFUNC( Fbo, GenerateRenderTargetTex )
GM_REG_MEMFUNC( Fbo, GenerateDepthBuffer )
GM_REG_MEMFUNC( Fbo, RemoveRenderTargetTex )
GM_REG_HANDLED_DESTRUCTORS(Fbo)
GM_REG_MEM_END()
GM_BIND_DEFINE(Fbo)

}