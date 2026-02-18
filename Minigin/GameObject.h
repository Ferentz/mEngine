#pragma once
#include <string>
#include <memory>
#include "Transform.h"

#include "GameComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject 
	{
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::unique_ptr<GameComponent>> m_components{};
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		void AddTextComponent(const std::string& text,
			 std::shared_ptr<Font> font,
			 const SDL_Color& color = { 255, 255, 255, 255 });
		void GetComponent(int index, GameComponent* out);
		void RemoveComponent(int index);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
}
