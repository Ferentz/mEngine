#pragma once
#include "components/GameComponent.h"
#include "Tilegrid.h"
#include <eventSystem/EventHash.h>
#include <eventSystem/broadcaster.h>
#include <glm/glm.hpp>

namespace dae
{
	class GridMove : public GameComponent
	{
	public:
		GridMove(GameObject& parent, Tilegrid& in_grid, glm::ivec2 startPoint, float speed)
			:GameComponent(parent),
			point{ startPoint },
			grid{&in_grid },
			speed{ speed }
		{
			GetGameObject()->SetParent(in_grid.GetGameObject(), false);
			auto pos = grid->GetGridLocationOfPoint(point);
			GetGameObject()->m_transform.SetLocalPosition(pos.x, pos.y);
		}
		void Update(float) override
		{
			if (!AtPoint)
			{
				GetGameObject()->m_transform.Translate(direction.x * speed, direction.y * speed);
				auto pos = GetGameObject()->m_transform.GetLocalTransform()->GetPosition();
				if (grid->IsAtPoint(point, glm::vec2(pos.x, pos.y)))
				{
					auto aimPos = grid->GetGridLocationOfPoint(point);
					GetGameObject()->m_transform.SetLocalPosition(aimPos.x, aimPos.y);
					AtPoint = true;
					m_signal.BroadCast(make_sdbm_hash("arrived"), GetGameObject());
				}
			}
		}

		bool IsAtPoint() const { return AtPoint; }

		Tilegrid* GetGrid() { return grid; }

		glm::ivec2 const & GetPoint() const { return point; }

		void MoveTo(glm::ivec2 movevec, bool force = false)
		{
			if (!AtPoint) return;

			auto newPoint = point + movevec;
			if (!grid->IsPointValid(newPoint)) return;
			if (force || grid->GetTile(newPoint)->GetComponent<Tile>()->CanMoveToTile())
			{
				SetTarget(newPoint);
			}
		}

		void SetTarget(glm::ivec2& newPoint)
		{
			if (!grid->IsPointValid(newPoint)) return;
			direction = newPoint - point;
			point = newPoint;
			AtPoint = false;
		}

		BroadCaster m_signal;

	private:
		bool AtPoint{true};
		glm::ivec2 point{};
		glm::ivec2 direction{};
		Tilegrid* grid{};
		float speed{};

		

	};
}