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
	public:
		Collider(GameObject& object)
			:GameComponent(object)
		{
			auto scene{ SceneManager::GetInstance().GetActiveScene() };
			if (scene == nullptr) return;

			m_scene = scene;
			scene->Add(*this);
		}

		~Collider() override
		{
			auto scene{ SceneManager::GetInstance().GetActiveScene() };
			if (scene == nullptr) return;

			if (m_scene == scene)
			{
				scene->Remove(this);
			}
			
		}
		
		
		void Update(float) override
		{
			collisions.clear();
			auto scene = SceneManager::GetInstance().GetActiveScene();
			if (scene == nullptr) return;
			auto const & colliders = scene->GetColliders();

			for (auto collider : colliders)
			{
				if (this == collider) continue;
				if (Overlap(*collider))
				{
					collisions.push_back(collider->GetGameObject());
				}
			}
			if (!collisions.empty()) m_signal.BroadCast(make_sdbm_hash("collide"), GetGameObject());
		}
		std::vector<GameObject*> const& GetCollisions()
		{
			return collisions;
		}

		BroadCaster m_signal;

	private:
		glm::vec2 size{5};
		std::vector<GameObject*> collisions{};

		dae::Scene* m_scene{};
		

		bool Overlap(Collider& other)
		{
			auto const & mypos = GetGameObject()->m_transform.GetWorldTransform()->GetPosition();
			auto const& otherPos = other.GetGameObject()->m_transform.GetWorldTransform()->GetPosition();
			
			if (
				isPointInsideAABB(
					glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x, otherPos.y)
				)
				) return true;

			if (
				isPointInsideAABB(
					glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x + other.size.x, otherPos.y)
				)
				) return true;
			if (
				isPointInsideAABB(
					glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x , otherPos.y + other.size.y)
				)
				) return true;
			if (
				isPointInsideAABB(
					glm::vec2(mypos.x, mypos.y), glm::vec2(otherPos.x + other.size.x, otherPos.y + other.size.y)
				)
				) return true;

			return false;
		}
		bool isPointInsideAABB(glm::vec2 pos2d, glm::vec2 point) {
			return 
				point.x >= pos2d.x &&
				point.x <= pos2d.x+ size.x &&
				point.y >= pos2d.y &&
				point.y <= pos2d.y + size.y
				;
		}

	};
}