#include "GameObject.h"
#include <string>

#include "ResourceManager.h"
#include "Renderer.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Font.h"
#include "Texture2D.h"

#include <iostream>

#include "GameObject.h"

//#include "components/GameComponent.h"
//#include "components/RenderComponent.h"


namespace dae
{
	GameObject::GameObject()
		:m_transform{this}
	{

	}
	GameObject::~GameObject() = default;

	void GameObject::Update(float deltaTime)
	{
		
		for (std::unique_ptr<GameComponent>& pComponent : m_components)
		{
			pComponent->Update(deltaTime);
		}
		//if (m_Dirty)
		//{
		//	//variable = (condition) ? expressionTrue : expressionFalse;
		//	Transform const* pass{ (m_pParent == nullptr) ? nullptr : m_pParent->QueryWorldTransform() };
		//	m_transform.QueryWorldTransform(pass);
		//}
	}

	void GameObject::Render()
	{
		// TODO change

		for (RenderComponent* pRenderComponent : m_renderComponents)
		{
			//RenderComponent* pRenderComponent = dynamic_cast<RenderComponent*>(pComponent.get());

			if (pRenderComponent != nullptr)
				pRenderComponent->Render();
		}
	}

	GameObject* const GameObject::GetParent()
	{
		return m_pParent;
	}

	void GameObject::SetParent(GameObject *newParent, bool keepWorldPos)
	{
		// if the new parent is already the parent of the object
		if (newParent == this->m_pParent) return;

		// if the new parent is a child of the object, works for nullptr
		if (this->IsChild(newParent)) return;
		
		//rebase transforms
		if (keepWorldPos)
		{
			m_transform.ReconstructLocal(newParent);
		}
		this->MakeDirty();


		//remove from old parent
		if (m_pParent) m_pParent->RemoveChild(this);
		// set new parent;
		this->m_pParent = newParent;
		if (m_pParent) m_pParent->AddChild(this);
	}

	void GameObject::AddChild(GameObject* newChild)
	{
		m_children.push_back(newChild);
	}

	void GameObject::RemoveChild(GameObject* toRemove)
	{
		// checks whole vector. remove " m_children.end()" as an erase argument and it will only remove 1 instance.
		m_children.erase(std::remove(m_children.begin(), m_children.end(), toRemove), m_children.end());
	}
	
	bool GameObject::IsChild(GameObject* object)
	{
		if (object == nullptr) return false;
		for (GameObject* child : m_children)
		{
			if ( object == child ) return true;
			if ( child->IsChild(object) ) return true;
		}
		return false;
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetLocalPosition(x, y);
	}

	SmartTransform * GameObject::GetTransform()
	{
		return &m_transform;
	}

	Transform const* GameObject::GetLocalTransform() const
	{
		return m_transform.GetLocalTransform();
	}

	Transform const* GameObject::GetWorldTransform()
	{
		return m_transform.GetWorldTransform();
	}

	void GameObject::MakeDirty()
	{
		for (GameObject* pChild : m_children)
		{
			pChild->MakeDirty();
		}

		if (m_transform.IsDirty()) return;

		m_transform.MakeDirty();
	}

	void GameObject::MarkForDelete(bool excludeChildren)
	{
		m_toDelete = true;
		if (excludeChildren)
		{
			for (GameObject* child : m_children)
			{
				child->SetParent(m_pParent);
			}
			// set children parent to curent parent
		}
		for (GameObject* child : m_children)
		{
			child->MarkForDelete();
		}
	}

	void GameObject::RemoveComponent(size_t index)
	{
		if (index < m_components.size())
		{
			m_components.erase(m_components.begin() + index);
		}
	}
}
