#pragma once

#include <tool/Forward.h>

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#include <type/Type.h>
#include <type/Vector.h>
#endif

#ifndef MUD_MODULES
#include <infra/Types.h>
#include <tree/Types.h>
#include <type/Types.h>
#include <refl/Types.h>
#include <srlz/Types.h>
#include <lang/Types.h>
#include <math/Types.h>
#include <geom/Types.h>
#include <ctx/Types.h>
#include <ui/Types.h>
#include <uio/Types.h>
#include <gfx/Types.h>
#include <gfx-pbr/Types.h>
#endif

#ifndef MUD_CPP_20
#include <string>
#include <cstdint>
#include <vector>
#endif


namespace mud
{
    // Exported types
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ToolState>();
    
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::EditContext>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::EditorAction>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::Gizmo>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::Tool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ToolContext>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ToolOption>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::TransformAction>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ViewAction>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::WorkPlaneAction>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::PlaneSnapOption>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::WorldSnapOption>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::RedoTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::UndoTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ViewportTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::WorkPlaneTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::Brush>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::TransformTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::RotateAction>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ScaleAction>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::TranslateAction>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::CopyTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::RotateTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ScaleTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::TranslateTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::FrameViewTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::SpatialTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ViewTool>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::CircleBrush>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::PlaceBrush>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::ScriptedBrush>();
    export_ template <> MUD_TOOL_EXPORT Type& type<mud::CopyAction>();
    
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::EditContext*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::EditorAction*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::Gizmo*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::Tool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ToolContext*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ToolOption*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::TransformAction*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ViewAction*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::WorkPlaneAction*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::PlaneSnapOption*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::WorldSnapOption*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::RedoTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::UndoTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ViewportTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::WorkPlaneTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::Brush*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::TransformTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::RotateAction*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ScaleAction*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::TranslateAction*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::CopyTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::RotateTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ScaleTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::TranslateTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::FrameViewTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::SpatialTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ViewTool*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::CircleBrush*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::PlaceBrush*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::ScriptedBrush*>>;
	export_ template struct MUD_TOOL_EXPORT Typed<std::vector<mud::CopyAction*>>;
}
