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


	void GameObject::SetParent(GameObject& parent, bool keepWorldPos)
	{
		// if the new parent is already the parent of the object
		if (&parent == this->m_pParent) return;

		// if the new parent is a child of the object
		if (this->IsChild(&parent)) return;

		//if the object is in the child list of the parent
		
		//rebase transforms
		if (keepWorldPos)
		{
			m_transform.Rebase(parent.QueryWorldTransform());
			//this->MakeDirty();
		}
		this->MakeDirty();


		// - should always return true atm. failsafe in function should be posible to remove
		if (!parent.AddChild(this)) return;

		//remove from old parent
		if (m_pParent) m_pParent->RemoveChild(this);
		
		// officialy set new parent;
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

	void GameObject::RemoveChild(GameObject* toRemove)
	{
		// checks whole vector. remove " m_children.end()" as an erase argument and it will only remove 1 instance.
		m_children.erase(std::remove(m_children.begin(), m_children.end(), toRemove), m_children.end());
	}

	bool GameObject::IsChild(GameObject* object)
	{
		for (GameObject* child : m_children)
		{
			if ( object == child ) return true;
			if ( child->IsChild(object) ) return true;
		}
		return false;
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y);
		for (RenderComponent * component : m_renderComponents)
		{
			component->MakeDirty();
		}
		//m_transform->SetPosition(x, y, 0.0f);
	}

	SmartTransform * GameObject::GetTransform()
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

	Transform * GameObject::QueryWorldTransform()
	{
		Transform* temp{ nullptr };
		if (m_pParent != nullptr)
		{
			temp = m_pParent->QueryWorldTransform();
		}
		m_Dirty = false;
		return m_transform.QueryWorldTransform(temp);
	}

	void GameObject::MakeDirty()
	{
		m_Dirty = true;
		m_transform.MakeDirty();
		for (RenderComponent* pComponent : m_renderComponents)
		{
			pComponent->MakeDirty();
		}
		for (GameObject* pChild : m_children)
		{
			pChild->MakeDirty();
		}
	}

	void GameObject::MarkForDelete(bool excludeChildren)
	{
		m_toDelete = true;
		if (excludeChildren)
		{
			for (GameObject* child : m_children)
			{
				child->SetParent(*m_pParent);
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
		if (index < m_components.size() && index >= 0)
		{
			m_components.erase(m_components.begin() + index);
		}
	}
}
