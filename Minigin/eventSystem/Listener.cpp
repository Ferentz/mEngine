#include "Listener.h"
#include "GameObject.h"
#include "BroadCaster.h"

namespace dae
{
	void Listener::TuneIn(EventId message, GameObject*)
	{
		if(m_callbacks.contains(message))
		m_callbacks[message]();
	}
	
	void Listener::Subscribe(BroadCaster& source)
	{
		source.Register(*this);
		m_channels.push_front(&source);
	}
	void Listener::UnSubscribe(BroadCaster& source)
	{
		source.UnRegister(this);
		RemoveChannel(&source);
	}
	void Listener::RemoveChannel(BroadCaster* source)
	{
		m_channels.remove(source);
	}

	
	void Listener::RemoveEvent(EventId id)
	{
		m_callbacks.erase(id);
	}
	
}