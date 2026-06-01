#pragma once

#include <components/GameComponent.h>

namespace digger
{
	class Entity : public dae::GameComponent
	{
	public:

		Entity(dae::GameObject& parent)
			:dae::GameComponent(parent)
		{}
		virtual ~Entity() = default;

		virtual void Die() = 0;
	};
}