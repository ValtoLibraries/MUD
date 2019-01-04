//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module mud.ui;
#else
#include <infra/Vector.h>
#include <infra/String.h>
#include <infra/File.h>
#include <ctx/Context.h>
#include <ui/UiWindow.h>
#include <ui/Api.h>
#endif

namespace mud
{
	std::map<string, Style*> UiWindow::s_styles;

	UiWindow::UiWindow(Context& context, Vg& vg, User* user)
		: m_resource_path(context.m_resource_path)
		, m_context(context)
		, m_vg(vg)
		, m_renderer(vg)
		, m_atlas(uvec2(1024U))
		, m_width(float(context.m_width))
		, m_height(float(context.m_height))
		, m_user(user)
	{
		this->init();
	}

	UiWindow::~UiWindow()
	{
		for(auto& image : m_images)
			if(image->d_handle != -1)
			{
				m_vg.unload_image(*image);
			}
	}

	void UiWindow::init()
	{
		this->init_styles();

		printf("INFO: Initializing UiWindow: resource path %s\n", m_resource_path.c_str());
		m_vg.setup_context();

		this->init_resources();
		this->load_resources();

		styles().setup(*this);

		m_root_sheet = make_object<Ui>(*this);

		m_context.init_input(m_root_sheet->m_mouse, m_root_sheet->m_keyboard);

		this->resize(uint16_t(m_width), uint16_t(m_height));
	}

	void UiWindow::init_styles()
	{
		styles();

		ui::cursor_styles();
		ui::scrollbar_styles();
		ui::dropdown_styles();
		ui::table_styles();
		ui::expandbox_styles();
		ui::treenode_styles();
		ui::tabber_styles();
		ui::menu_styles();
		ui::toolbar_styles();
		ui::window_styles();
		ui::dock_styles();
		ui::canvas_styles();
		ui::node_styles();
		ui::file_styles();
	}

	void UiWindow::reset_styles()
	{
		styles() = {};

		ui::cursor_styles() = {};
		ui::scrollbar_styles() = {};
		ui::dropdown_styles() = {};
		ui::table_styles() = {};
		ui::expandbox_styles() = {};
		ui::treenode_styles() = {};
		ui::tabber_styles() = {};
		ui::menu_styles() = {};
		ui::toolbar_styles() = {};
		ui::window_styles() = {};
		ui::dock_styles() = {};
		ui::canvas_styles() = {};
		ui::node_styles() = {};
		ui::file_styles() = {};
	}

	void UiWindow::init_resources()
	{
		string sprite_path = string(m_resource_path) + "interface/uisprites/";

		printf("INFO: Loading Images in path %s\n", sprite_path.c_str());

		std::vector<Image> images;
		load_folder_images(images, sprite_path, "");

		auto visit_folder = [&](cstring path, cstring folder)
		{
			UNUSED(path);
			load_folder_images(images, sprite_path + folder + "/", string(folder) + "/");
		};

		visit_folders(sprite_path.c_str(), visit_folder);

		m_images = vector_convert<object_ptr<Image>>(images, [](const Image& image) { return make_object<Image>(image); });
	}

	void UiWindow::load_resources()
	{
		m_vg.load_default_font();

		std::vector<Image*> images;
		for(auto& image : m_images) images.push_back(image.get());

		std::vector<unsigned char> atlas = m_atlas.generate_atlas(images);
		m_vg.load_image_RGBA(m_atlas.m_image, atlas.data());
	}

	Image& UiWindow::create_image(cstring name, uvec2 size, uint8_t* data, bool filtering)
	{
		m_images.emplace_back(make_object<Image>(name, name, size));
		Image& image = *m_images.back();
		image.d_filtering = filtering;
		m_vg.load_image_RGBA(image, data);
		return image;
	}

	void UiWindow::remove_image(Image& image)
	{
		vector_remove_if(m_images, [&](object_ptr<Image>& current) { return current.get() == &image; });
		m_vg.unload_image(image);
	}

	Image* UiWindow::find_image(cstring name)
	{
		for(auto& image : m_images)
			if(image->d_name == name)
				return image.get();
		return nullptr;
	}

	void UiWindow::resize(uint16_t width, uint16_t height)
	{
		m_width = float(width);
		m_height = float(height);

		m_context.reset(width, height);

		printf("INFO: UiWindow :: resize %i, %i\n", int(width), int(height));
		m_root_sheet->m_frame.set_size({ m_width, m_height });
	}

	bool UiWindow::input_frame()
	{
		bool pursue = !m_shutdown;
		pursue &= m_context.next_frame();

		if(m_context.m_width != uint(m_width) || m_context.m_height != uint(m_height))
			this->resize(uint16_t(m_context.m_width), uint16_t(m_context.m_height));

		m_root_sheet->input_frame();

		m_root_sheet->m_frame.relayout();

		return pursue;
	}

	void UiWindow::render_frame()
	{
		//m_root_sheet->render_frame();

		if(m_context.m_render_system.m_manual_render)
		{
			m_renderer.render(*m_root_sheet->m_frame.m_layer);
			// add sub layers
		}

		m_root_sheet->clear_events();
	}

	void UiWindow::shutdown()
	{
		m_shutdown = true;
	}
}
