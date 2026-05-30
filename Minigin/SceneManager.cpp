#include "SceneManager.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::FixedUpdate(float deltaTime)
{
	Update(deltaTime);
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

dae::Scene* dae::SceneManager::GetActiveScene()
{
	if(m_scenes.size() >= activeScene)
	return m_scenes[activeScene].get();

	return nullptr;
	// TODO: insert return statement here
}
