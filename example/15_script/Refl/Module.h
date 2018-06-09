
#pragma once

#if !defined MUD_MODULES || defined MUD_OBJ_LIB
#include <refl/Module.h>
#endif

#ifndef MUD_MODULES
#include <obj/Refl/Module.h>
#include <math/Refl/Module.h>
#include <ui/Refl/Module.h>
#include <uio/Refl/Module.h>
#include <gfx/Refl/Module.h>
#include <edit/Refl/Module.h>
#include <procgen/Refl/Module.h>
#include <procgen-gfx/Refl/Module.h>
#endif
        
#include <15_script/Forward.h>
#include <15_script/Types.h>
#include <15_script/Api.h>

#include <15_script/Refl/Convert.h>

#ifndef _15_SCRIPT_REFL_EXPORT
#define _15_SCRIPT_REFL_EXPORT MUD_IMPORT
#endif

	export_ class _15_SCRIPT_REFL_EXPORT ex_15_script : public Module
	{
	private:
		ex_15_script();

	public:
		static ex_15_script& m() { static ex_15_script instance; return instance; }
	};

#ifdef _15_SCRIPT_MODULE
extern "C"
_15_SCRIPT_REFL_EXPORT Module& getModule();
#endif
