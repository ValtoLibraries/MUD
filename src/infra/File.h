//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <infra/Array.h>
#include <type/Forward.h>

#ifndef MUD_CPP_20
#include <string>
#include <vector>
#include <functional>
#endif

namespace mud
{
	using string = std::string;
	using cstring = const char*;

	export_ struct Filepath
	{
		string path;
		string name;
		string extension;
	};

	export_ MUD_INFRA_EXPORT std::vector<uint8_t> read_binary_file(const string& path);
	export_ MUD_INFRA_EXPORT string read_text_file(const string& path);

	export_ MUD_INFRA_EXPORT void write_binary_file(const string& path, array<uint8_t> data);


	export_ MUD_INFRA_EXPORT bool file_exists(cstring path);
	export_ MUD_INFRA_EXPORT bool directory_exists(cstring path);
	export_ MUD_INFRA_EXPORT string file_directory(cstring path);

	export_ MUD_INFRA_EXPORT bool create_directory(cstring path);
	export_ MUD_INFRA_EXPORT bool create_directory_tree(cstring path);
	export_ MUD_INFRA_EXPORT bool create_file_tree(cstring path);

	export_ using FileVisitor = std::function<void(cstring, cstring)>;
	export_ MUD_INFRA_EXPORT void visit_files(cstring path, FileVisitor visit_file);
	export_ MUD_INFRA_EXPORT void visit_folders(cstring path, FileVisitor visit_folder, bool ignore_symbolic = true);

	export_ MUD_INFRA_EXPORT void write_file(cstring path, cstring content);

	inline string file_directory(const string& path)
	{
		return path.substr(0, path.rfind('/') + 1);
	}

	inline string file_name(const string& path)
	{
		return path.substr(path.rfind('/') + 1);
	}
}
