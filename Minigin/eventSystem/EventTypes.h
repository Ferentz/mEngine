#pragma once
namespace dae
{
	class GameObject;
	struct EventArg {};
	using EventId = unsigned int;
	struct Event {
		EventId id;
		GameObject* subject;
		Event()
			: id{0},
			subject{nullptr}
		{

		}
		explicit Event(EventId _id, GameObject * _subject)
			: id{ _id },
			subject{_subject}
		{}

		Event(Event const& ev) = default;

		Event& operator=(Event& ev) {
			this->id = ev.id;
			this->subject = ev.subject;
			return *this;
		}
	};


}