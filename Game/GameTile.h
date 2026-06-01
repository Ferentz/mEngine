#pragma once

#include <tileGrid/Tilegrid.h>


namespace dae
{
	class GameObject;
	class TextureComponent;
}

namespace digger
{
	class GameTile final : public dae::Tile
	{
	public:
		GameTile(dae::GameObject& parent, int x_tiles, int y_tiles, std::string const& texture, std::string const& traversedTexture, bool isTraversed = false);
		~GameTile() = default;

		float GetTileSize() const override;

		void SetTraversed();

		bool IsTraversed()
		{
			return isTraversed;
		}

		virtual bool CanMoveToTile() const override
		{
			return isTraversed;
		}

	private:
		dae::TextureComponent* m_textureComp;
		bool isTraversed;
		std::string const m_textureName;
		std::string const m_traversedTextureName;
	};


}
