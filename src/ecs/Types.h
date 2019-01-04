#pragma once

#include <ecs/Forward.h>

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#include <type/Type.h>
#include <type/Vector.h>
#endif

#ifndef MUD_MODULES
#include <infra/Types.h>
#include <type/Types.h>
#include <refl/Types.h>
#endif

#ifndef MUD_CPP_20
#include <string>
#include <cstdint>
#include <vector>
#endif


namespace mud
{
    // Exported types

	export_ template <> MUD_ECS_EXPORT Type& type<mud::Complex>();
    export_ template <> MUD_ECS_EXPORT Type& type<mud::Entity>();
    export_ template <> MUD_ECS_EXPORT Type& type<mud::Prototype>();

	export_ template struct MUD_ECS_EXPORT Typed<std::vector<mud::Complex*>>;
	export_ template struct MUD_ECS_EXPORT Typed<std::vector<mud::Entity*>>;
	export_ template struct MUD_ECS_EXPORT Typed<std::vector<mud::Prototype*>>;
}
