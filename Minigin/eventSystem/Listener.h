#pragma once

#include <forward_list>
#include <map>
#include <functional>
#include "EventTypes.h"

namespace dae
{
	class BroadCaster;
	class GameObject;

	class Listener
	{
		std::forward_list<BroadCaster*> m_channels;
		std::map<EventId, std::function<void()>> m_callbacks;

	public:
		void TuneIn(EventId message, GameObject* subject);
		void Subscribe(BroadCaster& source);
		void UnSubscribe(BroadCaster& source);
		void RemoveChannel(BroadCaster* source);

		template <typename T>
		void AddEvent(EventId id, T*obj, void(T::*func)() )
		{
			m_callbacks.emplace(id, std::bind(func, obj));
		}
		void RemoveEvent(EventId id);
	};
}