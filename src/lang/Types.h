#pragma once

#include <lang/Forward.h>

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#include <type/Type.h>
#include <type/Vector.h>
#endif

#ifndef MUD_MODULES
#include <infra/Types.h>
#include <type/Types.h>
#include <pool/Types.h>
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
    export_ template <> MUD_LANG_EXPORT Type& type<mud::Language>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::StreamModifier>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ValveKind>();
    
    export_ template <> MUD_LANG_EXPORT Type& type<mud::Interpreter>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::Pipe>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::Process>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ScriptClass>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::StreamBranch>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::Valve>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::Script>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessFunction>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessMethod>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessScript>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::Stream>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::LuaInterpreter>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::WrenInterpreter>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::TextScript>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::VisualScript>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessCallable>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessCreate>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessDisplay>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessGetMember>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessSetMember>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessValue>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessInput>();
    export_ template <> MUD_LANG_EXPORT Type& type<mud::ProcessOutput>();
    
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::Interpreter*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::Pipe*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::Process*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ScriptClass*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::StreamBranch*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::Valve*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::Script*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessFunction*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessMethod*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessScript*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::Stream*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::LuaInterpreter*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::WrenInterpreter*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::TextScript*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::VisualScript*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessCallable*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessCreate*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessDisplay*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessGetMember*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessSetMember*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessValue*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessInput*>>;
	export_ template struct MUD_LANG_EXPORT Typed<std::vector<mud::ProcessOutput*>>;
}
