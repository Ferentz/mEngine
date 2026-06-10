#include "Gem.h"

#include <collision/Collider.h>
#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>

namespace digger
{
	Gem::Gem(dae::GameObject& obj, dae::Collider& col)
		:dae::GameComponent(obj)
		, collider{ &col }
	{
		collider->m_signal.Register(*this);
		GetGameObject()->objectName = "tile";
	}

	void digger::Gem::TuneIn(dae::EventId , dae::GameObject* )
	{
		dae::EventStack::GetEventStack().PushEvent(
			dae::Event(dae::make_sdbm_hash("collect gem"), collider->GetCollisions()[0]->GetGameObject())
		);

		GetGameObject()->MarkForDelete();
	}
}

