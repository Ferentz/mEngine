#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();
		Scene* GetActiveScene();
		void SetActiveScene(size_t i, bool cleanOldScene = true);

		void CheckForSwitch();
		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render();

	private:
		bool switchScene{true};
		bool killOldScene{};
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		size_t activeScene{};
		size_t nextScene{};
		std::vector<std::unique_ptr<Scene>> m_scenes{};
	};
}
