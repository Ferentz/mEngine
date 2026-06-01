#pragma once

#include <components/GameComponent.h>
#include <eventSystem/Listener.h>
#include <eventSystem/BroadCaster.h>

namespace dae
{
	class Collider;
}

namespace digger
{
	class Gem : public dae::GameComponent, dae::Listener
	{
		dae::Collider* collider;
	public:
		Gem(dae::GameObject& obj, dae::Collider& col);
		void TuneIn(dae::EventId id, dae::GameObject* subject) override;
		// all it needs to do is record overlap
		dae::BroadCaster signal;
	};
}