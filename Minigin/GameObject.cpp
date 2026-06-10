#include "GameObject.h"


#include <iostream>
#include <memory>
#include <vector>
#include <variant>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"


#include <iostream>

//#include "components/GameComponent.h"
//#include "components/RenderComponent.h"


namespace dae
{
	GameObject::GameObject()
		:m_transform{this}
	{

	}

	GameObject::~GameObject() = default;

	void GameObject::Start()
	{
		for (std::unique_ptr<GameComponent>& pComponent : m_components)
		{
			pComponent->Start();
		}

		for (auto& child : m_children)
		{
			child->Start();
		}
	}

	void GameObject::Update(float deltaTime)
	{
		//std::cout << objectName << " update\n";
		for (std::unique_ptr<GameComponent>& pComponent : m_components)
		{
			pComponent->Update(deltaTime);
		}

		//int i{};
		for (auto& child : m_children)
		//for (size_t idx{} ; idx < m_children.size() ; idx ++) // this is slower than the ranegd, as it needs to do more checks. but as it stands i'm not making a system where new children get added in the next update if they tip the capacity over.
		{
			/*if (i == 206)
			{
				std::cout << i << " child idx is " << m_children[idx]->objectName << "\n children size is " << m_children.size() << ", capacity is " << m_children.capacity() << "\n";
			}*/
			
			child->Update(deltaTime);
			//i++;
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

		for (auto& child : m_children)
		{
			child->Render();
		}
	}

	Scene* GameObject::GetScene()
	{
		if (auto scene = GetParentAsScene())
		{
			return scene;
		}
		else if (auto object = GetParentAsObject())
		{
			return object->GetScene();
		}
		return nullptr;
	}

	GameObject* GameObject::GetParentAsObject()
	{
		if (std::holds_alternative<GameObject*>(m_pParent))
		{
			return std::get<GameObject*>(m_pParent);
		}
		return nullptr;
	}

	Scene* GameObject::GetParentAsScene()
	{
		if (std::holds_alternative<Scene*>(m_pParent))
		{
			return std::get<Scene*>(m_pParent);
		}
		return nullptr;
	}

	void GameObject::SetParent(GameObject *newParent, bool keepWorldPos)
	{
		if (!GetScene()) std::cerr << "object has not been added to a scene yet.\n";

		// if the new parent is already the parent of the object
		auto parentAsObject = GetParentAsObject();
		if ( newParent == parentAsObject) return;

		// if the new parent is a child of the object, works for nullptr
		if (this->IsChild(newParent)) return;
		
		//rebase transforms
		if (keepWorldPos)
		{
			m_transform.ReconstructLocal(newParent);
		}
		this->MakeDirty();


		//remove from old parent
		if (parentAsObject) parentAsObject->RemoveChild(this);
		else if(auto sceneParent = GetParentAsScene()) sceneParent->Release(this);

		// set new parent;
		if (newParent) this->m_pParent = newParent;
		else this->m_pParent = dae::SceneManager::GetInstance().GetActiveScene();

		if (auto parent = GetParentAsObject()) parent->AddChild(this);
		else dae::SceneManager::GetInstance().GetActiveScene()->Add(std::unique_ptr<GameObject>(this));
	}

	void GameObject::SetParent(Scene& newParent, bool keepWorldPos)
	{
		// if the new parent is already the parent of the object
		auto parentAsScene = GetParentAsScene();
		if (&newParent == parentAsScene) return;

		//rebase transforms
		if (keepWorldPos)
		{
			m_transform.ReconstructLocal(nullptr);
		}
		this->MakeDirty();


		//remove from old parent
		if (auto parent = GetParentAsObject()) parent->RemoveChild(this);
		else if (parentAsScene) parentAsScene->Release(this);

		// set new parent;
		this->m_pParent = &newParent;

		if (auto parentObject = GetParentAsObject()) parentObject->AddChild(this);
		else if (auto sceneParent = GetParentAsScene())
		{
			if(!sceneParent->HoldsObject(this))
				sceneParent->Add(std::unique_ptr<GameObject>(this));
		}
	}

	void GameObject::Add(std::unique_ptr<GameObject> && object, bool keepWorldPos)
	{
		auto child = object.release();
		if (child) child->SetParent(this, keepWorldPos);
	}

	void GameObject::AddChild(GameObject* newChild)
	{
		m_children.emplace_back(newChild);
	}

	void GameObject::RemoveChild(GameObject* toRemove)
	{
		auto it = std::find_if(m_children.begin(),m_children.end(),
			[toRemove](const std::unique_ptr<GameObject>& ptr)
			{
				return ptr.get() == toRemove;
			}
		);

		if (it == m_children.end()) return;

		it->release();
		m_children.erase(it);
	}
	
	bool GameObject::IsChild(GameObject* object)
	{
		if (object == nullptr) return false;
		for (auto& child : m_children)
		{
			if ( object == child.get() ) return true;
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
		for (auto & pChild : m_children)
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
			for (auto & child : m_children)
			{
				child->SetParent(GetParentAsObject());
			}
			// set children parent to curent parent
		}
		for (auto & child : m_children)
		{
			child->MarkForDelete();
		}
	}

	bool GameObject::Cleanup()
	{
		std::erase_if(m_children, [](std::unique_ptr<GameObject> const& obj) {
			return obj->Cleanup();
			});
		return m_toDelete;
	}

	void GameObject::RemoveComponent(size_t index)
	{
		if (index < m_components.size())
		{
			m_components.erase(m_components.begin() + index);
		}
	}
}
