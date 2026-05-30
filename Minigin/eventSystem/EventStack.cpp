#include "EventStack.h"
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

	EventStack::~EventStack()
	{
		m_persistentListeners.clear();
		m_specificListenerMap.clear();
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
		if (listener == nullptr) return;
		if (!m_persistentListeners.empty())
		{
			std::erase(m_persistentListeners, listener);
		}
		/*if (m_specificListenerMap.bucket_count() <= 0) return;
		for (auto & it : m_specificListenerMap)
		{
			std::erase(it.second, listener);
		}*/

		if (m_specificListenerMap.empty())
			return;

		for (auto it = m_specificListenerMap.begin();
			it != m_specificListenerMap.end();
			++it)
		{
			std::erase(it->second, listener);
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