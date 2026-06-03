#include <tileGrid/GridMove.h>
#include "Minigin.h"

namespace dae
{
	GridMove::GridMove(GameObject& parent, Tilegrid& in_grid, Collider& collider, glm::ivec2 startPoint, float speed)
		:GameComponent(parent),
		closestPoint{ startPoint },
		grid{ &in_grid },
		speed{ speed },
		collider{ &collider }
	{
		collider.size.x = in_grid.GetTileScale() * 0.75f;
		collider.size.y = in_grid.GetTileScale() * 0.75f;

		auto pos = grid->GetGridLocationOfPoint(closestPoint);
		GetGameObject()->m_transform.SetLocalPosition(pos.x, pos.y);
	}

	bool GridMove::Move(bool horizontal, int direction, bool influenced, float useSpeed)
	{
		if (!canMove) return false;

		if (useSpeed == -1) useSpeed = speed;

		glm::ivec2 moveVec;


		if (horizontal && m_horizontal)
		{
			moveVec = glm::ivec2(direction, 0);
			m_vertical = false;
		}
		else if (!horizontal && m_vertical)
		{
			moveVec = glm::ivec2(0, direction);
			m_horizontal = false;
		}
		else
		{
			// -> move to closest point
			moveVec = DirectionToClosest();


		}
		auto closestPos = grid->GetGridLocationOfPoint(closestPoint);
		glm::vec2 vecToOher{ closestPos - GetGameObject()->GetLocalTransform()->GetPosition() };

		float dot = glm::dot(vecToOher, glm::vec2(moveVec));

		auto newPoint = dot > 0 ? closestPoint : closestPoint + moveVec;

		if (!grid->IsPointValid(newPoint)) return false;

		float deltaSpeed = useSpeed * dae::Minigin::GetDeltaTime();
		if (collider != nullptr && !CheckCollisions(moveVec, horizontal, direction,useSpeed, influenced))
		{
			if (!influenced)
			{
				glm::vec2 dir = glm::vec2(moveVec) * -deltaSpeed;
				GetGameObject()->m_transform.Translate(dir.x, dir.y);
			}
			
			return false;
		}



		glm::vec2 dir = glm::vec2(moveVec) * deltaSpeed;
		GetGameObject()->m_transform.Translate(dir.x, dir.y);

		CheckPosition();
		return true;
	}

	glm::ivec2 GridMove::DirectionToClosest()
	{
		auto pos = GetGameObject()->m_transform.GetLocalTransform()->GetPosition();
		auto closestPos = grid->GetTile(closestPoint)->m_transform.GetLocalTransform()->GetPosition();

		auto direction = closestPos - pos;
		if (glm::length(direction) == 0) return direction;
		direction = glm::normalize(direction);

		direction = glm::round(direction);

		return direction;

	}


	bool GridMove::CheckPosition()
	{
		auto pos = GetGameObject()->m_transform.GetLocalTransform()->GetPosition();
		auto point = grid->GetPointOfGridLocation(pos);

		if (point != closestPoint) closestPoint = point;

		if (grid->IsAtPoint(closestPoint, pos))
		{
			m_vertical = true;
			m_horizontal = true;
			return true;
		}
		return false;
	}

	bool GridMove::CheckCollisions(glm::vec2 moveVec, bool horizontal, int direction, float useSpeed, bool influenced)
	{
		if (influenced)
		{
			if (horizontal && !h_collisionInfluence) return false;
			if (!horizontal && !v_collisionInfluence) return false;
		}
		for (auto other : collider->GetCollisions())
		{
			auto vecToOher{ other->GetGameObject()->GetLocalTransform()->GetPosition() - GetGameObject()->GetLocalTransform()->GetPosition() };
			if (horizontal)vecToOher.y = 0;
			else vecToOher.x = 0;
			float dot = glm::dot(vecToOher, moveVec);
			if (dot > 0) // same direction
			{
				if (auto otherMove = other->GetGameObject()->GetComponent<GridMove>())
				{
					if (!otherMove->Move(horizontal, direction, true, useSpeed))
					{
						return false; // if the other thing can't move, neither can we
					}
					continue; // this one can move, check next
				}
				return false; // if the other thing can't move, neither can we
			}
		}
		return true;//nothing is obstructing our direction, we good
	}
}