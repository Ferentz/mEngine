#pragma once

#include <components/GameComponent.h>
#include <glm/glm.hpp>

namespace digger
{
	class Entity : public dae::GameComponent
	{
	protected:
		glm::ivec2 const basePos;
	public:

		Entity(dae::GameObject& parent, glm::ivec2 base)
			:dae::GameComponent(parent)
			, basePos{base}
		{}
		virtual ~Entity() = default;

		virtual void Die() = 0;
		virtual void Respawn() = 0;
	};
}