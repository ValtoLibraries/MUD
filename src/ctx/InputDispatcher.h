//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#ifndef MUD_MODULES
#include <type/Unique.h>
#include <infra/EnumArray.h>
#endif
#include <ctx/Forward.h>
#include <ctx/KeyCode.h>
#include <ctx/InputEvent.h>

#ifndef MUD_CPP_20
#include <vector>
#include <map>
#endif

namespace mud
{
	export_ template <class T_Element>
	struct EventMap
	{
		EventMap() { m_events = {}; m_keyed_events = {}; }
		
#ifdef MUD_MODULES
		enum_array<DeviceType, enum_array<EventType, T_Element, size_t(EventType::Count)>, size_t(DeviceType::Count)> m_events;
		enum_array<DeviceType, enum_array<EventType, std::map<int, T_Element>, size_t(EventType::Count)>, size_t(DeviceType::Count)> m_keyed_events;
#else
		enum_array<DeviceType, enum_array<EventType, T_Element>> m_events;
		enum_array<DeviceType, enum_array<EventType, std::map<int, T_Element>>> m_keyed_events;
#endif

		void clear()
		{
			m_events = {}; m_keyed_events = {};
			//static EventMap<T_Element> empty;
			//memcpy(this, &empty, sizeof(EventMap<T_Element>));
		}

		T_Element& event(DeviceType device_type, EventType event_type) { return m_events[size_t(device_type)][size_t(event_type)]; }
		T_Element& event(DeviceType device_type, EventType event_type, int key) { return m_keyed_events[size_t(device_type)][size_t(event_type)][key]; }
	};

	export_ struct MUD_CTX_EXPORT EventBatch : public EventMap<InputEvent*>
	{
		ControlNode* m_control_node = nullptr;
		
		EventBatch() {}
	};

	export_ class MUD_CTX_EXPORT EventDispatcher
	{
	public:
		EventDispatcher(ControlNode* control_node);

		void update();

		ControlNode* dispatch_event(InputEvent& event, ControlNode* top_receiver = nullptr);
		void receive_event(InputEvent& event, ControlNode& receiver);

		ControlNode& m_control_node;
		std::vector<EventBatch> m_event_batches;
		size_t m_top = 0;
	};
}
