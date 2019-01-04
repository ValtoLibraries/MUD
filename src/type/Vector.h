//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <type/Forward.h>
#include <type/Type.h>
#include <type/TypeOf.h>
//#include <type/Types.h>

#ifndef MUD_CPP_20
#include <vector>
#include <string>
#endif

namespace mud // export_ namespace mud// @todo evaluate export at namespace level ?
{
	using string = std::string;

	export_ template <class T>
	struct Typed<array<T>>
	{
		static inline Type& type() { static string name = "array<" + string(mud::type<typename type_class<T>::type>().m_name) + ">"; static Type ty(name.c_str()); return ty; }
	};
	
	export_ template <>
	struct Typed<array<cstring>>
	{
		static inline Type& type() { static Type ty("array<cstring>"); return ty; }
	};

	export_ template <class T>
	struct Typed<std::vector<T>>
	{
		static inline Type& type() { static string name = "std::vector<" + string(mud::type<typename type_class<T>::type>().m_name) + ">"; static Type ty(name.c_str()); return ty; }
	};
	
	export_ template <>
	struct Typed<std::vector<cstring>>
	{
		static inline Type& type() { static Type ty("std::vector<cstring>"); return ty; }
	};

	export_ template <class... T_Args>
	inline std::vector<Type*> type_vector()
	{
		return{ &type<T_Args>()... };
	}
}
