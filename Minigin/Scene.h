#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"
//#include "collision/Collider.h"

namespace dae
{
	class Collider;

	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Remove(const GameObject& object);
		void RemoveAllObjects();

		void Add(Collider& object);
		void Remove(Collider* object);

		void Update(float deltaTime);
		void Render() const;

		std::vector < std::unique_ptr<GameObject>> const& GetObjects(){ return m_objects; }
		std::vector <Collider*> const& GetColliders() { return m_colliders; }

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void BreakFromScene() { breakFromScene = true; }

	private:
		bool breakFromScene{};
		friend class SceneManager;
		explicit Scene() = default;

		std::vector <Collider*> m_colliders{};
		std::vector < std::unique_ptr<GameObject>> m_objects{};
		
	};

}
