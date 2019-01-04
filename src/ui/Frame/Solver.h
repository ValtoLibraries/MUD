//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#ifndef MUD_MODULES
#include <infra/NonCopy.h>
#endif
#include <ui/Frame/Frame.h>
#include <ui/Style/Layout.h>

#ifndef MUD_CPP_20
#include <vector>
#endif

namespace mud
{
	using SolverVector = std::vector<FrameSolver*>;

	void collect_solvers(Frame& frame, SolverVector& solvers, DirtyLayout dirtyTop);
	void relayout(SolverVector& solvers);

	export_ class refl_ MUD_UI_EXPORT FrameSolver : public UiRect
	{
	public:
		FrameSolver(FrameSolver* solver, Layout* layout, Frame* frame = nullptr);
		virtual ~FrameSolver() {}

		inline bool flow() { return d_style->m_flow == FLOW; }
		inline bool posflow() { return d_style->m_flow <= ALIGN; }
		inline bool sizeflow() { return d_style->m_flow <= OVERLAY; }

		inline float dpadding(Dim dim) { return d_style->m_padding[dim]; }
		inline float dbackpadding(Dim dim) { return d_style->m_padding[dim + 2]; }
		inline float dmargin(Dim dim) { return d_style->m_margin[dim]; }

		inline Align dalign(Dim dim) { return d_style->m_align[dim]; }

		inline float dcontent(Dim dim) { return d_content[dim] + dpadding(dim) + dbackpadding(dim); }
		inline float dbounds(Dim dim) { return dcontent(dim) + dmargin(dim) * 2.f; }

		inline float dextent(Dim dim) { return m_size[dim] + dmargin(dim) * 2.f; }
		inline float doffset(Dim dim) { return m_position[dim] + m_size[dim] + dmargin(dim); }
		inline float dspace(Dim dim) { return m_size[dim] - dpadding(dim) - dbackpadding(dim); }

		//inline float spacing(FrameSolver& frame) { return d_prev ? d_style->m_spacing[d_length] : 0.f; }
		inline float spacing() { return d_style->m_spacing[d_length]; }

		void setup(const vec2& position, const vec2& size, const vec2& span, const vec2* content)
		{
			m_position = position;
			m_span = span;
			m_size = size;

			if(d_sizing.x == FIXED) d_content.x = (content ? content->x : m_size.x - dpadding(DIM_X) - dbackpadding(DIM_X));
			if(d_sizing.y == FIXED) d_content.y = (content ? content->y : m_size.y - dpadding(DIM_Y) - dbackpadding(DIM_Y));
			if(d_sizing.x == FIXED) m_size.x = d_content.x + dpadding(DIM_X) + dbackpadding(DIM_X);
			if(d_sizing.y == FIXED) m_size.y = d_content.y + dpadding(DIM_Y) + dbackpadding(DIM_Y);
		}

		void reset(bool partial = false)
		{
			m_size = { 0.f, 0.f };
			if(!partial)
				d_content = { 0.f, 0.f };
			d_spacing = { 0.f, 0.f };
			m_spaceContent = { 0.f, 0.f };
			d_contentExpand = false;
			d_totalSpan = 0.f;
			d_prev = nullptr;
			d_count = 0;
		}

		void applySpace(Dim length = DIM_NONE);

		virtual void collect(SolverVector& solvers);

		virtual FrameSolver& solver(FrameSolver& frame, Dim dim);
		virtual FrameSolver* grid() { return nullptr; }

		void sync();
		void compute();
		void layout();
		void read();

		virtual void compute(FrameSolver& frame, Dim dim);
		virtual void layout(FrameSolver& frame, Dim dim);

	public:
		Frame* d_frame;
		FrameSolver* d_parent;
		Layout* d_style;

		FrameSolver* m_solvers[2];
		FrameSolver* d_grid;

		Dim d_length = DIM_NONE;
		Dim d_depth = DIM_NONE;

		Dim2<Sizing> d_sizing = { SHRINK, SHRINK };

		vec2 d_content = { 0.f, 0.f };
		vec2 d_spacing = { 0.f, 0.f };
		vec2 m_spaceContent = { 0.f, 0.f };
		bool d_contentExpand = false;
		float d_totalSpan;

		Dim2<size_t> d_index;

		FrameSolver* d_prev = nullptr;
		size_t d_count = 0;
	};

	export_ class refl_ MUD_UI_EXPORT RowSolver : public FrameSolver
	{
	public:
		RowSolver(FrameSolver* solver, Layout* layout, Frame* frame = nullptr);

		virtual void compute(FrameSolver& frame, Dim dim);
		virtual void layout(FrameSolver& frame, Dim dim);

	protected:
		void measure(FrameSolver& frame, Dim dim);
		void resize(FrameSolver& frame, Dim dim);
		void position(FrameSolver& frame, Dim dim);

		float positionFree(FrameSolver& frame, Dim dim, float space);
		float positionSequence(FrameSolver& frame, float space);
	};

	export_ class MUD_UI_EXPORT CustomSolver : public NonCopy, public RowSolver
	{
	public:
		CustomSolver(FrameSolver* solver, Layout* layout, Frame* frame = nullptr);

		virtual void collect(SolverVector& solvers);

	protected:
		std::vector<unique_ptr<FrameSolver>> m_solvers;
	};

	export_ class refl_ MUD_UI_EXPORT TableSolver : public CustomSolver
	{
	public:
		TableSolver(FrameSolver* solver, Layout* layout, Frame* frame = nullptr);

		void divide(const std::vector<float>& spans);
		void update(const std::vector<float>& spans);

		virtual FrameSolver& solver(FrameSolver& frame, Dim dim);
		virtual FrameSolver* grid() { return this; }
	};

	export_ class refl_ MUD_UI_EXPORT LineSolver : public RowSolver
	{
	public:
		LineSolver(FrameSolver* solver, Space space);
		LineSolver(const LineSolver& other) = delete; // @kludge for clang modules bug
		LineSolver& operator=(const LineSolver& other) = delete;
		
		Layout d_style;
	};

	export_ class refl_ MUD_UI_EXPORT GridSolver : public CustomSolver
	{
	public:
		GridSolver(FrameSolver* solver, Layout* layout, Frame* frame = nullptr);

		void divide(std::vector<Space> spaces);

		virtual FrameSolver& solver(FrameSolver& frame, Dim dim);
	};
}
