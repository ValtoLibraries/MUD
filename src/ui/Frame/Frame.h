//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#ifndef MUD_MODULES
#include <infra/NonCopy.h>
#include <type/Unique.h>
#endif
#include <ui/Forward.h>
#include <ui/Frame/UiRect.h>

#ifndef MUD_CPP_20
#include <cmath>
#include <memory>
#endif

namespace mud
{
	using cstring = const char*;

	enum DirtyLayout : unsigned int
	{
		CLEAN,				// Frame doesn't need update
		DIRTY_REDRAW,		// The parent layout has changed
		DIRTY_PARENT,		// The parent layout has changed
		DIRTY_LAYOUT,		// The frame layout has changed
		DIRTY_FORCE_LAYOUT	// The frame layout has changed
	};

	export_ class refl_ MUD_UI_EXPORT Frame : public NonCopy, public UiRect
	{
	public:
		Frame(Frame* parent, Widget& widget);
		virtual ~Frame();

		bool empty() const;

		inline bool opaque() const { return m_opacity == OPAQUE; }
		inline bool hollow() const { return m_opacity == HOLLOW; }

		void set_caption(cstring text);
		void set_icon(Image* image);
		Image* icon() const;
		cstring caption() const;

		Frame& root();
		Layer& layer();

		FrameSolver& solver(Style& style, Dim length = DIM_NONE, Dim2<size_t> index = { 0, 0 });

		DirtyLayout clearDirty() { DirtyLayout dirty = d_dirty; d_dirty = CLEAN; return dirty; }
		void set_dirty(DirtyLayout dirty) { if(dirty > d_dirty) d_dirty = dirty; }
		void mark_dirty(DirtyLayout dirty);

		void update_style(bool reset = false);
		void update_state(WidgetState state);
		void update_inkstyle(InkStyle& inkstyle);

		void set_size(Dim dim, float size);
		void set_span(Dim dim, float span);
		void set_position(Dim dim, float position);

		inline void set_position(const vec2& pos) { set_position(DIM_X, pos.x), set_position(DIM_Y, pos.y); }
		inline void set_size(const vec2& size) { set_size(DIM_X, size.x); set_size(DIM_Y, size.y); }

		// global to local
		void integrate_position(Frame& root, vec2& global);
		inline vec2 integrate_position(const vec2& pos, Frame& root) { vec2 local = pos; integrate_position(root, local); return local; }
		inline vec2 local_position(const vec2& pos) { return integrate_position(pos, root()); }

		// local to global
		void derive_position(Frame& root, vec2& local);
		inline vec2 derive_position(const vec2& pos, Frame& root) { vec2 local = pos; derive_position(root, local); return local; }
		inline vec2 derive_position(const vec2& pos) { return derive_position(pos, root()); }
		inline vec2 absolute_position() { return derive_position({ 0.f, 0.f }); }

		float derive_scale(Frame& root);
		inline float absolute_scale() { return this->derive_scale(root()); }

		void clamp_to_parent();

		vec4 content_rect() const;

		bool inside(const vec2& pos);
		bool inside_abs(const vec2& pos) { return this->inside(local_position(pos)); }

		bool first(const Frame& frame);
		bool last(const Frame& frame);

		void transfer_pixel_span(Frame& prev, Frame& next, Dim dim, float pixelSpan);

		void relayout();

		void sync_solver(FrameSolver& solver);
		void read_solver(FrameSolver& solver);

		void debug_print(bool commit);

	public:
		Widget& d_widget;
		Frame* d_parent;
		DirtyLayout d_dirty = DIRTY_FORCE_LAYOUT;
		Dim2<size_t> d_index = { 0, 0 };

		Opacity m_opacity = CLEAR;

		Style* d_style = nullptr;
		InkStyle* d_inkstyle = nullptr;

	public:
		struct Content;
		object_ptr<Content> d_content;
		object_ptr<FrameSolver> m_solver;

		object_ptr<Layer> m_layer;

		static Vg* s_vg;
	};
}
