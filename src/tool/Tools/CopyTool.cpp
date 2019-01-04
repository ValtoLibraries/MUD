//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module mud.tool;
#else
#include <refl/Meta.h>
#include <math/Math.h>
#include <ui/Widget.h>
#include <ui/Frame/Frame.h>
#include <tool/Types.h>
#include <tool/Tools/CopyTool.h>
#endif

namespace mud
{
	CopyAction::CopyAction(array<Transform*> targets)
		: TranslateAction(targets)
		//, m_injector(type<Transform>())
	{}

	void CopyAction::apply(Transform& transform)
	{
		//	m_copies.push_back(&as<Transform>(m_creator.cloneObject(transform)));
		TranslateAction::apply(transform);
	}

	void CopyAction::undo(Transform& transform)
	{
		// m_copies[transform].destroy();
		TranslateAction::undo(transform);
	}

 	CopyTool::CopyTool(ToolContext& context)
		: TransformTool(context, "Copy", type<CopyTool>())
		, m_action()
	{}

	void CopyTool::begin(const vec3& position)
	{
		UNUSED(position);
		//m_action = make_object<CopyAction>(m_targets); // @kludge brute cast
	}

	void CopyTool::update(const vec3& position)
	{
		m_action->update(position, vec3());
		//m_action->apply();
	}

	void CopyTool::end()
	{
		this->commit(std::move(m_action));
	}

	object_ptr<TransformAction> CopyTool::create_action(array<Transform*> targets)
	{
		return make_object<CopyAction>(targets);
	}
}
