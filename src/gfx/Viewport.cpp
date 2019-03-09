//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <gfx/Cpp20.h>

#ifdef MUD_MODULES
module mud.gfx;
#else
#include <bgfx/bgfx.h>

#include <ecs/ECS.hpp>
#include <geom/Intersect.h>
#include <gfx/Viewport.h>
#include <gfx/Camera.h>
#include <gfx/Item.h>
#include <gfx/Renderer.h>
#include <gfx/Froxel.h>
#include <gfx/Shot.h>
#endif

//#include <cstdio>

//#define NO_OCCLUSION_CULLING

namespace mud
{
	GridECS s_viewer_ecs;
	GridECS* g_viewer_ecs = &s_viewer_ecs;

	static uint16_t viewportIndex = 1;

	Viewport::Viewport(Camera& camera, Scene& scene, uvec4 rect, bool scissor)
		: m_camera(&camera)
		, m_scene(&scene)
		, m_index(viewportIndex++)
		, m_rect(rect)
		, m_scissor(scissor)
		, m_culler(*this)
	{
		(Entt&)(*this) = { &s_viewer_ecs, s_viewer_ecs.create() };
	}

	Viewport::~Viewport()
	{}

	void Viewport::render_pass(const Pass& render_pass)
	{
		//printf("render pass rect %i, %i, %i, %i\n", int(m_rect.x), int(m_rect.y), int(m_rect.z), int(m_rect.w));

		bgfx::setViewRect(render_pass.m_index, uint16_t(m_rect.x), uint16_t(m_rect.y), uint16_t(rect_w(m_rect)), uint16_t(rect_h(m_rect)));
		bgfx::setViewTransform(render_pass.m_index, value_ptr(m_camera->m_transform), value_ptr(m_camera->m_projection));
		bgfx::setViewFrameBuffer(render_pass.m_index, render_pass.m_fbo);
		bgfx::setViewClear(render_pass.m_index, BGFX_CLEAR_NONE);

		if(m_scissor)
			bgfx::setViewScissor(render_pass.m_index, uint16_t(m_rect.x), uint16_t(m_rect.y), uint16_t(rect_w(m_rect)), uint16_t(rect_h(m_rect)));

		bgfx::touch(render_pass.m_index);
	}

	void Viewport::cull(Render& render)
	{
#ifndef NO_OCCLUSION_CULLING
		m_culler.render(render);
#else
		UNUSED(render);
#endif
	}

	void Viewport::render(Render& render)
	{
		if(rect_h(m_rect) != 0)
			m_camera->m_aspect = float(rect_w(m_rect)) / float(rect_h(m_rect));
		m_camera->update();

		if(m_camera->m_clusters)
		{
			m_camera->m_clusters->m_dirty |= uint8_t(Froxelizer::Dirty::Viewport) | uint8_t(Froxelizer::Dirty::Projection);
			m_camera->m_clusters->update(*this, m_camera->m_projection, m_camera->m_near, m_camera->m_far);
			m_camera->m_clusters->clusterize_lights(*m_camera, render.m_shot->m_lights);
			m_camera->m_clusters->upload();
		}

		for(RenderTask& task : m_tasks)
			task(render);
	}

	/*void hmdUpdate()
	{
		// Set view and projection matrix for view 0.
		const bgfx::HMD* hmd = bgfx::getHMD();
		if(NULL != hmd && 0 != (hmd->flags & BGFX_HMD_RENDERING))
		{
			float view[16];
			bx::mtxQuatTranslationHMD(view, hmd->eye[0].rotation, eye);
			bgfx::setViewTransform(0, view, hmd->eye[0].projection, BGFX_VIEW_STEREO, hmd->eye[1].projection);

			// Set view 0 default viewport.
			//
			// Use HMD's width/height since HMD's internal frame buffer size
			// might be much larger than window size.
			bgfx::setViewRect(0, 0, 0, hmd->width, hmd->height);
		}
		else
		
	}*/

	Ray Viewport::ray(const vec2& pos)
	{
		// coord in NDC
		float xNDC = (pos.x / float(rect_w(m_rect))) * 2.0f - 1.0f;
		float yNDC = ((float(rect_h(m_rect)) - pos.y) / float(rect_h(m_rect))) * 2.0f - 1.0f;

		return m_camera->ray({ xNDC, yNDC });
	}

	vec3 Viewport::raycast(const Plane& plane, const vec2& pos)
	{
		Ray ray = this->ray(pos);
		return plane_segment_intersection(plane, { ray.m_start, ray.m_end });
	}
}
