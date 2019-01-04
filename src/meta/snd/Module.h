
#pragma once

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#include <refl/Module.h>
#endif

#ifndef MUD_MODULES
#endif
        
#include <snd/Forward.h>
#include <snd/Types.h>
#include <snd/Api.h>

#include <meta/snd/Convert.h>

#ifndef MUD_SND_REFL_EXPORT
#define MUD_SND_REFL_EXPORT MUD_IMPORT
#endif

namespace mud
{
	export_ class MUD_SND_REFL_EXPORT mud_snd : public Module
	{
	private:
		mud_snd();

	public:
		static mud_snd& m() { static mud_snd instance; return instance; }
	};
}

#ifdef MUD_SND_MODULE
extern "C"
MUD_SND_REFL_EXPORT Module& getModule();
#endif
