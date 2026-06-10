#pragma once
//#include <components/GameComponent.h>
#include "Entity.h"
#include <eventSystem/Listener.h>
#include <eventSystem/BroadCaster.h>

//#include "Entity.h"

namespace dae
{
	class GameObject;
	class GridMove;
	class Collider;
	class TextureComponent;
}

namespace digger
{
	class Spawner;

	class Nobbin : public Entity , public dae::Listener
	{
	public:
		Nobbin(dae::GameObject& parent, dae::GridMove& movement, dae::Collider& collider, glm::ivec2 base, Spawner * origin, dae::TextureComponent& texture);

		void Update(float) override;

		void TuneIn(dae::EventId id, dae::GameObject* subject) override;

		void Die() override;
		void Respawn() override;

		bool CanDig() { return canDig; }

		bool IsAllive() { return isAllive; }

		//bool removeOnDie{ true };

		dae::BroadCaster signal{};
	private:
		bool isAllive{ true };
		bool canDig{ false };
		float timer{};
		float const digTime{ 3.f };
		float noDigTime{10.f};
		dae::GridMove* gridMove;
		Spawner* home;
		dae::TextureComponent* texture;
	};
}