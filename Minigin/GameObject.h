#pragma once
#include <string>
#include <memory>
#include "Transform.h"

namespace dae
{
	class GameComponent;
	class TransformComponent;
	class Font;
	class Texture2D;
	class GameObject 
	{
		std::unique_ptr<TransformComponent> m_transform;
		
		std::vector<std::unique_ptr<GameComponent>> m_components{};

	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		
		void SetPosition(float x, float y);
		TransformComponent const * GetTransform() const;

		template<class T, typename ...Args>
			requires std::is_base_of_v<dae::GameComponent, T>
		inline void AddComponent(Args&& ...args)
		{
			m_components.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
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

		void RemoveComponent(size_t index);

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
	
}
