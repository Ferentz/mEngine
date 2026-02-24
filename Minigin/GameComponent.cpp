
#include "GameComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	
	GameComponent::GameComponent(GameObject& parent)
		:m_pParent{&parent}
	{
	}

	void GameComponent::SetParent(GameObject& parent)
	{
		m_pParent = &parent;
	}

	GameObject const* GameComponent::GetParent() const
	{
		return m_pParent;
	}

	derivedComponent::derivedComponent(GameObject& parent, float val)
		:GameComponent(parent),
		m_val{val}
	{
	}

	void TransformComponent::SetPosition(const float x, const float y, const float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

	void dae::TransformComponent::SetPosition(const glm::vec3& position)
	{
		m_position = position;
	}

	void RenderComponent::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.0f);
	}

	TransformComponent const* RenderComponent::GetTransform() const
	{
		return &m_transform;
	}

	void TextureComponent::Render() const
	{
		if (m_texture != nullptr)
		{
			const auto& pos = GetTransform()->GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
	}

	TextureComponent::TextureComponent(GameObject& parent)
		:RenderComponent(parent),
		m_texture(nullptr)
	{
	}

	void TextureComponent::SetTexture(const std::string& filename)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	}

	TextComponent::TextComponent(GameObject& parent,
		const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
		:TextureComponent(parent),
		m_needsUpdate(true),
		m_text(text),
		m_color(color),
		m_font(std::move(font))
	{
	}

	void TextComponent::Update(float)
	{
		if (m_needsUpdate)
		{
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_DestroySurface(surf);
			m_texture = std::make_shared<Texture2D>(texture);
			m_needsUpdate = false;
		}
	}
	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

	void TextComponent::SetColor(const SDL_Color& color)
	{
		m_color = color;
		m_needsUpdate = true;
	}
	FPSComponent::FPSComponent(GameObject& parent, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
		:TextComponent(parent, text, font, color)
	{
	}
	void FPSComponent::Update(float deltaTime)
	{
		m_accTime += deltaTime;
		++m_frameCount;
		if (m_accTime >= 1.f)
		{
			if (const float fps{ m_frameCount / m_accTime };
				m_prevDelta - fps >= 0.01f || m_prevDelta - fps <= -0.01f)
			{
				SetText(std::format("FPS: {:.2f}", fps));
				m_prevDelta = fps;
				TextComponent::Update(deltaTime);
			}
			m_accTime = 0;
			m_frameCount = 0;
		}
	}
}

