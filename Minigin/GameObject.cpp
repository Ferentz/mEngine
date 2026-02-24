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
	GameObject::GameObject()
		:m_transform{ std::make_unique<TransformComponent>(*this) }
	{
	}

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
		m_transform->SetPosition(x, y, 0.0f);
	}

	TransformComponent const * GameObject::GetTransform() const
	{
		return m_transform.get();
	}

	void GameObject::RemoveComponent(size_t index)
	{
		if (index < m_components.size() && index >= 0)
		{
			m_components.erase(m_components.begin() + index);
		}
	}
}
