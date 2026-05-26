#pragma once
#include "eventSystem/EventTypes.h"
#include <vector>
//#include <map>
#include<unordered_map>

namespace dae
{
	class Listener;

	class EventStack
	{
	public:
		static EventStack& GetEventStack();
		void PushEvent(Event event);
		void Register(Listener& listener);
		void Register(Listener& listener, EventId event);
		//void Unregister(EventId events, Listener*);
		void Unregister(Listener* listener);
		void BroadCastEvents();

	private:
		EventStack(size_t size);
		
		size_t m_broadcastIndex{0};
		size_t m_fillIndex{0};
		std::vector<Event> m_stack;
		//std::vector<EventId> m_registeredEvents;
		std::unordered_map<EventId, std::vector<Listener*>> m_specificListenerMap{};
		std::vector<Listener*> m_persistentListeners;

		//think on how to best make it so a listener can pick up an event that allows the broadcasting to happen threaded.
		/*bool m_eventGotPickedUp{ false };
		Listener * */
	};
}