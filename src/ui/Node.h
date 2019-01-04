//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#ifndef MUD_MODULES
#include <type/Ref.h>
#include <math/Colour.h>
#include <math/Vec.h>
#endif
#include <ui/Forward.h>
#include <ui/Style/Style.h>
#include <ui/Widget.h>

namespace mud
{
	export_ struct refl_ MUD_UI_EXPORT NodeConnection
	{
		NodeConnection() : m_valid(false) {}
		NodeConnection(size_t out_node, size_t out_plug, size_t in_node, size_t in_plug) : m_valid(true), m_out_node(out_node), m_out_plug(out_plug), m_in_node(in_node), m_in_plug(in_plug) {}
		bool m_valid;
		size_t m_out_node;
		size_t m_out_plug;
		size_t m_in_node;
		size_t m_in_plug;
	};

namespace ui
{
	export_ struct NodeStyles
	{
		NodeStyles();
		Style node; Style body; Style plugs; Style inputs; Style outputs; Style knob; Style knob_output; Style knob_proxy; Style plug; Style cable; Style header;
	};

	export_ MUD_UI_EXPORT NodeStyles& node_styles();

	export_ struct CanvasStyles
	{
		CanvasStyles();
		Style canvas;
	};

	export_ MUD_UI_EXPORT CanvasStyles& canvas_styles();

	export_ MUD_UI_EXPORT NodePlug& node_plug(Node& node, cstring name, cstring icon, const Colour& colour, bool input, bool active, bool connected);
	
	export_ func_ inline NodePlug& node_input(Node& node, cstring name, cstring icon = "", const Colour& colour = Colour::NeonGreen, bool active = true, bool connected = false)
	{
		return node_plug(node, name, icon, colour, true, active, connected);
	}

	export_ func_ inline NodePlug& node_output(Node& node, cstring name, cstring icon = "", const Colour& colour = Colour::NeonGreen, bool active = true, bool connected = false)
	{
		return node_plug(node, name, icon, colour, false, active, connected);
	}

	export_ MUD_UI_EXPORT Node& node(Canvas& parent, array<cstring> title, int order = 0, Ref identity = {});
	export_ MUD_UI_EXPORT Node& node(Canvas& parent, array<cstring> title, float* position, int order = 0, Ref identity = {});
	export_ MUD_UI_EXPORT Node& node(Canvas& parent, array<cstring> title, vec2& position, int order = 0, Ref identity = {});
	export_ MUD_UI_EXPORT func_ Node& node(Canvas& parent, cstring title, vec2& position, int order = 0, Ref identity = {});

	export_ MUD_UI_EXPORT func_ Widget& node_cable(Canvas& canvas, NodePlug& plug_out, NodePlug& plug_in);

	export_ MUD_UI_EXPORT func_ Canvas& canvas(Widget& parent, size_t num_nodes = 0);
	export_ MUD_UI_EXPORT NodeConnection canvas_connect(Canvas& canvas);
	export_ MUD_UI_EXPORT void canvas_autolayout(Canvas& canvas);
}
}
