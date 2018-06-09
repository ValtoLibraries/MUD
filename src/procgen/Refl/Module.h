
#pragma once

#if !defined MUD_MODULES || defined MUD_OBJ_LIB
#include <refl/Module.h>
#endif

#ifndef MUD_MODULES
#include <obj/Refl/Module.h>
#include <srlz/Refl/Module.h>
#include <math/Refl/Module.h>
#include <geom/Refl/Module.h>
#endif
        
#include <procgen/Forward.h>
#include <procgen/Types.h>
#include <procgen/Api.h>

#include <procgen/Refl/Convert.h>

#ifndef MUD_PROCGEN_REFL_EXPORT
#define MUD_PROCGEN_REFL_EXPORT MUD_IMPORT
#endif

namespace mud
{
	export_ class MUD_PROCGEN_REFL_EXPORT mudprocgen : public Module
	{
	private:
		mudprocgen();

	public:
		static mudprocgen& m() { static mudprocgen instance; return instance; }
	};
}

#ifdef MUD_PROCGEN_MODULE
extern "C"
MUD_PROCGEN_REFL_EXPORT Module& getModule();
#endif
