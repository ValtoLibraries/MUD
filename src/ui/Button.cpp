//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <infra/Cpp20.h>
#ifndef MUD_CPP_20
#include <cstring>
#endif

#ifdef MUD_MODULES
module mud.ui;
#else
#include <infra/StringConvert.h>
#include <math/Image256.h>
#include <ui/Button.h>
#include <ui/Structs/Widget.h>
#include <ui/Structs/Container.h>
#include <ui/Sheet.h>
#include <ui/ScrollSheet.h>
#include <ui/UiWindow.h>
#endif

namespace mud
{
namespace ui
{
	Widget& spacer(Widget& parent)
	{
		return widget(parent, styles().spacer);
	}

	Widget& icon(Widget& parent, cstring icon)
	{
		return item(parent, styles().item, icon);
	}

	Widget& label(Widget& parent, cstring label)
	{
		return item(parent, styles().label, label);
	}

	Widget& title(Widget& parent, cstring label)
	{
		return item(parent, styles().title, label);
	}

	Widget& message(Widget& parent, cstring label)
	{
		return item(parent, styles().message, label);
	}

	Widget& text(Widget& parent, cstring label)
	{
		return item(parent, styles().text, label);
	}

	void button_logic(Widget& self)
	{
		if(MouseEvent mouse_event = self.mouse_event(DeviceType::MouseLeft, EventType::Pressed))
			self.enable_state(PRESSED);
		if(MouseEvent mouse_event = self.mouse_event(DeviceType::MouseLeft, EventType::Released))
			self.disable_state(PRESSED);

		if(MouseEvent mouse_event = self.mouse_event(DeviceType::MouseLeft, EventType::Stroked))
		{
			self.enable_state(ACTIVATED);
			mouse_event.consume(self);
		}
		else
		{
			self.disable_state(ACTIVATED);
		}
	}

	void toggle_logic(Widget& self, bool& on)
	{
		button_logic(self);

		if(self.activated())
			on = !on;

		self.set_state(ACTIVE, on);
	}

	Widget& button(Widget& parent, Style& style, cstring content)
	{
		Widget& self = item(parent, style, content);
		button_logic(self);
		return self;
	}

	Widget& multi_button(Widget& parent, Style& style, array<cstring> elements, Style* element_style)
	{
		Widget& self = multi_item(parent, style, elements, element_style);
		button_logic(self);
		return self;
	}

	Widget& toggle(Widget& parent, Style& style, bool& on, cstring content)
	{
		Widget& self = item(parent, style, content);
		toggle_logic(self, on);
		return self;
	}

	Widget& multi_toggle(Widget& parent, Style& style, bool& on, array<cstring> elements, Style* element_style)
	{
		Widget& self = multi_item(parent, style, elements, element_style);
		toggle_logic(self, on);
		return self;
	}

	Widget& button(Widget& parent, cstring content)
	{
		return button(parent, styles().button, content);
	}

	Widget& multi_button(Widget& parent, array<cstring> elements, Style* element_style)
	{
		return multi_button(parent, styles().multi_button, elements, element_style);
	}

	Widget& toggle(Widget& parent, bool& on, cstring content)
	{
		return toggle(parent, styles().toggle, on, content);
	}

	Widget& multi_toggle(Widget& parent, bool& on, array<cstring> elements, Style* element_style)
	{
		return multi_toggle(parent, styles().multi_button, on, elements, element_style);
	}

	bool modal_button(Widget& screen, Widget& parent, cstring content, uint32_t mode)
	{
		if(button(parent, content).activated())
			screen.m_switch |= mode;
		return (screen.m_switch & mode) != 0;
	}

	bool modal_multi_button(Widget& screen, Widget& parent, array<cstring> elements, uint32_t mode)
	{
		if(multi_button(parent, elements).activated())
			screen.m_switch |= mode;
		return (screen.m_switch & mode) != 0;
	}

	Widget& checkbox(Widget& parent, bool& on)
	{
		return toggle(parent, styles().checkbox, on);
	}

	Widget& fill_bar(Widget& parent, float percentage, Dim dim)
	{
		Widget& self = widget(parent, styles().fill_bar);
		spanner(self, styles().filler, dim, percentage);
		spanner(self, styles().spacer, dim, 1.f - percentage);
		item(self, styles().slider_display, string(to_string(percentage) + "%").c_str());
		return self;
	}

	Widget& image256(Widget& parent, Style& style, cstring name, const Image256& source)
	{
		Widget& self = widget(parent, style);
		Image* image = self.ui_window().find_image(name);
		if(!image)
		{
			std::vector<uint8_t> data = source.read();
			image = &self.ui_window().create_image(name, uvec2{ source.m_width, source.m_height }, &data[0], false);
		}
		self.m_frame.set_icon(image);
		return self;
	}

	Widget& image256(Widget& parent, cstring name, const Image256& source)
	{
		return image256(parent, styles().image, name, source);
	}

	Widget& image256(Widget& parent, cstring name, const Image256& source, const vec2& size)
	{
		Widget& self = image256(parent, styles().image_stretch, name, source);
		//self.m_frame.set_size(size);
		ui::dummy(self, size);
		return self;
	}

	Widget& radio_choice(Widget& parent, cstring value, bool active)
	{
		Widget& self = multi_button(parent, styles().radio_choice, carray<cstring, 1>{ value }, &styles().radio_choice_item);
		self.set_state(ACTIVE, active);
		return self;
	}

	bool radio_switch(Widget& parent, array<cstring> labels, uint32_t& value, Dim dim)
	{
		Widget& self = widget(parent, styles().radio_switch, false, dim);
		bool changed = false;
		for(uint32_t index = 0; index < uint32_t(labels.size()); ++index)
		{
			if(radio_choice(self, labels[index], value == index).activated())
			{
				changed = true;
				value = index;
			}
		}
		return changed;
	}

	Widget& dropdown(Widget& parent, Style& style, cstring value, PopupFlags popup_flags, Style* list_style)
	{ 
		Widget& self = widget(parent, style);
		Widget& header = multi_toggle(self, dropdown_styles().head, self.m_open, carray<cstring, 1>{ value });
		Widget& button = toggle(self, dropdown_styles().toggle, self.m_open);

		self.set_state(HOVERED, header.hovered() || button.hovered());
		self.m_body = nullptr;

		if(self.m_open)
		{
			self.m_body = &popup(self, list_style ? *list_style : dropdown_styles().list, popup_flags);
			self.m_open &= self.m_body->m_open;
		}

		return self;
	}

	Widget& dropdown_choice(Widget& parent, array<cstring> elements, bool active)
	{
		Widget& self = multi_button(parent, dropdown_styles().choice, elements);
		self.set_state(ACTIVE, active);
		return self;
	}

	bool popdown(Widget& parent, array<cstring> choices, uint32_t& value, vec2 position, PopupFlags popup_flags)
	{
		Widget& self = popup_at(parent, dropdown_styles().popdown, position, popup_flags);
		ScrollSheet& sheet = scroll_sheet(self);

		for(uint32_t i = 0; i < uint32_t(choices.size()); ++i)
			if(dropdown_choice(*sheet.m_body, carray<cstring, 1>{ choices[i] }, i == value).activated())
			{
				value = i;
				return true;
			}
		return false;
	}

	bool dropdown_input(Widget& parent, array<cstring> choices, uint32_t& value, bool compact)
	{
		if(value >= choices.size())
			value = uint32_t(choices.size()) - 1;
		Style& style = compact ? dropdown_styles().dropdown_input_compact : dropdown_styles().dropdown_input;
		Widget& self = dropdown(parent, style, value == UINT32_MAX ? "" : choices[value], PopupFlags::AutoModal);
		if(!self.m_body) return false;

		for(uint32_t i = 0; i < uint32_t(choices.size()); ++i)
			if(dropdown_choice(*self.m_body, carray<cstring, 1>{ choices[i] }, value == i).activated())
			{
				value = i;
				self.m_open = false;
				return true;
			}

		return false;
	}

	bool typedown_input(Widget& parent, array<cstring> choices, uint32_t& value)
	{
		bool changed = dropdown_input(parent, choices, value); //dropdown_styles().typedown_input
		//if(scope->m_state & ACTIVATED)
		//	filter_input(self);
		return changed;
	}

	Widget& menu_choice(Widget& parent, array<cstring> elements)
	{
		Widget& self = multi_button(parent, elements); //, menu_styles().choice
		if(self.activated())
			self.m_parent->m_parent->m_open = false;
		return self;
	}

	Widget& menu_choice(Widget& parent, cstring content)
	{
		return menu_choice(parent, carray<cstring, 1>{ content });
	}

	Widget& menu(Widget& parent, cstring label, bool submenu)
	{
		Style& list_style = submenu ? menu_styles().sublist : menu_styles().list;
		return dropdown(parent, menu_styles().menu, label, PopupFlags::Modal, &list_style);
	}

	Widget& menubar(Widget& parent)
	{
		return widget(parent, menu_styles().menubar);
	}

	Widget& toolbutton(Widget& parent, cstring icon)
	{
		return button(parent, icon);
		//string value;
		//return dropdown_input(parent, styles().tool_button, {}, value);
	}

	Widget& tooldock(Widget& parent)
	{
		return widget(parent, toolbar_styles().tooldock);//, GRID)
	}

	Widget& toolbar(Widget& parent, bool wrap)
	{
		Widget& self = widget(parent, wrap ? toolbar_styles().toolbar_wrap : toolbar_styles().toolbar);
		widget(self, toolbar_styles().mover);
		return self;
	}
}
}
