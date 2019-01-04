

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module mud.math;
#else
#include <math/Types.h>
#include <math/Api.h>
#include <type/Vector.h>
//#include <ecs/Proto.h>
#endif

namespace mud
{
    // Exported types
    template <> MUD_MATH_EXPORT Type& type<mud::Axes>() { static Type ty("mud::Axes"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Axis>() { static Type ty("mud::Axis"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Clockwise>() { static Type ty("mud::Clockwise"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Side>() { static Type ty("mud::Side"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::SignedAxis>() { static Type ty("mud::SignedAxis"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Spectrum>() { static Type ty("mud::Spectrum"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::TrackMode>() { static Type ty("mud::TrackMode"); return ty; }
    
    template <> MUD_MATH_EXPORT Type& type<mud::AutoStat<float>>() { static Type ty("AutoStat<float>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::AutoStat<int>>() { static Type ty("AutoStat<int>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Colour>() { static Type ty("Colour"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Image>() { static Type ty("Image"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Image256>() { static Type ty("Image256"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ImageAtlas>() { static Type ty("ImageAtlas"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Palette>() { static Type ty("Palette"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Range<float>>() { static Type ty("Range<float>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Range<mud::Colour>>() { static Type ty("Range<mud::Colour>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Range<mud::quat>>() { static Type ty("Range<mud::quat>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Range<mud::vec3>>() { static Type ty("Range<mud::vec3>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Range<uint32_t>>() { static Type ty("Range<uint32_t>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Time>() { static Type ty("Time"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::TimeSpan>() { static Type ty("TimeSpan"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Transform>() { static Type ty("Transform"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<float>>() { static Type ty("ValueCurve<float>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<mud::Colour>>() { static Type ty("ValueCurve<mud::Colour>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<mud::quat>>() { static Type ty("ValueCurve<mud::quat>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<mud::vec3>>() { static Type ty("ValueCurve<mud::vec3>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueCurve<uint32_t>>() { static Type ty("ValueCurve<uint32_t>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<float>>() { static Type ty("ValueTrack<float>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<mud::Colour>>() { static Type ty("ValueTrack<mud::Colour>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<mud::quat>>() { static Type ty("ValueTrack<mud::quat>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<mud::vec3>>() { static Type ty("ValueTrack<mud::vec3>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ValueTrack<uint32_t>>() { static Type ty("ValueTrack<uint32_t>"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::bvec3>() { static Type ty("bvec3"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::bvec4>() { static Type ty("bvec4"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ivec2>() { static Type ty("ivec2"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ivec3>() { static Type ty("ivec3"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::ivec4>() { static Type ty("ivec4"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::mat4>() { static Type ty("mat4"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::quat>() { static Type ty("quat"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::uvec2>() { static Type ty("uvec2"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::uvec3>() { static Type ty("uvec3"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::uvec4>() { static Type ty("uvec4"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::vec2>() { static Type ty("vec2"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::vec3>() { static Type ty("vec3"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::vec4>() { static Type ty("vec4"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::SpriteAtlas>() { static Type ty("SpriteAtlas", type<mud::ImageAtlas>()); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Sprite>() { static Type ty("Sprite", type<mud::Image>()); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Ratio>() { static Type ty("Ratio"); return ty; }
    template <> MUD_MATH_EXPORT Type& type<mud::Gauge>() { static Type ty("Gauge"); return ty; }
}
