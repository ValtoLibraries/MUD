//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module mud.tool;
#else
#include <math/Axes.h>
#include <geom/Shapes.h>
#include <geom/Intersect.h>
#include <gfx/Gfx.h>
#include <gfx/Item.h>
#include <tool/Types.h>
#include <tool/Tools/ScaleTool.h>
#include <gfx-ui/Viewer.h>
#endif

namespace mud
{
	ScaleAction::ScaleAction(array<Transform*> targets)
		: TransformAction(targets)
		, m_scale_offset(Zero3)
	{}

	void ScaleAction::apply(Transform& transform)
	{
		transform.m_scale += m_scale_offset;
	}

	void ScaleAction::undo(Transform& transform)
	{
		transform.m_scale += -m_scale_offset;
	}

	void ScaleAction::update(const vec3& start, const vec3& end)
	{
		m_scale_offset = end - start;
	}

 	ScaleTool::ScaleTool(ToolContext& context)
		: TransformTool(context, "Scale", type<ScaleTool>())
	{
		m_gizmos.push_back(linear_gizmo(Axis::X, 0.f));
		m_gizmos.push_back(linear_gizmo(Axis::Y, 1.f / 3.f));
		m_gizmos.push_back(linear_gizmo(Axis::Z, 2.f / 3.f));
		m_gizmos.push_back(planar_gizmo(Axis::X, 0.f));
		m_gizmos.push_back(planar_gizmo(Axis::Y, 1.f / 3.f));
		m_gizmos.push_back(planar_gizmo(Axis::Z, 2.f / 3.f));
		m_gizmos.push_back(uniform_gizmo());
		m_current = &m_gizmos.front();
	}

	Item& scale_1d_gizmo(Gnode& parent, Axis axis, Colour colour, uint32_t flags = 0U)
	{
		Gnode& node = gfx::transform(parent, {}, to_vec3(axis), ZeroQuat);
		//return gfx::shape(node, Quad(0.2f, c_tangents[uint(axis)], c_binormals[uint(axis)]), Symbol(colour, Colour::None, true), flags);
		return gfx::shape(node, Cube(0.05f), Symbol(colour, Colour::None, true), flags);
	}

	Item& scale_2d_gizmo(Gnode& parent, Axis axis, Colour colour, uint32_t flags = 0U)
	{
		Gnode& node = gfx::transform(parent, {}, 0.5f * to_vec3(axis), ZeroQuat);
		return gfx::shape(node, Quad(0.2f, c_tangents[uint(axis)], c_binormals[uint(axis)]), Symbol(colour, Colour::None, true, true), flags);
	}

	Item& scale_3d_gizmo(Gnode& parent, Colour colour, uint32_t flags = 0U)
	{
		Gnode& node = gfx::transform(parent, {}, Zero3, ZeroQuat);
		return gfx::shape(node, Cube(0.1f), Symbol(colour, Colour::None, true), flags);
	}

	Gizmo ScaleTool::linear_gizmo(Axis axis, float hue)
	{
#ifdef MUD_MODULES // @todo clang bug
		return {};
#else
		auto grab_point = [this, axis](Viewer& viewer, const vec2& pos) { UNUSED(pos); return inverse(m_transform.m_rotation) * gizmo_grab_linear(viewer, m_transform, axis); };

		auto draw_handle = [=](Gnode& parent) { return &scale_1d_gizmo(parent, axis, Colour::Invisible, ItemFlag::Ui); };
		auto draw_gizmo = [=](Gnode& parent, bool active) { scale_1d_gizmo(parent, axis, gizmo_colour(hue, active)); };
		return { draw_handle, draw_gizmo, nullptr, false, grab_point };
#endif
	}

	Gizmo ScaleTool::planar_gizmo(Axis normal, float hue)
	{
#ifdef MUD_MODULES // @todo clang bug
		return {};
#else
		auto grab_point = [this, normal](Viewer& viewer, const vec2& pos) { UNUSED(pos); return inverse(m_transform.m_rotation) * gizmo_grab_planar(viewer, m_transform, normal); };

		auto draw_handle = [=](Gnode& parent) { return &scale_2d_gizmo(parent, normal, Colour::Invisible, ItemFlag::Ui); };
		auto draw_gizmo = [=](Gnode& parent, bool active) { scale_2d_gizmo(parent, normal, gizmo_colour(hue, active)); };
		return { draw_handle, draw_gizmo, nullptr, false, grab_point };
#endif
	}

	Gizmo ScaleTool::uniform_gizmo()
	{
#ifdef MUD_MODULES // @todo clang bug
		return {};
#else
		auto grab_point = [this](Viewer& viewer, const vec2& pos)
		{
			vec2 delta = (pos - m_drag_start) * 5.f / viewer.m_frame.m_size;
			return fabs(delta.x) > fabs(delta.y) ? vec3(delta.x) : vec3(delta.y);
		};

		auto draw_handle = [=](Gnode& parent) { return &scale_3d_gizmo(parent, Colour::Invisible, ItemFlag::Ui); };
		auto draw_gizmo = [=](Gnode& parent, bool active) { scale_3d_gizmo(parent, active ? Colour::White : Colour::AlphaWhite); };
		return { draw_handle, draw_gizmo, nullptr, false, grab_point };
#endif
	}

	object_ptr<TransformAction> ScaleTool::create_action(array<Transform*> targets)
	{
		return make_object<ScaleAction>(targets);
	}

}
