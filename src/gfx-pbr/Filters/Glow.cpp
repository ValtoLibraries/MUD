//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <gfx/Cpp20.h>

#include <bgfx/bgfx.h>

#ifdef MUD_MODULES
module mud.gfx.pbr;
#else
#include <ecs/ECS.hpp>
#include <math/Vec.hpp>
#include <gfx/RenderTarget.h>
#include <gfx/Filter.h>
#include <gfx/Pipeline.h>
#include <gfx/Asset.h>
#include <gfx/GfxSystem.h>
#include <gfx-pbr/Types.h>
#include <gfx-pbr/Handles.h>
#include <gfx-pbr/Filters/Glow.h>
#include <gfx-pbr/Filters/Blur.h>
#include <gfx-pbr/Filters/Tonemap.h>
#include <gfx-pbr/Gpu/Glow.hpp>
#endif

#define MAX_GLOW_LEVELS 7

namespace mud
{
	GpuState<Glow> GpuState<Glow>::me;

	BlockGlow::BlockGlow(GfxSystem& gfx, BlockFilter& filter, BlockCopy& copy, BlockBlur& blur)
		: GfxBlock(gfx, *this)
		, m_filter(filter)
		, m_copy(copy)
		, m_blur(blur)
		, m_bleed_program(gfx.programs().create("filter/glow_bleed"))
		, m_merge_program(gfx.programs().create("filter/glow"))
	{
		m_shader_block.m_options = { "GLOW_FILTER_BICUBIC" };

		m_blur.m_program.register_block(this->m_shader_block);
		m_merge_program.register_block(this->m_shader_block);
	}

	void BlockGlow::init_block()
	{
		GpuState<Glow>::me.init();
	}

	void BlockGlow::begin_render(Render& render)
	{
		UNUSED(render);
#ifdef DEBUG_GLOW
		this->debug_glow(render, *render.m_target);
#endif
	}

	void BlockGlow::debug_glow(Render& render, RenderTarget& target)
	{
		m_gfx.m_copy->debug_show_texture(render, target.m_cascade.m_texture, vec4(0.f), 1);
		m_gfx.m_copy->debug_show_texture(render, target.m_ping_pong.last(), vec4(0.f));
	}

	void BlockGlow::submit_pass(Render& render)
	{
		if(render.m_filters.comp<Glow>().m_enabled)
			this->render(render, render.m_filters.comp<Glow>());
	}

	void BlockGlow::render(Render& render, Glow& glow)
	{
		this->glow(render, *render.m_target, glow);
	}

	void BlockGlow::glow(Render& render, RenderTarget& target, Glow& glow)
	{
		this->glow_bleed(render, target, glow);
		this->glow_blur(render, target, glow);
		this->glow_merge(render, target, glow);
	}

	void BlockGlow::glow_bleed(Render& render, RenderTarget& target, Glow& glow)
	{
		m_filter.source0(target.m_diffuse);
		//bgfx::setUniform(m_blur.u_uniform.u_exposure, &m_tonemap.m_exposure);

		GpuState<Glow>::me.upload(glow);

		Pass pass = render.composite_pass("glow bleed");
		m_filter.quad(pass, target.m_ping_pong.swap(), m_bleed_program);
	}

	void BlockGlow::glow_blur(Render& render, RenderTarget& target, Glow& glow)
	{
		UNUSED(glow);
		uvec4 rect = render.m_rect;

		static BlurKernel kernel = {
			{ 0.174938f, 0.165569f, 0.140367f, 0.106595f, 0.165569f, 0.140367f, 0.106595f },
			{ 0.288713f, 0.233062f, 0.122581f, 0.233062f, 0.122581f }
		};

		size_t max_level = 0;

		for(vec4::length_type i = 0; i < 8; ++i)
			if((i < 4 && glow.m_levels_1_4[i]) || (i >= 4 && glow.m_levels_5_8[i - 4]))
				max_level = i;

		for(uint8_t i = 0; i < (max_level + 1); i++)
		{
			m_blur.gaussian_pass(render, target, rect, i, true, kernel);
			m_blur.gaussian_pass(render, target, rect, i, false, kernel);

			//bool blit_support = (bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_BLIT) != 0;
			bool blit_support = false;

			RenderQuad quad = { target.source_quad(vec4(rect), true), target.dest_quad_mip(vec4(rect), i + 1, true), true };

			Pass pass = render.composite_pass("glow blit");
			if(blit_support)
				bgfx::blit(pass.m_index,
						   target.m_cascade.m_texture, i + 1, uint16_t(rect.x), uint16_t(rect.y), 0,
						   target.m_ping_pong.last(), 0, uint16_t(rect.x), uint16_t(rect.y), 0, uint16_t(rect.width), uint16_t(rect.height), 1);
			else
				m_copy.submit(pass, *target.m_cascade.m_fbos[i + 1], target.m_ping_pong.last(), quad);
		}
	}

	void BlockGlow::glow_merge(Render& render, RenderTarget& target, Glow& glow)
	{
		ProgramVersion program = { m_merge_program };

		program.set_option(m_index, GLOW_FILTER_BICUBIC, glow.m_bicubic_filter);

		m_filter.source0(target.m_post_process.last());
		m_filter.source1(target.m_cascade.m_texture);

		GpuState<Glow>::me.upload(glow);

		Pass pass = render.composite_pass("glow merge");
		m_filter.quad(pass, target.m_post_process.swap(), program);
	}
}
