//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <ui/Forward.h>
#include <ui/Style/Style.h>

#ifndef MUD_CPP_20
#include <string>
#endif

struct dirent;

namespace mud
{
	export_ using string = std::string;

namespace ui
{
	export_ struct FileStyles
	{
		FileStyles();
		Style dir; Style file;
	};

	export_ MUD_UI_EXPORT FileStyles& file_styles();

	export_ MUD_UI_EXPORT func_ Widget& dir_item(Widget& parent, cstring name);
	export_ MUD_UI_EXPORT func_ Widget& file_item(Widget& parent, cstring name);
	export_ MUD_UI_EXPORT func_ Widget& file_list(Widget& parent, string& path);

	export_ MUD_UI_EXPORT func_ Widget& file_browser(Widget& parent, string& path);

	export_ MUD_UI_EXPORT func_ Widget& dir_node(Widget& parent, cstring path, cstring name, bool collapsed);
	export_ MUD_UI_EXPORT func_ Widget& file_node(Widget& parent, cstring name);
	export_ MUD_UI_EXPORT func_ Widget& file_tree(Widget& parent, cstring path);
}
}
