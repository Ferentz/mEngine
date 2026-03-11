#pragma once
#include "GameComponent.h"

namespace dae
{
	enum class Direction
	{
		up,
		down,
		left,
		right
	};

	struct Vector2
	{
		float x{};
		float y{};
	};

	class MoveComponent final : public GameComponent
	{
	public:
		virtual ~MoveComponent() override = default;
		explicit MoveComponent(GameObject& parent, float val);

		virtual void Update(float) override;
		void Move(Direction direction);

	private: // i tried something, it didnt work. i need fps
		float m_speed{};
		glm::vec3 m_moveVec{};

		MoveComponent(const MoveComponent& other) = delete;
		MoveComponent(MoveComponent&& other) = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other) = delete;

	};
}