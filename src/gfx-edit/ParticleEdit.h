//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#ifndef MUD_MODULES
#include <ui/Ui.h>
#include <ui/Input.h>
#include <gfx/Particles.h>
#endif
#include <gfx-edit/Forward.h>

namespace mud
{
	MUD_GFX_EDIT_EXPORT void particle_edit(Widget& parent, GfxSystem& system, ParticleGenerator& generator);
	MUD_GFX_EDIT_EXPORT void particle_edit(Widget& parent, GfxSystem& system, Call& particles);
	MUD_GFX_EDIT_EXPORT void particle_editor(Widget& parent, GfxSystem& system);
}
