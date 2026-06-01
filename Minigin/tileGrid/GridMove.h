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
		GridMove(GameObject& parent, Tilegrid& in_grid, Collider& collider, glm::ivec2 startPoint, float speed);

		Tilegrid* GetGrid() { return grid; }

		glm::ivec2 const& GetClosestPoint() const { return closestPoint; }


		virtual bool Move(bool horizontal, int direction, bool influenced = false, float useSpeed = -1);

		
		glm::ivec2 DirectionToClosest();

		void SetInfluence(bool h, bool v)
		{
			h_collisionInfluence = h;
			v_collisionInfluence = v;
		}

		bool canMove{ true };

	private:
		bool m_vertical{ true};
		bool m_horizontal{ true };

		bool h_collisionInfluence{ true };
		bool v_collisionInfluence{ true };

		glm::ivec2 closestPoint{};
		glm::ivec2 goalPoint{};
		Tilegrid* grid{};
		float speed{};

		Collider* collider;

		bool CheckPosition();

		bool CheckCollisions(glm::vec2 moveVec, bool horizontal, int direction, float useSpeed, bool influenced = false);

	};
}