//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <gfx/Cpp20.h>

#include <bx/math.h>

#ifdef MUD_MODULES
module mud.gfx;
#else
#include <infra/Vector.h>
#include <math/Interp.h>
#include <math/Math.h>
#include <gfx/Types.h>
#include <gfx/Animation.h>
#include <gfx/Node3.h>
#include <gfx/Item.h>
#include <gfx/Mesh.h>
#include <gfx/Skeleton.h>
#endif

namespace mud
{
	static Type* s_target_types[3] = { &type<vec3>(), &type<quat>(), &type<vec3>() };

	Animation::Animation(cstring name)
		: m_name(name)
	{}
	
	AnimationTrack::AnimationTrack(Animation& animation, size_t node, cstring node_name, AnimationTarget target)
		: m_animation(&animation), m_node(node), m_node_name(node_name), m_target(target), m_value_type(s_target_types[size_t(target)])
	{}

	void AnimationTrack::insert_key(float time, const Value& value, float transition)
	{
		size_t position = key_before(time);
		m_keys.insert(m_keys.begin() + position + 1, Key{ time, value, transition });
	}

	size_t AnimationTrack::key_after(float time) const
	{
		assert(m_keys.size() > 0);
		auto predicate = [](float lhs, const Key& rhs) { return lhs < rhs.m_time; };
		auto result = std::upper_bound(m_keys.begin(), m_keys.end(), time, predicate);
		return result - m_keys.begin();
	}

	size_t AnimationTrack::key_before(float time) const
	{
		return key_after(time) - 1;
	}

	Value AnimationTrack::sample(AnimationCursor& cursor) const
	{
		assert(m_keys.size() > 0);

		if(cursor.m_time == m_keys.back().m_time)
		{
			return m_keys.back().m_value;
		}
		if(m_keys.size() == 1)
		{
			return m_keys[0].m_value;
		}

		float c = std::abs(cursor.m_time - m_keys[cursor.m_prev].m_time) / std::abs(m_keys[cursor.m_next].m_time - m_keys[cursor.m_prev].m_time);
		//printf("sample %i < %i, %f\n", int(cursor.m_prev), int(cursor.m_next), c);

		auto at = [&](size_t i) { return Ref((void*)m_keys[i].m_value.m_value, *m_value_type); };

		if(m_interpolation == Interpolation::Nearest)
		{
			return m_keys[cursor.m_prev].m_value;
		}
		else if(m_interpolation == Interpolation::Linear)
		{
			Value result;
			interpolate(Ref((void*)result.m_value, *m_value_type), at(cursor.m_prev), at(cursor.m_next), c);
			return result;
		}
		else if(m_interpolation == Interpolation::Cubic)
		{
			Value result;
			size_t pre = max<size_t>(0, cursor.m_prev - 1);
			size_t post = max<size_t>(cursor.m_next + 1, m_keys.size());
			interpolate_cubic(Ref((void*)result.m_value, *m_value_type), at(pre), at(cursor.m_prev), at(cursor.m_next), at(post), c);
			return result;
		}
	}

	Value AnimationTrack::value(AnimationCursor& cursor, bool forward) const
	{
		size_t key = forward ? cursor.m_next : cursor.m_prev;
		return m_keys[key].m_value;
	}
}

