#include "eventSystem/EventStack.h"
#include "eventSystem/Listener.h"
namespace dae
{
	EventStack::EventStack(size_t seize)
	{

	}
	void EventStack::PushEvent(Event event)
	{
		m_stack[m_index] = event; // copy int and adress
		m_index++;
		if (m_index >= m_stack.size())
		{
			m_index = 0;
		}
	}
	void EventStack::Register(EventId events, Listener&)
	{
	}
	void EventStack::Unregister(EventId events, Listener*)
	{
	}
	void EventStack::Unregister(Listener*)
	{
	}
	void EventStack::BroadCastEvent(size_t index)
	{
		for (size_t i{}; i < m_registeredEvents.size(); i++)
		{
			for (Listener* listener : m_registeredListeners[i])
			{
				listener->TuneIn(m_registeredEvents[i]);
			}
		}
	}
}