//#include <mud/mud.h>
//#include <mud/Types.h>
//#include <mud/Modules.h>

#include <mud/Shell.h>

#include <gfx-pbr/Api.h>
#include <gfx-obj/Api.h>
#include <gfx-gltf/Api.h>

#include <gfx/GfxSystem.h>

#include <ui-vg/VgVg.h>

#ifdef MUD_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

//#define MUD_GFX_DEFERRED

namespace mud
{
#ifdef MUD_PLATFORM_EMSCRIPTEN
	static Shell* g_app = nullptr;
	static size_t g_frame = 0U;
	static size_t g_iterations = 0U;
	static void iterate()
	{
		g_app->pump();
		if(g_iterations > 0 && g_frame++ > g_iterations)
			emscripten_cancel_main_loop();
	}
#endif

	string exec_path(int argc, char *argv[])
	{
#ifdef _WIN32
		UNUSED(argc);
		string exec_path = argv[0];
		string exec_dir(exec_path.begin(), exec_path.begin() + exec_path.rfind('\\'));
#else
		UNUSED(argc); UNUSED(argv);
		string exec_dir = "./";
#endif
		return exec_dir;
	}

    Shell::Shell(array<cstring> resource_paths, int argc, char *argv[])
        : m_exec_path(exec_path(argc, argv))
		, m_resource_path(resource_paths[0])
		, m_job_system()
		, m_gfx_system(resource_paths)
		//, m_lua(false)
		//, m_wren(false)
		//, m_editor(m_gfx_system)
	{
		//System::instance().load_modules({ &mud_infra::m(), &mud_type::m(), &mud_pool::m(), &mud_refl::m(), &mud_proto::m(), &mud_tree::m() });
		//System::instance().load_modules({ &mud_srlz::m(), &mud_math::m(), &mud_geom::m(), &mud_noise::m(), &mud_wfc::m(), &mud_fract::m(), &mud_lang::m() });
		//System::instance().load_modules({ &mud_ctx::m(), &mud_ui::m(), &mud_gfx::m(), &mud_gfx_pbr::m(), &mud_gfx_obj::m(), &mud_gfx_gltf::m(), &mud_gfx_ui::m(), &mud_tool::m() });

		// @todo this should be automatically done by math module
		register_math_conversions();

		//m_editor.m_script_editor.m_lua = &m_lua;
		//m_editor.m_script_editor.m_wren = &m_wren;

		//declare_gfx_edit();

		m_gfx_system.m_job_system = &m_job_system;
		m_job_system.adopt();

		this->init();
	}

	Shell::~Shell()
    {
		m_job_system.emancipate();
	}

	void Shell::run(const std::function<void(Shell&)>& func, size_t iterations)
	{
		m_pump = func;

#ifdef MUD_PLATFORM_EMSCRIPTEN
		g_app = this;
		g_iterations = iterations;
		emscripten_set_main_loop(iterate, 0, 1);
#else
		size_t frame = 0;
		while(pump() && (iterations == 0 || frame++ < iterations));
#endif
	}

	bool Shell::pump()
	{
		bool pursue = m_ui_window->input_frame();
		m_pump(*this);
		m_gfx_system.begin_frame();
		m_ui_window->render_frame();
		pursue &= m_gfx_system.next_frame();
		return pursue;
	}

	void Shell::init()
	{
		m_context = m_gfx_system.create_context("mud EditorCore", 1600, 900, false);
		GfxContext& context = as<GfxContext>(*m_context);
#if defined MUD_VG_VG
		m_vg = make_object<VgVg>(m_resource_path.c_str(), &m_gfx_system.m_allocator);
#elif defined MUD_VG_NANOVG
		m_vg = make_object<VgNanoBgfx>(m_resource_path.c_str());
#endif
		context.m_reset_vg = [&] { return m_vg->load_texture(context.m_target->m_diffuse.idx); };

		m_ui_window = make_unique<UiWindow>(*m_context, *m_vg);

		//m_ui_window = &m_gfx_system.create_window("mud EditorCore", 1280, 720, false);
		m_ui = m_ui_window->m_root_sheet.get();

#ifdef MUD_GFX_DEFERRED
		pipeline_pbr(m_gfx_system, *m_gfx_system.m_pipeline, true);
#else
		pipeline_pbr(m_gfx_system, *m_gfx_system.m_pipeline);
#endif
		m_gfx_system.init_pipeline();

		//static ImporterOBJ obj_importer(m_gfx_system);
		//static ImporterGltf gltf_importer(m_gfx_system);

		//string stylesheet = "minimal.yml";
		//string stylesheet = "vector.yml";
		//string stylesheet = "blendish_dark.yml";
		//set_style_sheet(*m_ui_window->m_styler, (string(m_resource_path) + "interface/styles/" + stylesheet).c_str());

		style_minimal(*m_ui_window);
	}

	void shell_context(Widget& parent, ShellContext& context)
	{
		Widget& board = ui::board(parent);
		context.m_screen = &ui::board(board);
		context.m_dockbar = &ui::dockbar(board, context.m_docksystem);
	}
}
