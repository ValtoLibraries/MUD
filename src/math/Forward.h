#pragma once

#include <infra/Config.h>

#include <infra/Forward.h>
#include <type/Forward.h>
#include <refl/Forward.h>

#ifndef TWO_MATH_EXPORT
#define TWO_MATH_EXPORT TWO_IMPORT
#endif

namespace two
{
    enum Clockwise : unsigned int;
    enum class Axis : unsigned int;
    enum class Axes : unsigned int;
    enum class SignedAxis : unsigned int;
    enum class Side : unsigned int;
    enum class TrackMode : unsigned int;
    enum class Spectrum : short;

	template <class T> class vector2d;
	template <class T> struct vector3d;

    template <class T> struct Range;
    template <class T> struct ValueCurve;
    template <class T> struct ValueTrack;
	template <class T> struct StatDef;
	template <class T> struct Stat;

	template <class T> struct v2;
	template <class T> struct v3;
	template <class T> struct v4;

	using half2 = v2<ushort>;
	using half3 = v3<ushort>;
	using half4 = v4<ushort>;

	using ushort2 = v2<ushort>;
	using ushort3 = v3<ushort>;
	using ushort4 = v4<ushort>;

	using float2 = v2<float>;
	using float3 = v3<float>;
	using float4 = v4<float>;

	using double2 = v2<double>;
	using double3 = v3<double>;
	using double4 = v4<double>;

	using int2 = v2<int>;
	using int3 = v3<int>;
	using int4 = v4<int>;

	using uint2 = v2<uint>;
	using uint3 = v3<uint>;
	using uint4 = v4<uint>;

	using bool2 = v2<bool>;
	using bool3 = v3<bool>;
	using bool4 = v4<bool>;

	using vec2 = float2;
	using vec3 = float3;
	using vec4 = float4;
	using uvec2 = uint2;
	using uvec3 = uint3;
	using uvec4 = uint4;
	using ivec2 = int2;
	using ivec3 = int3;
	using ivec4 = int4;
	using bvec3 = bool3;
	using bvec4 = bool4;

	struct mat3;
	struct mat4;
	struct quat;

    struct Transform;
    struct Colour;
    class Lerp;
    class Grided3;
    struct Image;
    struct Palette;
    struct Image256;
    class ImageAtlas;
    class Sprite;
    class SpriteAtlas;
	class TextureAtlas;
    struct Ratio;
    struct Gauge;
    struct Time;
    struct TimeSpan;
    class Clock;
    struct Anim;
    class Animator;
}

#ifdef TWO_META_GENERATOR
#include <stl/span.h>
#include <stl/vector.h>
//#include <math/Vec.h>
namespace stl
{
	export_ extern template struct refl_ span_ span<uint8_t>;
	export_ extern template struct refl_ span_ span<int>;
	export_ extern template struct refl_ span_ span<float>;
	export_ extern template struct refl_ span_ span<uint32_t>;
	export_ extern template struct refl_ span_ span<two::vec3>;
	export_ extern template struct refl_ span_ span<two::quat>;
	export_ extern template struct refl_ span_ span<two::Colour>;
	export_ extern template struct refl_ span_ span<two::uvec3>;

	export_ extern template class refl_ seque_ vector<int>;
	export_ extern template class refl_ seque_ vector<float>;
	export_ extern template class refl_ seque_ vector<uint32_t>;
	export_ extern template class refl_ seque_ vector<two::vec3>;
	export_ extern template class refl_ seque_ vector<two::quat>;
	export_ extern template class refl_ seque_ vector<two::Colour>;
	export_ extern template class refl_ seque_ vector<two::uvec3>;
}
#endif
