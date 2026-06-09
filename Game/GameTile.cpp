#include "GameTile.h"

#include <components/RenderComponent.h>
#include <Texture2D.h>
#include <eventSystem/EventStack.h>
#include <eventSystem/EventHash.h>

#include <iostream>


namespace digger
{
	GameTile::GameTile(dae::GameObject& parent, int x_tiles, int y_tiles, std::string const& texture, std::string const& traversedTexture, bool isTraversed)
		:Tile(parent, x_tiles, y_tiles),
		m_textureName{ texture },
		m_traversedTextureName{ traversedTexture },
		isTraversed{ isTraversed }
	{
		m_textureComp = GetGameObject()->AddNGetComponent<dae::TextureComponent>();
		m_textureComp->SetTexture(isTraversed ? m_traversedTextureName : m_textureName);
		m_textureComp->renderScale = 1.5f;
	}

	float GameTile::GetTileSize() const
	{
		return m_textureComp->GetRenderSize().x;
	}

	void GameTile::SetTraversed()
	{
		if (!isTraversed)
		{
			std::cout << "tile dug--------\n";
			dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("ground dug"), this->GetGameObject() });
			isTraversed = true;
			m_textureComp->SetTexture(m_traversedTextureName);
		}

	}
}