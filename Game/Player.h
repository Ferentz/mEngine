#pragma once
#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

namespace dae
{
	class GridMove;
	class Collider;
}
namespace digger
{
	class Player : public dae::GameComponent, public dae::Listener
	{
	public :
		Player(dae::GameObject& obj, dae::GridMove& movement, dae::Collider& collider);

		void TuneIn(dae::EventId id, dae::GameObject* subject) override;
	private:
		dae::GridMove* gridMove;
	};
}