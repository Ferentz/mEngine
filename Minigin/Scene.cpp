#include "Scene.h"
#include "Scene.h"
#include "Scene.h"
#include "Scene.h"
#include "Scene.h"
#include "Scene.h"
#include <algorithm>
#include <vector>
#include "Scene.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	auto ptr{ object.get() };
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
	if (ptr->GetParentAsScene() != this) ptr->SetParent(static_cast<Scene&>(*this));
}

void Scene::Remove(const GameObject& object)
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[&object](const auto& ptr) { return ptr.get() == &object; }
		),
		m_objects.end()
	);
}

void Scene::RemoveAllObjects()
{
	m_objects.clear();
}

void dae::Scene::Release(GameObject* object)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[object](const std::unique_ptr<GameObject>& ptr)
		{
			return ptr.get() == object;
		});

	if (it == m_objects.end()) return;

	it->release();
	m_objects.erase(it);
}

bool dae::Scene::HoldsObject(GameObject* object)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[object](const std::unique_ptr<GameObject>& ptr)
		{
			return ptr.get() == object;
		});

	if (it == m_objects.end()) return false;
	
	return true;
}

void dae::Scene::Add(Collider& object)
{
	m_colliders.emplace_back(&object);
}

void dae::Scene::Remove(Collider* object)
{
	std::erase(m_colliders, object);
}

void dae::Scene::CleanUp()
{
	std::erase_if(m_objects, [](std::unique_ptr<GameObject> const& obj) {
		return obj->Cleanup();
		});

}

void Scene::Start() const
{
	for (const auto& object : m_objects)
	{
		object->Start();
	}
}

void Scene::Update(float deltatime)
{
	CleanUp();

	for(auto& object : m_objects)
	{
		object->Update(deltatime);
		if (breakFromScene)
		{
			breakFromScene = false;
			return;
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

