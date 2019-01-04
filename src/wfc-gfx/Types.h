#pragma once

#include <wfc-gfx/Forward.h>

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#include <type/Type.h>
#include <type/Vector.h>
#endif

#ifndef MUD_MODULES
#include <infra/Types.h>
#include <tree/Types.h>
#include <type/Types.h>
#include <srlz/Types.h>
#include <math/Types.h>
#include <geom/Types.h>
#include <wfc/Types.h>
#include <ctx/Types.h>
#include <ui/Types.h>
#include <uio/Types.h>
#include <gfx/Types.h>
#include <gfx-ui/Types.h>
#endif

#ifndef MUD_CPP_20
#include <string>
#include <cstdint>
#include <vector>
#endif


namespace mud
{
    // Exported types
    
    export_ template <> MUD_WFC_GFX_EXPORT Type& type<mud::TileModel>();
    export_ template <> MUD_WFC_GFX_EXPORT Type& type<mud::WfcBlock>();
    
	export_ template struct MUD_WFC_GFX_EXPORT Typed<std::vector<mud::TileModel*>>;
	export_ template struct MUD_WFC_GFX_EXPORT Typed<std::vector<mud::WfcBlock*>>;
}
