#pragma once

#include <memory>
#include "Transform.h"
#include "Components.h"
#include <variant>
#include <vector>
#include <utility>
#include <type_traits>
#include <concepts>

#include <string>

namespace dae
{
	//class GameComponent;
	class TransformComponent;
	class Font;
	class Texture2D;
	class Scene;

	class GameObject final
	{
	public:
		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Start();
		void Update(float deltaTime);
		void Render();

		Scene* GetScene();

		GameObject* GetParentAsObject();
		Scene* GetParentAsScene();
		std::vector<std::unique_ptr<GameObject>> const& GetChildren() { return m_children; }

		void SetParent(GameObject *newParent, bool keepWorldPos = false);
		void SetParent(Scene& newParent, bool keepWorldPos = false);
		void Add(std::unique_ptr<GameObject> && object, bool keepWorldPos = false);
		void ResreveCHildren(size_t nbr)
		{
			minChidren += nbr;
			m_children.reserve(minChidren);
		}


		bool IsChild(GameObject* object);
		void SetPosition(float x, float y);
		auto GetTransform() -> SmartTransform*;


		Transform const* GetLocalTransform() const;
		Transform const * GetWorldTransform();

		void MakeDirty();
		void MarkForDelete(bool excludeChildren = false);
		bool WillBeDeleted() const { return m_toDelete; }
		bool Cleanup();

		// COMPONENT FUNCTIONS
		void RemoveComponent(size_t index);

		template<class T, typename ...Args>
			requires std::is_base_of_v<dae::GameComponent, T>
		inline void AddComponent(Args&& ...args)
		{

			m_components.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));

			if constexpr (std::derived_from<T, RenderComponent>)
			{
				m_renderComponents.emplace_back(static_cast<RenderComponent*>(m_components.back().get()));
			}
			
		}

		template<class T>
			requires std::is_base_of_v<GameComponent, T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> result;
			if (m_toDelete) return result;

			for (auto& component : m_components)
			{
				if (auto ptr = dynamic_cast<T*>(component.get()))
				{
					result.push_back(ptr);
				}
			}

			return result;
		}

		template<class T>
			requires std::is_base_of_v<GameComponent, T>
		T* GetComponent()
		{
			if(m_toDelete)return nullptr;

			for (auto& component : m_components)
			{
				if (auto ptr = dynamic_cast<T*>(component.get())) // get retrives the pointer fro the unique pointer
				{
					return ptr;
				}
			}
			return nullptr;
		}

		template<class T>
			requires std::is_base_of_v<GameComponent, T>
		T* GetComponent_ChildrenInclusive()
		{
			if (auto ptr = GetComponent<T>()) return ptr;

			for (auto& child : m_children)
			{
				if (T* component = child->GetComponent_ChildrenInclusive<T>()) return component;
			}
			return nullptr;
		}

		template<class T>
			requires std::is_base_of_v<GameComponent, T>
		std::vector<T*> GetComponents_ChildrenInclusive()
		{
			std::vector<T*> result;

			if (m_toDelete)
				return result;

			// Add this object's matching components
			for (auto& component : m_components)
			{
				if (auto ptr = dynamic_cast<T*>(component.get()))
				{
					result.push_back(ptr);
				}
			}

			// Recursively collect from children
			for (auto& child : m_children)
			{
				auto childResults =
					child->GetComponents_ChildrenInclusive<T>();

				result.insert( result.end(), childResults.begin(), childResults.end());
			}

			return result;
		}

		template<class T>
			requires std::is_base_of_v<GameComponent, T>
		int CountComponents_childrenInclusive()
		{
			int result{};

			if (!m_toDelete)
			{
				for (auto& component : m_components)
				{
					if (auto ptr = dynamic_cast<T*>(component.get()))
					{
						result++;
					}
				}
			}

			for (auto& child : m_children)
			{
				if (int components = child->CountComponents_childrenInclusive<T>())
				{
					result += components;
				}
			}

			return result;
		}

		template<class T>
			requires std::is_base_of_v<GameComponent, T>
		T* GetLatestComponent()
		{
				return dynamic_cast<T*>(m_components.back().get());
		}

		template<class T, typename ...Args>
			requires std::is_base_of_v<dae::GameComponent, T>
		T* AddNGetComponent(Args&& ...args)
		{
			AddComponent<T>(std::forward<Args>(args)...);
			return GetLatestComponent<T>();
		}

		SmartTransform m_transform;

		std::string objectName{"unnamed"};

	private:
		
		bool m_toDelete{};
		std::variant<GameObject*, Scene*> m_pParent{};
		std::vector<std::unique_ptr<GameComponent>> m_components{};
		std::vector<RenderComponent*> m_renderComponents{};
		std::vector<std::unique_ptr<GameObject>> m_children{};
		size_t minChidren{};

	private:
		void AddChild(GameObject* newChild);
		void RemoveChild(GameObject* toRemove);
		
	};
}