#pragma once
#include <forward_list>
#include <string>
#include "EventTypes.h"

namespace dae
{
	class Listener;
	class GameObject;

	class BroadCaster
	{
	public:
		/*void AddEvent(EventId event);
		void RemoveEvent(EventId event);*/
		void BroadCast(EventId event, GameObject* subject);
		void Register(Listener& listener);
		void UnRegister(Listener* listener);
	private:
		//Event m_event(make_sdbm_hash("PlayerDied"));
		std::forward_list<Listener*> m_subscribers;
	};
}