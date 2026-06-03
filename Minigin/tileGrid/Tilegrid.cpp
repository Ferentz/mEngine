#include "Tilegrid.h"

namespace dae
{

	Tilegrid::Tilegrid(GameObject& parent, int x_tiles, int y_tiles, float tileScale)
		:GameComponent{ parent },
		width{ x_tiles },
		height{ y_tiles },
		tileScale{ tileScale }
	{
		tiles.resize(width * height);
	}

	GameObject* Tilegrid::GetTile(glm::ivec2 point)
	{
		if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height) return nullptr;


		return tiles[point.x + width * point.y];
	}

	bool Tilegrid::IsAtPoint(glm::ivec2 point, glm::vec2 pos) const
	{
		auto pointPos = GetGridLocationOfPoint(point);
		if (glm::length(pos - pointPos) < 3.f)
		{
			return true;
		}
		return false;
	}

	glm::vec2 Tilegrid::GetGridLocationOfPoint(glm::ivec2 point) const
	{
		return glm::vec2(point.x * (tileScale), point.y * tileScale);
	}

	glm::ivec2 Tilegrid::GetPointOfGridLocation(glm::vec2 location) const
	{
		glm::ivec2 point{ glm::round(glm::vec2(location.x / (tileScale), location.y / tileScale)) };

		if (point.x < 0) point.x = 0;
		if (point.x >= width) point.x = width-1;
		if (point.y < 0) point.y = 0;
		if (point.y >= height) point.y = height - 1;
		return point;
	}

	bool Tilegrid::IsPointValid(glm::ivec2 point)
	{
		if (point.x >= 0 && point.x < width
			&& point.y >= 0 && point.y < height)
			return true;
		return false;
	}
}