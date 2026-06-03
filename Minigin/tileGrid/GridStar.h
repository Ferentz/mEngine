#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <glm/glm.hpp>


#include <tileGrid/Tilegrid.h>

namespace dae
{
	struct Node
	{
		glm::ivec2 point{};
		float gCost{};
		float hCost{};
		glm::ivec2 parent{ -1, -1 };

		float FCost() const
		{
			return gCost + hCost;
		}
	};

	struct CompareNode
	{
		bool operator()(const Node& a, const Node& b) const
		{
			return a.FCost() > b.FCost();
		}
	};

	class GridStar
	{
	public:

		/*template<class T>
			requires std::is_base_of_v<Tile, T>*/
		static std::vector<glm::ivec2> FindPath( Tilegrid& grid, const glm::ivec2& start, const glm::ivec2& goal, bool care = true)
		{
			if (!grid.IsPointValid(start) || !grid.IsPointValid(goal))
			{
				return {};
			}

			if (start == goal)
			{
				return { start };
			}

			//like the sortedset in uniy
			std::priority_queue<Node, std::vector<Node>, CompareNode> openSet;

			std::unordered_map<int, Node> allNodes;
			std::unordered_set<int> closedSet;

			auto PointToIndex = [&](const glm::ivec2& p)
			{
				auto max = grid.GetMax();
				return p.x + p.y * max.x;
			};

			auto Heuristic = [](const glm::ivec2& a, const glm::ivec2& b)
			{
				return static_cast<float>(
					std::abs(a.x - b.x) +
					std::abs(a.y - b.y));
			};

			const glm::ivec2 directions[4]
			{
				{ 1, 0 },
				{-1, 0 },
				{ 0, 1 },
				{ 0,-1 }
			};

			Node startNode;
			startNode.point = start;
			startNode.gCost = 0.f;
			startNode.hCost = Heuristic(start, goal);

			openSet.push(startNode);
			allNodes[PointToIndex(start)] = startNode;

			while (!openSet.empty())
			{
				Node current = openSet.top();
				openSet.pop();

				int currentIndex =
					PointToIndex(current.point);

				if (closedSet.contains(currentIndex))
				{
					continue;
				}

				closedSet.insert(currentIndex);

				// Goal reached
				if (current.point == goal)
				{
					std::vector<glm::ivec2> path;

					glm::ivec2 walkPoint = goal;

					while (walkPoint != glm::ivec2(-1, -1))
					{
						path.push_back(walkPoint);

						int idx = PointToIndex(walkPoint);

						walkPoint = allNodes[idx].parent;
					}

					std::reverse(path.begin(), path.end());

					return path;
				}

				// Explore neighbors
				for (const auto& dir : directions)
				{
					glm::ivec2 neighbor = current.point + dir;

					if (!grid.IsPointValid(neighbor))
					{
						continue;
					}

					int neighborIndex = PointToIndex(neighbor);

					if (closedSet.contains(neighborIndex))
					{
						continue;
					}

					auto* tileObj = grid.GetTile(neighbor);

					if (!tileObj)
					{
						continue;
					}

					if (care)
					{
						auto* tile = tileObj->GetComponent<Tile>();

						if (!tile || !tile->CanMoveToTile())
						{
							continue;
						}
					}
					

					float tentativeG = current.gCost + 1.f;

					auto found = allNodes.find( neighborIndex);

					bool betterPath = found == allNodes.end() || tentativeG < found->second.gCost;

					if (betterPath)
					{
						Node neighborNode;
						neighborNode.point = neighbor;

						neighborNode.gCost = tentativeG;

						neighborNode.hCost = Heuristic( neighbor, goal);

						neighborNode.parent = current.point;

						allNodes[neighborIndex] = neighborNode;

						openSet.push( neighborNode);
					}
				}
			}

			// No path found
			return {};
		}
	};
}