#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"



dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime){}

void dae::GameObject::Render() const
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddTextComponent(const std::string& text,
	std::shared_ptr<Font> font,
	const SDL_Color& color)
{
	//m_components.push_back(std::make_unique<GameComponent>(this));
	//TextComponent(GameObject* parent,
	// const std::string& text,
	// std::shared_ptr<Font> font,
	// const SDL_Color& color = { 255, 255, 255, 255 });
	m_components.push_back(std::make_unique<TextComponent>(this,text, font, color));
}

void dae::GameObject::GetComponent(int index, GameComponent* out)
{
	out = m_components[index].get();
}

void dae::GameObject::RemoveComponent(int index)
{
	if (index < m_components.size() && index >= 0)
	{
		m_components.erase(m_components.begin() + index);
	}
}
