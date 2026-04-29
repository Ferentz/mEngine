#include "BroadCaster.h"
#include "Listener.h"
#include "GameObject.h"

namespace dae
{
	void BroadCaster::BroadCast(EventId event, GameObject* subject)
	{
		for (Listener* listener : m_subscribers)
		{
			listener->TuneIn(event, subject); //TuneIn(event, subject);
		}
	}
	void BroadCaster::Register(Listener& listener)
	{
		m_subscribers.push_front(&listener);
	}
	void BroadCaster::UnRegister(Listener* listener)
	{
		m_subscribers.remove(listener);
	}
}