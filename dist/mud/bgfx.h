#pragma once

#include <mud/ctx.wasm.h>
#include <mud/ctx.glfw.h>
#include <mud/ctx.win.h>
#include <mud/math.h>
#include <mud/ctx.h>
#include <mud/infra.h>
#include <mud/type.h>




#ifndef MUD_BGFX_EXPORT
#define MUD_BGFX_EXPORT MUD_IMPORT
#endif

namespace mud
{
    class BgfxContext;
    class BgfxSystem;
}


#include <stl/vector.h>

#ifndef MUD_MODULES
	#if defined MUD_CONTEXT_GLFW
	#elif defined MUD_CONTEXT_WASM
	#elif defined MUD_CONTEXT_WINDOWS
	#else
	#endif
#endif

namespace bx
{
	struct AllocatorI;
}

namespace mud
{
	class BgfxSystem;

#if defined MUD_CONTEXT_GLFW
	export_ class MUD_BGFX_EXPORT BgfxContext : public GlfwContext
#elif defined MUD_CONTEXT_WASM
	export_ class MUD_BGFX_EXPORT BgfxContext : public EmContext
#elif defined MUD_CONTEXT_WINDOWS
	export_ class MUD_BGFX_EXPORT BgfxContext : public WinContext
#else
	export_ class MUD_BGFX_EXPORT BgfxContext : public Context
#endif
	{
	public:
		BgfxContext(BgfxSystem& gfx, const string& name, const uvec2& size, bool fullscreen, bool main, bool init = true);

		virtual void render_frame() override;

		virtual void reset_fb(const uvec2& size) override;
	};

	export_ struct MUD_BGFX_EXPORT TimerBx
	{
		int64_t m_start = 0;

		void begin();
		float end();
	};

	export_ class refl_ MUD_BGFX_EXPORT BgfxSystem : public RenderSystem
	{
	public:
		BgfxSystem(const string& resource_path);
		~BgfxSystem();

		virtual bool begin_frame() override;
		virtual void end_frame() override;

		void init(BgfxContext& context);
		void advance();

		bx::AllocatorI& allocator();

	public:
		bool m_initialized = false;

		attr_ uint32_t m_frame = 1;
		double m_start_counter = 0.0;

		attr_ float m_time = 0.f;
		attr_ float m_frame_time = 0.f;
		attr_ float m_delta_time = 0.f;

		bool m_capture = false;
		size_t m_capture_every = 0;
	};
}


#include <stdint.h>
#include <stl/string.h>
#include <stl/vector.h>

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#endif

#ifndef MUD_MODULES
#endif


namespace mud
{
    // Exported types
    
    
    export_ template <> MUD_BGFX_EXPORT Type& type<mud::BgfxSystem>();
}
