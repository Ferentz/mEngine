#pragma once
#include "Entity.h"
#include <eventSystem/Listener.h>

namespace dae
{
	class GridMove;
	class Collider;
}
namespace digger
{
	class Digger : public Entity, public dae::Listener
	{
	public :
		Digger(dae::GameObject& obj, dae::GridMove& movement, dae::Collider& collider);

		void TuneIn(dae::EventId id, dae::GameObject* subject) override;
	
		void Die();

		bool IsAllive() { return isAllive; }
	private:
		bool isAllive{true};
		dae::GridMove* gridMove;
	};
}