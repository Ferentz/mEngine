#pragma once
#include "components/GameComponent.h"
namespace dae
{
	struct EventArg {};
	using EventId = unsigned int;
	struct Event {
		EventId const id;
		GameComponent const * subject;
		explicit Event(EventId _id, GameComponent const * _subject)
			: id{ _id },
			subject{_subject}
		{}
	};


}