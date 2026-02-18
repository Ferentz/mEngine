#include "GameComponent.h"
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	

	GameComponent::GameComponent(GameObject* parent)
		:m_pParent{parent}
	{
	}

	derivedComponent::derivedComponent(GameObject* parent, float val)
		:GameComponent(parent),
		m_val{val}
	{
	}

	TextComponent::TextComponent(GameObject* parent, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
		: GameComponent(parent),
		m_needsUpdate(true),
		m_text(text),
		m_color(color), m_font(std::move(font)), m_textTexture(nullptr)
	{
	}

	void TextComponent::Update(float deltaTime)
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
			m_textTexture = std::make_shared<Texture2D>(texture);
			m_needsUpdate = false;
		}
	}

	void TextComponent::Render() const
	{
		if (m_textTexture != nullptr)
		{
			const auto& pos = m_transform.GetPosition();
			Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
		}
	}

	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

	void TextComponent::SetPosition(const float x, const float y)
	{
		m_transform.SetPosition(x, y);
	}

	void TextComponent::SetColor(const SDL_Color& color)
	{
		m_color = color;
		m_needsUpdate = true;
	}

	template<class T, typename... Args>
		requires std::is_base_of_v<GameComponent, T>
	std::unique_ptr<GameComponent> MakeComponent(GameObject* parent, Args&&... args)
	{
		return std::make_unique<T>(parent, std::forward<Args>(args)...);
	}


}

