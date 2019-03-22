//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <gfx/Cpp20.h>

#include <bx/platform.h>
#include <bx/readerwriter.h>
#include <bgfx/bgfx.h>

#ifdef MUD_MODULES
module mud.gfx;
#else
#include <stl/string.h>
#include <stl/vector.h>
#include <stl/map.h>
#include <stl/hash_base.hpp>
#include <stl/algorithm.h>
#include <infra/ToString.h>
#include <stl/table.h>
#include <infra/File.h>
#include <gfx/Program.h>
#include <gfx/GfxSystem.h>
#include <gfx/Texture.h>
#include <gfx/Material.h>
#include <gfx/Shader.h>
#include <gfx/Renderer.h>
#include <gfx/Pipeline.h>
#endif

#include <cstring>
#include <cstdio>

namespace bgfx
{
	int compileShader(int _argc, const char* _argv[]);
	void getShaderError(char* _outputText, uint16_t& _outputSize);
}

namespace mud
{
	static const bgfx::Memory* load_mem(bx::FileReaderI& reader, const char* file_path)
	{
		if(!bx::open(&reader, file_path))
		{
			printf("ERROR: failed to open file %s\n", file_path);
			return nullptr;
		}

		uint32_t size = uint32_t(bx::getSize(&reader));
		const bgfx::Memory* mem = bgfx::alloc(size + 1);
		bx::read(&reader, mem->data, size);
		bx::close(&reader);
		mem->data[mem->size - 1] = '\0';
		return mem;
	}

	bgfx::ShaderHandle load_shader(bx::FileReaderI& reader, cstring path)
	{
		if(file_exists(path))
			return bgfx::createShader(load_mem(reader, path));
		else
			return BGFX_INVALID_HANDLE;
	}

	bgfx::ProgramHandle load_compute_program(bx::FileReaderI& reader, const string& shader_path)
	{
		const string  cs_path = shader_path + "_cs";

		bgfx::ShaderHandle compute_shader = load_shader(reader, cs_path.c_str());

		return bgfx::createProgram(compute_shader, true);
	}

	bgfx::ProgramHandle load_program(bx::FileReaderI& reader, const string& shader_path)
	{
		const string vs_path = shader_path + "_vs";
		const string gs_path = shader_path + "_gs";
		const string fs_path = shader_path + "_fs";

		bgfx::ShaderHandle vertex_shader = load_shader(reader, vs_path.c_str());
		bgfx::ShaderHandle geometry_shader = load_shader(reader, gs_path.c_str());
		bgfx::ShaderHandle fragment_shader = load_shader(reader, fs_path.c_str());

		bgfx::ProgramHandle program = bgfx::isValid(geometry_shader) ? bgfx::createProgram(vertex_shader, geometry_shader, fragment_shader, true)
																	 : bgfx::createProgram(vertex_shader, fragment_shader, true);

		if(!bgfx::isValid(program))
			printf("ERROR: failed to load program %s\n", shader_path.c_str());
		else
			printf("INFO: loaded program %s id %i\n", shader_path.c_str(), int(program.idx));

		return program;
	}

	static table<ShaderType, cstring> c_shader_suffixes = 
	{
		"_cs.sc",
		"_fs.sc",
		"_gs.sc",
		"_vs.sc",
	};

	string shader_path(GfxSystem& gfx, const string& name, ShaderType shader_type)
	{
		string suffix = c_shader_suffixes[shader_type];
		return gfx.m_resource_path + "/shaders/" + name + suffix;
	}

#ifdef MUD_LIVE_SHADER_COMPILER
	bool compile_shader(GfxSystem& gfx, const string& name, const string& suffix, ShaderType shader_type, const string& defines_in, const string& source)
	{
		string defines = defines_in;
		bool is_opengl = bgfx::getRendererType() == bgfx::RendererType::OpenGLES
					  || bgfx::getRendererType() == bgfx::RendererType::OpenGL;

		string source_path = shader_path(gfx, name, shader_type);

		if(source != "")
		{
			write_file(source_path.c_str(), source);
		}

#ifdef _DEBUG
		//bool debug = true;
#else
		//bool debug = false;
#endif

		static table<ShaderType, cstring> output_suffixes = { "_cs", "_fs", "_gs", "_vs" };

		string output_suffix = output_suffixes[shader_type];
		string output_path = gfx.m_resource_path + "/shaders/compiled/" + name + suffix + output_suffix;

		create_file_tree(output_path.c_str());

		printf("INFO: Compiling Shader : %s\n", source_path.c_str());
		printf("INFO: Defines : %s\n", defines.c_str());

		string include = gfx.m_resource_path + "/shaders/";
		string varying_path = gfx.m_resource_path + "/shaders/varying.def.sc";

		enum Target { GLSL, ESSL, HLSL, Metal };
#if BX_PLATFORM_WINDOWS
		Target target = is_opengl ? GLSL : HLSL;
#elif BX_PLATFORM_LINUX
		Target target = GLSL; UNUSED(is_opengl);
#elif BX_PLATFORM_EMSCRIPTEN
		//Target target = ESSL; UNUSED(is_opengl);
		Target target = GLSL; UNUSED(is_opengl);
#elif BX_PLATFORM_OSX
		Target target = is_opengl ? GLSL : Metal;
#endif

		if(target == ESSL || target == Metal)
			defines += "NO_TEXEL_FETCH;";

		vector<cstring> args;
		auto push_arg = [&args](cstring name, cstring arg) { args.push_back(name); args.push_back(arg); };

		static table<ShaderType, cstring> types = { "compute", "fragment", "geometry", "vertex" };

		push_arg("-f", source_path.c_str());
		push_arg("-o", output_path.c_str());
		push_arg("-i", include.c_str());
		args.push_back("--depends");
		push_arg("--varyingdef", varying_path.c_str());
		push_arg("--define", defines.c_str());
		push_arg("--type", types[shader_type]);

		//if(debug)
			//args.push_back("--debug");

		args.push_back("-O3");

		if(target == GLSL)
		{
			push_arg("--platform", "linux");
			push_arg("--profile", "120");
			//push_arg("--profile", "130");
			//push_arg("--profile", "430");
		}
		else if(target == ESSL)
		{
			push_arg("--platform", "android");
		}
		else if(target == HLSL)
		{
			static table<ShaderType, cstring> profiles = { "cs_5_0", "ps_5_0", "gs_5_0", "vs_5_0" };
			push_arg("--platform", "windows");
			push_arg("--profile", profiles[shader_type]);
		}
		else if(target == Metal)
		{
			push_arg("--platform", "osx");
			push_arg("--profile", "metal");
		}

		int result = bgfx::compileShader(uint32_t(args.size()), args.data());

		if(result == EXIT_FAILURE)
		{
			char output_text[90000];
			uint16_t output_size;
			bgfx::getShaderError(output_text, output_size);

			printf("ERROR: failed to compile %s (%s), defines = %s\n", source_path.c_str(), output_path.c_str(), defines.c_str());
			printf("%s\n", output_text);
			return false;
		}

		return true;
	}
#endif

	struct Program::Impl
	{
		map<uint64_t, Version> m_versions;
	};

	string program_defines(const Program& program, const ProgramVersion& version)
	{
		string defines = "";

		for(size_t option = 0; option < 32; ++option)
			if(version.m_options & uint32_t(1 << option))
				defines += program.m_option_names[option] + ";";

		for(size_t mode = 0; mode < program.m_mode_names.size(); ++mode)
			defines += program.m_mode_names[mode] + "=" + to_string(version.m_modes[mode]) + ";";

		for(const ShaderDefine& define : program.m_defines)
			defines += string(define.m_name) + "=" + define.m_value + ";";

		return defines;
	}

	GfxSystem* Program::ms_gfx_system = nullptr;

	Program::Program(const string& name, bool compute)
		: m_name(name)
		, m_compute(compute)
		, m_impl(make_unique<Impl>())
	{
		GfxBlock& mat = *ms_gfx_system->m_renderer.block<BlockMaterial>();
		
		static cstring options[] = { "INSTANCING", "BILLBOARD", "SKELETON", "QNORMALS", "VFLIP", "MRT", "DEFERRED", "CLUSTERED",
									 "ZONES_BUFFER", "LIGHTS_BUFFER", "MATERIALS_BUFFER" };
		this->register_options(0, options);
		this->register_block(mat);

		m_blocks[MaterialBlock::Base] = true;
	}

	Program::~Program()
	{}

	void Program::register_blocks(span<GfxBlock*> blocks)
	{
		for(GfxBlock* block : blocks)
			this->register_block(*block);
	}

	void Program::register_block(const GfxBlock& block)
	{
		m_shader_blocks[block.m_index].m_enabled = true;
		this->register_options(block.m_index, block.m_shader_block->m_options);
		this->register_modes(block.m_index, block.m_shader_block->m_modes);
		if(block.m_shader_block->m_defines.size() > 0)
			prepend(m_defines, block.m_shader_block->m_defines);
	}

	void Program::register_options(uint8_t block, span<cstring> options)
	{
		m_shader_blocks[block].m_enabled = true;
		m_shader_blocks[block].m_option_shift = uint8_t(m_option_names.size());

		for(size_t i = 0; i < options.size(); ++i)
			m_option_names.push_back(options[i]);
	}

	void Program::register_modes(uint8_t block, span<cstring> modes)
	{
		m_shader_blocks[block].m_enabled = true;
		m_shader_blocks[block].m_mode_shift = uint8_t(m_mode_names.size());

		for(size_t i = 0; i < modes.size(); ++i)
			m_mode_names.push_back(modes[i]);
	}

	void Program::set_block(MaterialBlock type, bool enabled)
	{
		m_blocks[type] = enabled;
	}

	void Program::set_pass(PassType type, bool enabled)
	{
		m_passes[type] = enabled;
	}

	void Program::set_source(ShaderType type, const string& source)
	{
		m_sources[type] = source;
	}

	string Program::defines(const ProgramVersion& version) const
	{
		return program_defines(*this, version);
	}

	ProgramVersion Program::shader_version(Version& version)
	{
		ProgramVersion config = { this };
		memcpy(&config.m_options, &version.m_version, sizeof(uint64_t));
		return config;
	}

	void Program::compile(GfxSystem& gfx, Version& version, bool compute)
	{
		const ProgramVersion config = shader_version(version);

		const string suffix = "_v" + to_string(version.m_version);
		const string defines = program_defines(*this, config);

		bool compiled = true;
#ifdef MUD_LIVE_SHADER_COMPILER
		if(compute)
		{
			compiled &= compile_shader(gfx, m_name, suffix, ShaderType::Compute, defines, m_sources[ShaderType::Compute]);
		}
		else
		{
			compiled &= compile_shader(gfx, m_name, suffix, ShaderType::Vertex, defines, m_sources[ShaderType::Vertex]);
			compiled &= compile_shader(gfx, m_name, suffix, ShaderType::Fragment, defines, m_sources[ShaderType::Fragment]);

			if(file_exists(shader_path(gfx, m_name, ShaderType::Geometry).c_str()))
				compiled &= compile_shader(gfx, m_name, suffix, ShaderType::Geometry, defines, m_sources[ShaderType::Geometry]);
		}
#endif

		const string full_name = m_name + suffix;

		if(!compiled)
		{
			printf("WARNING: failed to compile program %s : using last valid version instead\n", full_name.c_str());
			version.m_update = m_update;
			return;
		}

		printf("INFO: loading program %s with options %s\n", full_name.c_str(), defines.c_str());
		const string compiled_path = gfx.m_resource_path + "/shaders/compiled/" + full_name;
		version.m_program = compute ? load_compute_program(gfx.file_reader(), compiled_path)
									: load_program(gfx.file_reader(), compiled_path);
		version.m_update = m_update;
	}

	void Program::update(GfxSystem& gfx)
	{
		for(auto& hash_version : m_impl->m_versions)
		{
			Version& version = hash_version.second;
			if(version.m_update < m_update)
			{
				this->compile(gfx, version, m_compute);
			}
		}
	}

	bgfx::ProgramHandle Program::default_version()
	{
		ProgramVersion config;
		config.m_program = this;
		return this->version(config);
	}

	bgfx::ProgramHandle Program::version(const ProgramVersion& config)
	{
		uint64_t version_hash = config.hash();

		Version& version = m_impl->m_versions[version_hash];
		if(version.m_update < m_update)
		{
			version.m_version = version_hash;
			this->compile(*ms_gfx_system, version, m_compute);
		}

		return version.m_program;
	}
}
