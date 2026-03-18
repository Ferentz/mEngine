#pragma once
#include "eventSystem/EventTypes.h"
#include <vector>
#include <map>

namespace dae
{
	class Listener;

	class EventStack
	{
	public:
		EventStack(size_t size);
		void PushEvent(Event event);
		void Register(EventId events, Listener&);
		void Unregister(EventId events, Listener*);
		void Unregister(Listener*);

	private:
		void BroadCastEvent(size_t index);
		size_t m_index;
		std::vector<Event> m_stack;
		std::vector<EventId> m_registeredEvents;
		std::vector<std::vector<Listener*>>> m_registeredListeners;
	};
}