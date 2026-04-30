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
		static EventStack& GetEventStack();
		void PushEvent(Event event);
		void Register(Listener& listener);
		//void Unregister(EventId events, Listener*);
		void Unregister(Listener* listener);
		void BroadCastEvents();

	private:
		EventStack(size_t size);
		
		size_t m_broadcastIndex{0};
		size_t m_fillIndex{0};
		std::vector<Event> m_stack;
		//std::vector<EventId> m_registeredEvents;
		std::vector<Listener*> m_registeredListeners;

		//think on how to best make it so a listener can pick up an event that allows the broadcasting to happen threaded.
		/*bool m_eventGotPickedUp{ false };
		Listener * */
	};
}