#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	if (switchScene)
	{
		activeScene = nextScene;
		switchScene = false;
	}
	m_scenes[activeScene]->Update(deltaTime);
}

void dae::SceneManager::FixedUpdate(float deltaTime)
{
	Update(deltaTime);
}

void dae::SceneManager::Render()
{
	m_scenes[activeScene]->Render();

}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

dae::Scene* dae::SceneManager::GetActiveScene()
{
	if(m_scenes.size() >= nextScene)
	return m_scenes[nextScene].get();

	return nullptr;
	// TODO: insert return statement here
}

void dae::SceneManager::SetActiveScene(int i)
{
	if (i >= m_scenes.size() && i < 0) return;
	switchScene = true;
	nextScene = i;
	m_scenes[activeScene]->BreakFromScene();
}
