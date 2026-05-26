#include "EventStack.h"
#include "eventSystem/EventStack.h"
#include "eventSystem/Listener.h"

#include <algorithm>

namespace dae
{
	EventStack& EventStack::GetEventStack()
	{
		static EventStack inst{ 100 };
		return inst;
	}


	EventStack::EventStack(size_t seize)
	{
		m_stack.resize(seize);
	}

	void EventStack::PushEvent(Event event)
	{
		m_stack[m_fillIndex] = event; // copy int and adress
		size_t tempFIllIDX = m_fillIndex;
		tempFIllIDX++;
		if (tempFIllIDX >= m_stack.size())
		{
			tempFIllIDX = 0;
		}

		if (tempFIllIDX == m_broadcastIndex) return; // no overfilling

		m_fillIndex = tempFIllIDX;

	}
	void EventStack::Register( Listener& listener)
	{
		auto it = std::find(m_persistentListeners.begin(), m_persistentListeners.end(), &listener);
		if (it == m_persistentListeners.end())
			m_persistentListeners.push_back(&listener);
	}
	void EventStack::Register(Listener& listener, EventId event)
	{

		if (std::find(m_persistentListeners.begin(),
			m_persistentListeners.end(),
			&listener) != m_persistentListeners.end())
		{
			return;
		}

		auto& listeners = m_specificListenerMap[event]; // if not yet in map, make

		if (std::find(listeners.begin(), listeners.end(), &listener) == listeners.end())
		{
			listeners.push_back(&listener);
		}

	}
	/*void EventStack::Unregister(EventId events, Listener*)
	{
	}*/
	void EventStack::Unregister(Listener* listener)
	{
		m_persistentListeners.erase(
			std::remove(m_persistentListeners.begin(), m_persistentListeners.end(), listener),
			m_persistentListeners.end());

		if (listener == nullptr) return;

		std::erase(m_persistentListeners, listener);

		for (auto it : m_specificListenerMap)
		{
			std::erase(it.second, listener);
		}
	}
	void EventStack::BroadCastEvents()
	{
		size_t idx{ m_broadcastIndex };
		size_t nbrTraversedElements{}; // infinity loop prevemtion
		while ((idx != m_fillIndex) && (nbrTraversedElements <= m_stack.size()))
		{
			Event& event = m_stack[idx];
			for (Listener* listener : m_persistentListeners)
			{
				listener->TuneIn(event.id, event.subject);
			}

			auto& listeners = m_specificListenerMap[event.id]; // if not yet in map, make
			for (Listener* listener : listeners)
			{
				listener->TuneIn(event.id, event.subject);
			}

			event.id = 0;
			event.subject = nullptr;

			++idx;
			++nbrTraversedElements;
		}
	}
}