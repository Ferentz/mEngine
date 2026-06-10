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
		Digger(dae::GameObject& obj, dae::GridMove& movement, dae::Collider& collider, glm::ivec2 base);

		void TuneIn(dae::EventId id, dae::GameObject* subject) override;
	
		void Die() override;
		void Respawn() override;

		bool IsAllive() { return isAllive; }
	private:
		bool isAllive{true};
		dae::GridMove* gridMove;
		
	};
}