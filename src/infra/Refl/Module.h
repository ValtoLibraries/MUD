
#pragma once

#if !defined MUD_MODULES || defined MUD_OBJ_LIB
#include <refl/Module.h>
#endif

#ifndef MUD_MODULES
#endif
        
#include <infra/Forward.h>
#include <infra/Types.h>
#include <infra/Api.h>

#include <infra/Refl/Convert.h>

#ifndef MUD_INFRA_REFL_EXPORT
#define MUD_INFRA_REFL_EXPORT MUD_IMPORT
#endif

namespace mud
{
	export_ class MUD_INFRA_REFL_EXPORT mudinfra : public Module
	{
	private:
		mudinfra();

	public:
		static mudinfra& m() { static mudinfra instance; return instance; }
	};
}

#ifdef MUD_INFRA_MODULE
extern "C"
MUD_INFRA_REFL_EXPORT Module& getModule();
#endif
