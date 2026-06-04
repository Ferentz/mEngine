#pragma once
#include "components/GameComponent.h"
#include <GameObject.h>
#include <glm/glm.hpp>
#include <SceneManager.h>
#include <eventSystem/BroadCaster.h>
#include <eventSystem/EventHash.h>

namespace dae
{

	class Collider final: public GameComponent
	{
		static std::vector<Collider*> s_colliders;

	public:
		Collider(GameObject& object);

		~Collider() override;
		void Start() override
		{

		}
		
		
		void Update(float) override;

		std::vector<Collider*> const& GetCollisions()
		{
			return collisions;
		}

		void ResolveCollisions()
		{

		}

		glm::vec2 size{ 5 };
		bool canCollide{ true };
		bool isTrigger{ false };
		BroadCaster m_signal;

	private:
		
		std::vector<Collider*> collisions{};
		

		bool Overlap(Collider& other);
		bool isPointInsideAABB(glm::vec2 pos2d, glm::vec2 point);

	};

}