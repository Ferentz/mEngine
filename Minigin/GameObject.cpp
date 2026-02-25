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

	GameObject::~GameObject() = default;

	void GameObject::Update(float deltaTime)
	{
		if (m_Dirty)
		{
			//variable = (condition) ? expressionTrue : expressionFalse;
			Transform const * pass{ (m_pParent == nullptr) ? nullptr : m_pParent->QueryWorldTransform()};
			m_transform.QueryWorldTransform(pass);
		}
		for (std::unique_ptr<GameComponent>& pComponent : m_components)
		{
			pComponent->Update(deltaTime);
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


	void GameObject::SetParent(GameObject& parent, bool keepWorldPos)
	{
		if (&parent == m_pParent) return;

		if (!parent.AddChild(this)) return;

		if (keepWorldPos)
		{
			//rebase pos
			m_transform.
		}
		else
		{
			this->MakeDirty();
		}
		this->m_pParent = &parent;
		
	}

	bool GameObject::AddChild(GameObject* newChild)
	{
		if (std::find(m_children.begin(), m_children.end(), newChild) != m_children.end())
		{
			return false;
		}

		m_children.push_back(newChild);
		return true;
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y);
		for (std::unique_ptr<GameComponent>& component : m_components)
		{
			component->MakeDirty();
		}
		//m_transform->SetPosition(x, y, 0.0f);
	}

	SmartTransform const * GameObject::GetTransform() const
	{
		return &m_transform;
	}

	Transform const* GameObject::GetLocalTransform() const
	{
		return m_transform.GetLocalTransform();
	}

	Transform const* GameObject::GetWorldTransform() const
	{
		return m_transform.GetWorldTransform();
	}

	Transform const* GameObject::QueryWorldTransform()
	{
		if (m_Dirty)
		{
			m_transform.QueryWorldTransform(nullptr);
			m_Dirty = false;
		}
		return m_transform.GetWorldTransform();
	}

	void GameObject::MakeDirty()
	{
		m_Dirty = true;
		m_transform.MakeDirty();
		for (std::unique_ptr<GameComponent>& pComponent : m_components)
		{
			pComponent->MakeDirty();
		}
		for (GameObject* pChild : m_children)
		{
			pChild->MakeDirty();
		}
	}

	void GameObject::RemoveComponent(size_t index)
	{
		if (index < m_components.size() && index >= 0)
		{
			m_components.erase(m_components.begin() + index);
		}
	}

	void RotatingObject::Update(float deltaTime)
	{
		m_transform.SetRotation(m_transform.GetRotation() + m_rotatingSpeed * deltaTime);
		MakeDirty();
		GameObject::Update(deltaTime);
	}
	void RotatingObject::SetRotationSpeed(float speed)
	{
		m_rotatingSpeed = speed;
	}
}
