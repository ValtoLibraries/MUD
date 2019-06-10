//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <infra/Cpp20.h>
#ifdef TWO_MODULES
#define _GLIBCXX_TYPE_TRAITS
#include <xmmintrin.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

#define TEXTURE_CLAMP BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP
#define TEXTURE_CLAMP_UVW BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP | BGFX_TEXTURE_W_CLAMP
#define TEXTURE_POINT BGFX_TEXTURE_MIN_POINT | BGFX_TEXTURE_MAG_POINT
#endif

