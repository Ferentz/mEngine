#pragma once
#include "GameComponent.h"
#include "Transform.h"
namespace dae
{
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
		Transform const* QueryWorldTransform();
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