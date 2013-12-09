#include "VirtualMachineLibs.h"

#include <gm/gmMachine.h>
#include <gm/gmBind.h>

#include <gm/gmSystemLib.h>
#include <gm/gmMathLib.h>
#include <gm/gmArrayLib.h>
#include <gm/gmStringLib.h>
#include <gm/gmDebug.h>

#include <gm/gmExtLibDebug.h>
#include <gm/gmExtLibInput.h>
#include <gm/gmExtLibGfx.h>
#include <gm/gmExtLibWindow.h>
#include <gm/gmExtLibMVP.h>
#include <gm/gmExtLibImmGfx.h>
#include <gm/gmExtLibTouchInput.h>
#include <gm/gmExtLibInput.h>
#include <gm/gmExtLibAudio.h>
#include <gm/gmExtLibFmod.h>
#include <gm/gmExtLibProfiler.h>

#include <math/Cam2d.h>
#include <math/Cam3d.h>
#include <math/Perlin2d.h>
#include <math/CurlNoise.h>
#include <gfx/ShaderProgram.h>
#include <gfx/Vbo.h>
#include <gfx/VboBuilder.h>
#include <gfx/Vao.h>
#include <gfx/Ibo.h>
#include <gfx/Texture2d.h>
#include <gfx/Font.h>
#include <gfx/LineGraph.h>
#include <gfx/Fbo.h>
#include <gfx/SpriteBatchDrawer.h>
#include <imgui/ImguiGM.h>
#include <imgui/ImguiTilesheetChunks.h>
#include <audio/SoundBuffer.h>
#include <audio/SoundChannel.h>
#include <physics/Verlet2DBody.h>
#include <physics/Verlet3DBody.h>

#ifdef ENABLE_OSC
#include <ext/osc/OscHandler.h>
#endif // ENABLE_OSC

// fmod
#include <audio/fmod/FmodEventDesc.h>
#include <audio/fmod/FmodEventInst.h>

//  Game specific
#include "../../game/blank_game/gmBlankGameInit.h"

namespace funk
{

void RegisterLibs( gmMachine * vm )
{
	// Init libraries
	gmBindSystemLib(vm);
	gmBindMathLib(vm);
	gmBindArrayLib(vm);
	gmBindStringLib(vm);
	gmBindDebugLib(vm);
	
	// custom libs
	gmBindGfxLib(vm);
	gmBindMVPLib(vm);
	gmBindWindowLib(vm);
	gmBindImmGfxLib(vm);
	gmBindTouchInputLib(vm);
	gmBindImguiLib(vm);
	gmBindInputLib(vm);
	gmBindAudioLib(vm);
	gmBindFunkDebugLib(vm);	
	gmBindProfilerLib(vm);	
	
	// Init common custom user types
	GM_BIND_INIT( Cam2d, vm );
	GM_BIND_INIT( Cam3d, vm );
	GM_BIND_INIT( Perlin2d, vm );
	GM_BIND_INIT( CurlNoise, vm );
	GM_BIND_INIT( ShaderProgram, vm );
	GM_BIND_INIT( Vbo, vm );
	GM_BIND_INIT( VboBuilder, vm );
	GM_BIND_INIT( Vao, vm );
	GM_BIND_INIT( Ibo, vm );
	GM_BIND_INIT( Texture2d, vm );
	GM_BIND_INIT( Font, vm );
	GM_BIND_INIT( LineGraph, vm );
	GM_BIND_INIT( Fbo, vm );
	GM_BIND_INIT( SpriteBatchDrawer, vm );
	GM_BIND_INIT( SoundBuffer, vm );
	GM_BIND_INIT( SoundChannel, vm );
	GM_BIND_INIT( Verlet2DBody, vm );
	GM_BIND_INIT( Verlet3DBody, vm );
	GM_BIND_INIT( ImguiTilesheetChunks, vm );

	#if defined(FMOD_ENABLED)
	gmBindFmodLib(vm);
	GM_BIND_INIT( FmodEventDesc, vm );
	GM_BIND_INIT( FmodEventInst, vm );
	#endif

	#ifdef ENABLE_OSC
	GM_BIND_INIT( OscHandler, vm );
	#endif // ENABLE_OSC
	
	// Init game-specific types
	blank_game::InitGameLibs(vm);
}
}