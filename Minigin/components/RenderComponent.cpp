
#include "RenderComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"


namespace dae
{
	void RenderComponent::Update(float)
	{
	}

	void RenderComponent::MakeDirty()
	{
		m_transform.MakeDirty();

	}

	void RenderComponent::SetPosition(float x, float y)
	{
		MakeDirty();
		m_transform.SetLocalPosition(x, y, 0.0f);
	}

	Transform const* RenderComponent::GetLocalTransform() const
	{
		return m_transform.GetLocalTransform();
	}

	Transform const* RenderComponent::GetWorldTransform() const
	{
		return m_transform.GetWorldTransform();
	}

	Transform const* RenderComponent::QueryWorldTransform()
	{
		return m_transform.QueryWorldTransform(GetParent()->QueryWorldTransform());
	}



	TextureComponent::TextureComponent(GameObject& parent)
		:RenderComponent{ parent },
		m_texture(nullptr)
	{
	}
	
	void TextureComponent::Render()
	{
		if (m_texture != nullptr)
		{
			const auto& pos = QueryWorldTransform()->GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
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
		RenderComponent::Update({});
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
		:TextComponent{ parent, text, font, color }
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