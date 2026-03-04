#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "GameComponent.h"
namespace dae
{
	//class GameComponent;
	class TransformComponent;
	class Font;
	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update(float deltaTime);
		virtual void Render();

		void SetParent(GameObject& newParent, bool keepWorldPos = false);
		
		bool IsChild(GameObject* object);
		void SetPosition(float x, float y);
		auto GetTransform() -> SmartTransform*;


		Transform const* GetLocalTransform() const;
		Transform * QueryWorldTransform();
		Transform const* GetWorldTransform() const;

		void MakeDirty();
		void MarkForDelete(bool excludeChildren = false);
		bool WillBeDeleted() const { return m_toDelete; }

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

	protected:
		bool m_Dirty{ true };
		SmartTransform m_transform{};

	private:
		bool m_toDelete{};
		GameObject* m_pParent{};
		std::vector<std::unique_ptr<GameComponent>> m_components{};
		std::vector<RenderComponent*> m_renderComponents{};
		std::vector<GameObject*> m_children{};

	private:
		bool AddChild(GameObject* newChild);
		void RemoveChild(GameObject* toRemove);
	};
}