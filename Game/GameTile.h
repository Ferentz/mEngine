#pragma once

#include <tileGrid/Tilegrid.h>
#include <Components.h>
#include <eventSystem/EventStack.h>
#include <eventSystem/EventHash.h>

namespace dae
{
	class GameObject;
}

class GameTile final : public dae::Tile
{
public:
	GameTile(dae::GameObject& parent, int x_tiles, int y_tiles, std::string const & texture, std::string const& traversedTexture, bool isTraversed = false)
		:Tile(parent, x_tiles, y_tiles),
		m_textureName{texture},
		m_traversedTextureName{ traversedTexture },
		isTraversed{ isTraversed }
	{
		m_textureComp = GetGameObject()->AddNGetComponent<dae::TextureComponent>();
		m_textureComp->SetTexture(isTraversed? m_traversedTextureName: m_textureName);
	}
	~GameTile() = default;

	float GetTileSize() const override
	{
		return m_textureComp->GetTexture()->GetSize().x;
	}

	bool CanMoveToTile() const override
	{
		return IsOccupied();
	}

	void SetTraversed()
	{
		if (!isTraversed)
		{
			dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("ground dug"), this->GetGameObject() });
			isTraversed = true;
			m_textureComp->SetTexture(m_traversedTextureName);
		}
		
	}

	bool IsTraversed()
	{
		return isTraversed;
	}

	dae::GameObject* GetOccupant() const
	{
		return occupant;
	}

	bool IsOccupied() const
	{
		if (occupant == nullptr) return true;
		else return false;
	}

	bool TakeOccupancy(dae::GameObject* newOccupant)
	{
		if (occupant == nullptr)
		{
			occupant = newOccupant;
			return true;
		}
		return false;
	}
	bool ReleaseOccupancy(dae::GameObject* OldOccupant)
	{
		if (occupant == OldOccupant)
		{
			occupant = nullptr;
			return true;
		}
		return false;
	}
private:
	dae::GameObject* occupant{ nullptr };
	dae::TextureComponent * m_textureComp;
	bool isTraversed;
	std::string const m_textureName;
	std::string const m_traversedTextureName;

};

// we wanna be able to pass a tile, and for it to set on its own.
