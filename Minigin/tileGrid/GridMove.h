#pragma once
#include "components/GameComponent.h"
#include "Tilegrid.h"
#include <eventSystem/EventHash.h>
#include <eventSystem/Listener.h>
#include <glm/glm.hpp>

#include "collision/Collider.h"

namespace dae
{
	class GridMove : public GameComponent
	{
	public:
		GridMove(GameObject& parent, Tilegrid& in_grid, Collider& collider, glm::ivec2 startPoint, float speed)
			:GameComponent(parent),
			closestPoint{ startPoint },
			grid{ &in_grid },
			speed{ speed },
			collider{ &collider }
		{
			collider.size.x = in_grid.GetTileScale() * 0.75f;
			collider.size.y = in_grid.GetTileScale() * 0.75f;
			GetGameObject()->SetParent(in_grid.GetGameObject(), false);
			auto pos = grid->GetGridLocationOfPoint(closestPoint);
			GetGameObject()->m_transform.SetLocalPosition(pos.x, pos.y);
		}

		void Update(float) override
		{
			if (goToPoint)
			{
				auto direction = DirectionToClosest();
				glm::vec2 dir = glm::vec2(direction) * speed;
				GetGameObject()->m_transform.Translate(dir.x, dir.y);
				if (CheckPosition())
				{
					goToPoint = false;
				}
			}
		}


		Tilegrid* GetGrid() { return grid; }

		glm::ivec2 const& GetClosestPoint() const { return closestPoint; }


		virtual bool Move(bool horizontal, int direction, bool influenced = false)
		{
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

			auto newPoint = dot > 0? closestPoint : closestPoint + moveVec;

			if (!grid->IsPointValid(newPoint)) return false;

			if (collider != nullptr && !CheckCollisions(moveVec, horizontal, direction, influenced)) return false;

			glm::vec2 dir = glm::vec2(moveVec) * speed;
			GetGameObject()->m_transform.Translate(dir.x, dir.y);

			CheckPosition();
			return true;
		}

		bool CheckPosition()
		{
			auto pos = GetGameObject()->m_transform.GetLocalTransform()->GetPosition();
			auto point = grid->GetPointOfGridLocation(pos);

			if (point != closestPoint) closestPoint = point;

			if (grid->IsAtPoint(closestPoint, pos))
			{
				//auto closestPos = grid->GetTile(closestPoint)->m_transform.GetLocalTransform()->GetPosition();
				//GetGameObject()->m_transform.SetLocalPosition(closestPos.x, closestPos.y);
				m_vertical = true;
				m_horizontal = true;
				return true;
			}
			return false;
		}

		glm::ivec2 DirectionToClosest()
		{
			auto pos = GetGameObject()->m_transform.GetLocalTransform()->GetPosition();
			auto closestPos = grid->GetTile(closestPoint)->m_transform.GetLocalTransform()->GetPosition();

			auto direction = closestPos - pos;
			if (glm::length(direction) == 0) return direction;
			direction = glm::normalize(direction);

			direction = glm::round(direction);

			return direction;

		}

		bool CheckCollisions(glm::vec2 moveVec, bool horizontal, int direction, bool influenced = false)
		{
			if (influenced)
			{
				if (horizontal && !h_collisionInfluence) return false; // TODO :: make part of game instead of engine
				if (!horizontal && !v_collisionInfluence) return false; // TODO :: make part of game instead of engine
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
						if (!otherMove->Move(horizontal, direction, true))
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

		void SetInfluence(bool h, bool v)
		{
			h_collisionInfluence = h;
			v_collisionInfluence = v;
		}

	protected:
		bool m_vertical{ true};
		bool m_horizontal{ true };

		bool h_collisionInfluence{ true };
		bool v_collisionInfluence{ true };

		bool goToPoint{ false };
		glm::ivec2 closestPoint{};
		glm::ivec2 goalPoint{};
		Tilegrid* grid{};
		float speed{};

		Collider* collider;

	};
}