#pragma once
#include <string>
#include <memory>

//#include "GameObject.h"
#include "Transform.h"

#include <SDL3/SDL_pixels.h>
#include <glm/glm.hpp>

namespace dae
{
	

	class GameObject;
	class Texture2D;
	class Font;
	
	class GameComponent
	{
	public:
		virtual ~GameComponent() = default;
		explicit GameComponent(GameObject& parent);

		void SetParent(GameObject& parent);
		virtual void Update(float) {};


	protected:
		explicit GameComponent(GameObject* parent)
			:m_pParent{parent}
		{}
		GameObject* GetParent() const;
	private:
		GameObject* m_pParent;
		
		
		GameComponent(const GameComponent& other) = delete;
		GameComponent(GameComponent&& other) = delete;
		GameComponent& operator=(const GameComponent& other) = delete;
		GameComponent& operator=(GameComponent&& other) = delete;
	};


	class derivedComponent final : public GameComponent
	{
	public:
		virtual ~derivedComponent() override = default;
		explicit derivedComponent(GameObject& parent, float val);


	protected: // i tried something, it didnt work. i need fps
		float m_val{};
		
		derivedComponent(const derivedComponent& other) = delete;
		derivedComponent(derivedComponent&& other) = delete;
		derivedComponent& operator=(const derivedComponent& other) = delete;
		derivedComponent& operator=(derivedComponent&& other) = delete;
	};

	class RotatorComponent final : public GameComponent
	{
	public:
		virtual ~RotatorComponent() override = default;
		explicit RotatorComponent(GameObject& parent);

		virtual void Update(float) override;

		void SetRotationSpeed(float speed);

	protected: // i tried something, it didnt work. i need fps
		float m_rotatingSpeed{ 1.f };

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;
	};

	class RenderComponent : public GameComponent
	{
	public:
		virtual ~RenderComponent() = default;
		explicit RenderComponent(GameObject& parent)
			:GameComponent{ parent },
			m_transform{}
		{
		}

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
		
		virtual void Render() = 0;
		virtual void Update(float) override;
		void MakeDirty();

		void SetPosition(float x, float y);
		Transform const* GetLocalTransform() const;
		Transform const * QueryWorldTransform();
		Transform const* GetWorldTransform() const;

	private:
		SmartTransform m_transform;
	};

	class TextureComponent : public RenderComponent
	{
	public:
		virtual ~TextureComponent() override = default;
		explicit TextureComponent(GameObject& parent);

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		virtual void Render() override;
		//virtual void Render()const override {}
		void SetTexture(const std::string& filename);

	protected:
		std::shared_ptr<Texture2D> m_texture{};
	private:
		
	};

	class TextComponent : public TextureComponent
	{
	public:
		virtual ~TextComponent() override = default;

		explicit TextComponent(GameObject& parent,
			const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });

		virtual void Update(float) override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	protected:

	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};
	};

	class FPSComponent final : public TextComponent
	{
	public:
		virtual ~FPSComponent() override = default;

		explicit FPSComponent(GameObject& parent,
			const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });

		virtual void Update(float) override;

	protected:

	private:
		float m_accTime{};
		int m_frameCount{};
		float m_prevDelta{};
	};

}