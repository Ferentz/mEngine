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

		auto it = std::find(m_registeredListeners.begin(), m_registeredListeners.end(), &listener);
		if (it == m_registeredListeners.end())
			m_registeredListeners.push_back(&listener);
	}
	/*void EventStack::Unregister(EventId events, Listener*)
	{
	}*/
	void EventStack::Unregister(Listener* listener)
	{
		m_registeredListeners.erase(
			std::remove(m_registeredListeners.begin(), m_registeredListeners.end(), listener),
			m_registeredListeners.end());
	}
	void EventStack::BroadCastEvents()
	{
		size_t idx{ m_broadcastIndex };
		size_t nbrTraversedElements{}; // infinity loop prevemtion
		while ((idx != m_fillIndex) && (nbrTraversedElements <= m_stack.size()))
		{
			Event& event = m_stack[idx];
			for (Listener* listener : m_registeredListeners)
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