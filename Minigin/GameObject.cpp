#include <string>

#include "ResourceManager.h"
#include "Renderer.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

#include <iostream>

#include "GameObject.h"

#include "GameComponent.h"


namespace dae
{
	GameObject::GameObject() = default;

	GameObject::~GameObject() = default;

	void GameObject::Update(float deltaTime)
	{
		for (std::unique_ptr<GameComponent>& pComponent : m_components)
		{
			pComponent->Update(deltaTime);
			RenderComponent* pRenderComponent = dynamic_cast<RenderComponent*>(pComponent.get());

			if (pRenderComponent != nullptr)
				pRenderComponent->Render();
		}
	}

	void GameObject::Render() const
	{
		/*const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);*/
		for (std::unique_ptr<GameComponent>  const & pComponent : m_components)
		{
			RenderComponent* pRenderComponent = dynamic_cast<RenderComponent*>(pComponent.get());

			if (pRenderComponent != nullptr)
				pRenderComponent->Render();
		}
	}

	

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.0f);
	}

	Transform const * GameObject::GetTransform() const
	{
		return &m_transform;
	}


	/*template<class T, typename ...Args>
		requires std::is_base_of_v<dae::GameComponent, T>
	inline void GameObject::AddComponent(Args&& ...args)
	{
		m_components.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
	}*/


	void GameObject::RemoveComponent(int index)
	{
		if (index < m_components.size() && index >= 0)
		{
			m_components.erase(m_components.begin() + index);
		}
	}
}
