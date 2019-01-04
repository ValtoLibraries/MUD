#pragma once

#include <math/Forward.h>

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#include <type/Type.h>
#include <type/Vector.h>
#endif

#ifndef MUD_MODULES
#include <infra/Types.h>
#include <type/Types.h>
#include <refl/Types.h>
#include <srlz/Types.h>
#endif

#ifndef MUD_CPP_20
#include <string>
#include <cstdint>
#include <vector>
#endif

#include <math/Structs.h>

namespace mud
{
    // Exported types
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Axes>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Axis>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Clockwise>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Side>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::SignedAxis>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Spectrum>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::TrackMode>();
    
    export_ template <> MUD_MATH_EXPORT Type& type<mud::AutoStat<float>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::AutoStat<int>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Colour>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Image>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Image256>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ImageAtlas>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Palette>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Range<float>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Range<mud::Colour>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Range<mud::quat>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Range<mud::vec3>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Range<uint32_t>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Time>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::TimeSpan>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Transform>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<float>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<mud::Colour>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<mud::quat>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<mud::vec3>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<uint32_t>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<float>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<mud::Colour>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<mud::quat>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<mud::vec3>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<uint32_t>>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::bvec3>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::bvec4>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ivec2>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ivec3>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::ivec4>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::mat4>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::quat>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::uvec2>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::uvec3>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::uvec4>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::vec2>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::vec3>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::vec4>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::SpriteAtlas>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Sprite>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Ratio>();
    export_ template <> MUD_MATH_EXPORT Type& type<mud::Gauge>();
    
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::AutoStat<float>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::AutoStat<int>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Colour*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Image*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Image256*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ImageAtlas*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Palette*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Range<float>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Range<mud::Colour>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Range<mud::quat>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Range<mud::vec3>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Range<uint32_t>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Time*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::TimeSpan*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Transform*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueCurve<float>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueCurve<mud::Colour>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueCurve<mud::quat>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueCurve<mud::vec3>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueCurve<uint32_t>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueTrack<float>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueTrack<mud::Colour>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueTrack<mud::quat>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueTrack<mud::vec3>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ValueTrack<uint32_t>*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::bvec3*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::bvec4*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ivec2*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ivec3*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::ivec4*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::mat4*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::quat*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::uvec2*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::uvec3*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::uvec4*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::vec2*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::vec3*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::vec4*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::SpriteAtlas*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Sprite*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Ratio*>>;
	export_ template struct MUD_MATH_EXPORT Typed<std::vector<mud::Gauge*>>;
}
