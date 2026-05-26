#pragma once

//#include <forward_list>
//#include <map>
//#include <functional>
#include "EventTypes.h"

namespace dae
{
	class GameObject;
	class Listener
	{

	public:
		virtual ~Listener() = default;
		virtual void TuneIn(EventId message, GameObject* subject) = 0;
	};
}