//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module mud.ui;
#else
#include <infra/Reverse.h>
#include <ui/Frame/Solver.h>
#include <ui/Style/Style.h>
#include <ui/Structs/Widget.h>
#include <ui/Frame/Caption.h>
#include <ui/Frame/Layer.h>
#endif

namespace mud
{
	void collect_solvers(Frame& frame, SolverVector& solvers, DirtyLayout dirtyTop)
	{
		if(dirtyTop >= DIRTY_FORCE_LAYOUT)
			frame.set_dirty(DIRTY_FORCE_LAYOUT);
		else if(dirtyTop >= DIRTY_LAYOUT)
			//frame.set_dirty(DIRTY_PARENT);
			frame.set_dirty(DIRTY_LAYOUT); // @kludge because relayout with DIRTY_PARENT isn't always correct due to size / content issue

		if(!frame.d_dirty)
			return;

		//this->debugPrintDepth();
		//printf(" >> %s %s\n", d_style->m_name.c_str(), to_string(d_dirty).c_str());

		if(frame.d_dirty >= DIRTY_PARENT)
		{
			frame.m_solver->collect(solvers);
		}

		if(frame.d_dirty >= DIRTY_REDRAW)
		{
			frame.layer().setRedraw();
			frame.layer().setForceRedraw(); // @ kludge for nodes in canvas when moving the canvas window
		}

		for(auto& widget : frame.d_widget.m_nodes)
			collect_solvers(widget->m_frame, solvers, frame.d_dirty);

		frame.clearDirty();
	}

	void relayout(SolverVector& solvers)
	{
		//for(FrameSolver* solver : solvers)
		//	solver->sync();

		for(FrameSolver* solver : reverse_adapt(solvers))
			solver->compute();

		for(FrameSolver* solver : solvers)
			solver->layout();

		for(FrameSolver* solver : solvers)
			solver->read();
	}

	Space Space::preset(SpacePreset preset)
	{
		if     (preset == SHEET)  return { PARAGRAPH,  WRAP,   WRAP };
		else if(preset == FLEX)	  return { PARALLEL,   WRAP,   WRAP };
		else if(preset == ITEM)   return { READING,    SHRINK, SHRINK };
		else if(preset == UNIT)   return { PARAGRAPH,  SHRINK, SHRINK };
		else if(preset == BLOCK)  return { PARAGRAPH,  FIXED,  FIXED };
		else if(preset == LINE)   return { READING,    WRAP,   SHRINK };
		else if(preset == STACK)  return { PARAGRAPH,  SHRINK, WRAP };
		else if(preset == DIV)    return { ORTHOGONAL, WRAP,   SHRINK };
		else if(preset == SPACER) return { PARALLEL,   WRAP,   SHRINK };
		else if(preset == BOARD)  return { READING,    EXPAND, EXPAND };
		else if(preset == LAYOUT) return { PARAGRAPH,  EXPAND, EXPAND };
		else 					  return { PARAGRAPH,  WRAP,   WRAP };
	}

	float AlignSpace[5] = { 0.f, 0.5f, 1.f, 0.f, 1.f };
	float AlignExtent[5] = { 0.f, 0.5f, 1.f, 1.f, 0.f };

	FrameSolver::FrameSolver(FrameSolver* solver, Layout* layout, Frame* frame)
		: d_frame(frame)
		, d_parent(solver)
		, d_style(layout)
		, m_solvers{ solver ? &solver->solver(*this, DIM_X) : nullptr, solver ? &solver->solver(*this, DIM_Y) : nullptr }
		, d_grid(solver ? solver->grid() : nullptr)
	{
		if(d_style)
			this->applySpace();
	}

	FrameSolver& FrameSolver::solver(FrameSolver& frame, Dim dim)
	{
		if(dim == d_length && d_grid && frame.d_frame && frame.d_parent != d_grid) // && !frame.d_style->m_no_grid
			return d_grid->solver(frame, dim);
		return *this;
	}

	void FrameSolver::applySpace(Dim length)
	{
		const Space& space = d_style->m_space;

		if(length != DIM_NONE)
			d_length = length;
		else if(space.direction == ORTHOGONAL)
			d_length = flip_dim(d_parent->d_length);
		else if(space.direction == PARALLEL)
			d_length = d_parent->d_length;
		else if(space.direction == READING)
			d_length = DIM_X;
		else if(space.direction == PARAGRAPH)
			d_length = DIM_Y;

		d_depth = flip_dim(d_length);

		d_sizing[d_length] = space.sizingLength;
		d_sizing[d_depth] = space.sizingDepth;
	}

	void FrameSolver::collect(SolverVector& solvers)
	{
		this->reset();
		this->sync();
		solvers.push_back(this);
	}

	void FrameSolver::sync()
	{
		if(d_frame)
			d_frame->sync_solver(*this);
	}

	void FrameSolver::compute()
	{
		if(!d_parent) return;
		m_solvers[DIM_X]->compute(*this, DIM_X);
		m_solvers[DIM_Y]->compute(*this, DIM_Y);

#if 0 // DEBUG
		if(!d_frame) return;
		d_frame->debug_print(false);
		printf(" LAYOUT measured content size %i , %i\n", int(d_content.x), int(d_content.y));
#endif
	}

	void FrameSolver::layout()
	{
		if(!d_parent) return;
		m_solvers[DIM_X]->layout(*this, DIM_X);
		m_solvers[DIM_Y]->layout(*this, DIM_Y);

#if 0 // DEBUG
		if(!d_frame) return;
		d_frame->debug_print(false);
		printf(" size %i , %i ", int(m_size.x), int(m_size.y));
		printf(" position %i , %i\n", int(m_position.x), int(m_position.y));
#endif
	}

	void FrameSolver::read()
	{
		if(d_frame)
			d_frame->read_solver(*this);
	}

	void FrameSolver::compute(FrameSolver& frame, Dim dim)
	{
		UNUSED(frame); UNUSED(dim);
	}

	void FrameSolver::layout(FrameSolver& frame, Dim dim)
	{
		UNUSED(frame); UNUSED(dim);
	}

	RowSolver::RowSolver(FrameSolver* solver, Layout* layout, Frame* frame)
		: FrameSolver(solver, layout, frame)
	{}

	void RowSolver::compute(FrameSolver& frame, Dim dim)
	{
		if(dim == d_length && frame.flow() && frame.d_sizing[d_length] >= WRAP)
			d_totalSpan += frame.m_span[d_length];

		if(!frame.sizeflow())
			return;

		if(frame.d_sizing[dim] <= WRAP)
			this->measure(frame, dim);

		if(frame.d_sizing[dim] <= SHRINK && frame.flow() && dim == d_length)
			m_spaceContent[dim] += frame.dbounds(dim);

		if(dim == d_length && frame.d_sizing[d_length] >= WRAP)
			d_contentExpand = true;
	}

	void RowSolver::measure(FrameSolver& frame, Dim dim)
	{
		if(dim == d_length && frame.flow())
		{
			d_content[dim] += frame.dbounds(dim) + (d_count++ ? this->spacing() : 0.f);
			d_spacing[dim] += frame.dmargin(dim) * 2.f + (d_count++ ? this->spacing() : 0.f);
		}
		else
			d_content[dim] = max(d_content[dim], frame.dbounds(dim));
	}

	void RowSolver::layout(FrameSolver& frame, Dim dim)
	{
		if(dim == d_length && frame.flow() && frame.d_sizing[d_length] >= WRAP)
			frame.m_span[d_length] = frame.m_span[d_length] / d_totalSpan;

		this->resize(frame, dim);

		if(frame.posflow())
			this->position(frame, dim);
	}

	void RowSolver::resize(FrameSolver& frame, Dim dim)
	{
		if(d_style->m_layout[dim] < AUTO_SIZE)
			return;

		float space = this->dspace(dim);
		//bool hasSpace = space > d_content[dim]; // @todo: implement scarcity check, current behavior when scarce is wrong
		if(dim == d_length && frame.flow())
		{
			float spacings = d_spacing[dim];
			space -= (m_spaceContent[dim] + spacings);
			space *= frame.m_span[d_length];
		}
		else
		{
			space -= frame.dmargin(dim) * 2.f;
		}

		float content = frame.dcontent(dim);

		Sizing sizing = frame.d_sizing[dim];
		if(sizing == SHRINK)
			frame.m_size[dim] = content;
		else if(sizing == WRAP)
			frame.m_size[dim] = max(content, space);
		else if(sizing == EXPAND)
			frame.m_size[dim] = space;
	}

	void RowSolver::position(FrameSolver& frame, Dim dim)
	{
		if(d_style->m_layout[dim] < AUTO_LAYOUT)
			return;

		float space = this->dspace(dim);

		if(dim == d_length && frame.flow())
			frame.m_position[dim] = this->positionSequence(frame, d_contentExpand ? 0.f : space - d_content[d_length]);
		else
			frame.m_position[dim] = this->positionFree(frame, dim, space);

		if(dim == d_length && frame.flow())
			d_prev = &frame;
	}

	float RowSolver::positionFree(FrameSolver& frame, Dim dim, float space)
	{
		Align align = frame.dalign(dim);// == d_length ? DIM_X : DIM_Y);
		float alignOffset = space * AlignSpace[align] - frame.dextent(dim) * AlignExtent[align];
		return (frame.flow() ? dpadding(dim) + frame.dmargin(dim) : 0.f) + alignOffset;
	}

	float RowSolver::positionSequence(FrameSolver& frame, float space)
	{
		auto alignSequence = [&](FrameSolver& frame, float space) { return space * AlignSpace[frame.dalign(d_length)]; };
		if(d_prev)
			return d_prev->doffset(d_length) - alignSequence(*d_prev, space) + this->spacing() + alignSequence(frame, space);
		else
			return dpadding(d_length) + frame.dmargin(d_length) + alignSequence(frame, space);
	}

	CustomSolver::CustomSolver(FrameSolver* solver, Layout* layout, Frame* frame)
		: RowSolver(solver, layout, frame)
	{}

	void CustomSolver::collect(SolverVector& solvers)
	{
		FrameSolver::collect(solvers);

		for(auto& solver : m_solvers)
			solver->collect(solvers);
	}

	static Layout& gridOverlayStyle()
	{
		static Layout style;
		//style.m_space = { READING, EXPAND, EXPAND };
		style.m_space = { READING, WRAP, WRAP };
		style.m_flow = OVERLAY;
		style.m_spacing = { 2.f, 2.f };
		return style;
	}

	static Layout& columnSolverStyle()
	{
		static Layout style;
		//style.m_space = { PARAGRAPH, EXPAND, EXPAND };
		style.m_space = { PARAGRAPH, WRAP, WRAP };
		style.m_layout = { AUTO_LAYOUT, NO_LAYOUT };
		return style;
	}

	TableSolver::TableSolver(FrameSolver* solver, Layout* layout, Frame* frame)
		: CustomSolver(solver, layout, frame)
	{
		this->divide(layout->m_table_division);
	}

	void TableSolver::divide(const std::vector<float>& columns)
	{
		m_solvers.clear();
		m_solvers.emplace_back(make_unique<RowSolver>(this, &gridOverlayStyle()));
		for(size_t i = 0; i < columns.size(); ++i)
		{
			m_solvers.emplace_back(make_unique<RowSolver>(m_solvers.front().get(), &columnSolverStyle()));
			m_solvers.back()->m_span = { columns[i], 0.f };
		}
	}

	void TableSolver::update(const std::vector<float>& columns)
	{
		if(m_solvers.size() != columns.size() + 1)
			return divide(columns);

		for(size_t i = 0; i < columns.size(); ++i)
			m_solvers[1 + i]->m_span[d_depth] = columns[i];
	}

	FrameSolver& TableSolver::solver(FrameSolver& frame, Dim dim)
	{
		UNUSED(dim);
		if(frame.d_frame && frame.d_parent != this && !frame.d_parent->d_style->m_no_grid)
		{
			//size_t column0 = frame.d_frame->dindex(d_depth);
			size_t column = frame.d_frame->d_widget.m_index;
			frame.d_frame->d_index[d_depth] = column;
			return 1 + column < m_solvers.size() ? *m_solvers[1 + column] : *this;
		}
		return *this;
	}

	LineSolver::LineSolver(FrameSolver* solver, Space space)
		: RowSolver(solver, nullptr)
		, d_style()
	{
		d_style.m_space = space;
		d_style.m_space.direction = READING;
		FrameSolver::d_style = &d_style;
		this->applySpace();
	}

	GridSolver::GridSolver(FrameSolver* solver, Layout* layout, Frame* frame)
		: CustomSolver(solver, layout, frame)
	{
		this->divide(layout->m_grid_division);
	}

	void GridSolver::divide(std::vector<Space> lines)
	{
		m_solvers.clear();
		for(size_t i = 0; i < lines.size(); ++i)
			m_solvers.emplace_back(make_unique<LineSolver>(this, lines[i]));
	}

	FrameSolver& GridSolver::solver(FrameSolver& frame, Dim dim)
	{
		UNUSED(dim);
		if(frame.d_frame)
		{
			size_t row = frame.d_frame->d_index[d_length];
			return *m_solvers[row];
		}
		return *this;
	}
}
