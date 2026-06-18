#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "Scene.h"

#include <iostream>

void dae::SceneManager::CheckForSwitch()
{
	if (switchScene)
	{
		if (m_scenes.size() <= 0) return;

		size_t oldScene = activeScene;
		activeScene = nextScene;
		switchScene = false;
		m_scenes[activeScene]->Start();

		if (!killOldScene) return;

		m_scenes[oldScene].reset();
	}
}

void dae::SceneManager::Update(float deltaTime)
{
	if (m_scenes.size() <= 0) return;
	m_scenes[activeScene]->Update(deltaTime);
}

void dae::SceneManager::FixedUpdate(float deltaTime)
{
	Update(deltaTime);
}

void dae::SceneManager::Render()
{
	if (m_scenes.size() <= 0) return;
	m_scenes[activeScene]->Render();

}

dae::Scene& dae::SceneManager::CreateScene()
{
	for (auto& scene : m_scenes)
	{
		if (scene.get() == nullptr)
		{
			scene.reset( new Scene()); // TODO, see if there is a way to make it so only scene manager can make a scene while not making it imposible for make unique to make a scene
			return *m_scenes.back();
		}
	}
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

dae::Scene* dae::SceneManager::GetActiveScene()
{
	
	if (m_scenes.size() <= 0) return nullptr;
	if (switchScene && m_scenes.size() >= nextScene) return m_scenes[nextScene].get(); // no active scene/in the middle of switching scenes
	if(m_scenes.size() >= activeScene)
	return m_scenes[activeScene].get();

	return nullptr;
	// TODO: insert return statement here
}

void dae::SceneManager::SetActiveScene(size_t i, bool cleanOldScene )
{
	std::cout << "\n switch scene";
	if (i	>= m_scenes.size() && i < 0) return;
	switchScene = true;
	killOldScene = cleanOldScene;
	nextScene = i;
	if (m_scenes.size() <= 0) return;
	m_scenes[activeScene]->BreakFromScene();
}
