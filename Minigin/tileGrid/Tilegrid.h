#pragma once
//#include <components/GameComponent.h>
#include <GameObject.h>
#include <vector>
#include "Texture2D.h"
#include "SceneManager.h"
namespace dae
{
	class Tile : public dae::GameComponent
	{
	public:
		Tile(GameObject& parent, int x, int y )
			:GameComponent{ parent },
			point{x,y}
		{

		}
		
		glm::ivec2 const & GetPoint()
		{
			return point;
		}

		virtual float GetTileSize() const
		{
			return 1.f;
		}
		virtual bool CanMoveToTile() const
		{
			return true;
		}
		
	private:
		
		glm::ivec2 point;
	};

	class Tilegrid : public dae::GameComponent
	{
	public:
		// we want to get x and y tiles. size? maybe width and height?
		Tilegrid(GameObject& parent, int x_tiles, int y_tiles, float tileScale)
			:GameComponent{ parent },
			width{ x_tiles },
			height{ y_tiles },
			tileScale{ tileScale }
		{
			tiles.resize(width * height);
		}
		~Tilegrid() = default;

		

		//tiles only update when player moves, so maybe it can listen for player move? but this would be 
		void UpdateTile() {};

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
		
		GameObject* GetTile(glm::ivec2 point)
		{
			if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height) return nullptr;
			return tiles[point.x + width * point.y];
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

		bool IsAtPoint(glm::ivec2 point, glm::vec2 pos) const
		{
			auto pointPos = GetGridLocationOfPoint(point);
			if (glm::length(pos - pointPos) < 3.f)
			{
				return true;
			}
			return false;
		}

		virtual glm::vec2 GetGridLocationOfPoint(glm::ivec2 point) const
		{
			return glm::vec2(point.x * (tileScale), point.y * tileScale);
		}

		virtual glm::ivec2 GetPointOfGridLocation(glm::vec2 location) const
		{
			return glm::round(glm::vec2(location.x / (tileScale), location.y / tileScale));
		}

		bool IsPointValid(glm::ivec2 point)
		{
			if (point.x >= 0 && point.x < width
				&& point.y >= 0 && point.y < height)
				return true;
			return false;
		}

		float GetTileScale() const
		{
			return tileScale;
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