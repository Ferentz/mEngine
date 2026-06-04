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
		void Release(GameObject* object);
		bool HoldsObject(GameObject* object);

		void CleanUp();

		void Start() const;

		void Update(float deltaTime);
		void Render() const;

		std::vector < std::unique_ptr<GameObject>> const& GetObjects(){ return m_objects; }

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

		std::vector < std::unique_ptr<GameObject>> m_objects{};
		
	};

}
