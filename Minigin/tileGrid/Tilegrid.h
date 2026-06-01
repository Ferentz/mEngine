#pragma once

#include <GameObject.h>
#include <vector>
#include <SceneManager.h>

#include <tileGrid/Tile.h>

namespace dae
{
	class Tilegrid : public dae::GameComponent
	{
	public:
		// we want to get x and y tiles. size? maybe width and height?
		Tilegrid(GameObject& parent, int x_tiles, int y_tiles, float tileScale);

		~Tilegrid() = default;

		GameObject* GetTile(glm::ivec2 point);

		bool IsAtPoint(glm::ivec2 point, glm::vec2 pos) const;

		virtual glm::vec2 GetGridLocationOfPoint(glm::ivec2 point) const;

		virtual glm::ivec2 GetPointOfGridLocation(glm::vec2 location) const;

		bool IsPointValid(glm::ivec2 point);

		float GetTileScale() const
		{
			return tileScale;
		}

		glm::ivec2 GetMax() { return glm::ivec2(width, height); }

		template<class T, typename ...Args>
			requires std::is_base_of_v<dae::Tile, T>
		void MakeGrid(bool TakeTileSize, Args&& ...args)
		{
			tiles.clear();
			tiles.resize(width * height);

			InsertTile<T>(0, 0, std::forward<Args>(args)...);
			if (TakeTileSize)
				tileScale = tiles[0]->GetComponent<T>()->GetTileSize();

			for (int x{}; x < width; x++)
			{
				for (int y{}; y < height; y++)
				{
					if (x == 0 && y == 0) continue;
					InsertTile<T>(x, y, std::forward<Args>(args)...);
				}
			}
			
		}
		
		template<class T, typename ...Args>
			requires std::is_base_of_v<dae::Tile, T>
		dae::GameObject* MakeTile(int x, int y, Args&& ...args)
		{
			if(x + width * y < tiles.size() && tiles[x + width * y] != nullptr)
			tiles[x + width * y]->MarkForDelete();

			InsertTile<T>(x, y, std::forward<Args>(args)...);

			return tiles[x + width * y];
		}

	protected:
		int width;
		int height;
		float tileScale;

		std::vector<GameObject*> tiles;

		template<class T, typename ...Args>
			requires std::is_base_of_v<dae::Tile, T>
		GameObject* InsertTile(int x, int y, Args&& ...args)
		{
			auto tile = std::make_unique<dae::GameObject>();

			tile->AddNGetComponent<T>(x, y, std::forward<Args>(args)...);
			tile->SetParent(GetGameObject());
			auto pos = GetGridLocationOfPoint(glm::ivec2(x, y));
			tile->SetPosition(pos.x, pos.y);

			tiles[x + width * y] = tile.get();

			if(auto scene = dae::SceneManager::GetInstance().GetActiveScene())
				scene->Add(std::move(tile));
			return tiles[x + width * y];
		}
	};

}