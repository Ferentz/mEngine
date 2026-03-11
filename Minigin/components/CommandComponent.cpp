#include "CommandComponent.h"
#include "GameObject.h"
namespace dae
{
	MoveComponent::MoveComponent(GameObject& parent, float val)
		:GameComponent{ parent },
		m_speed{val}
	{
	}
	void MoveComponent::Update(float deltaTime)
	{
		auto transform{ GetParent()->GetTransform() };
		glm::vec3 const & r_pos{ transform->GetLocalTransform()->GetPosition() };
	
		transform->SetPosition(
			r_pos.x + m_moveVec.x * m_speed * deltaTime,
			r_pos.y + m_moveVec.y *m_speed* deltaTime);
		GetParent()->MakeDirty(); // why doesnt set position make everything dirty yet T.T
		m_moveVec = {};
	}
	void MoveComponent::Move(Direction direction)
	{
		switch (direction)
		{
		case dae::Direction::up:
			m_moveVec.y = -1.f;
			break;
		case dae::Direction::down:
			m_moveVec.y = 1.f;
			break;
		case dae::Direction::left:
			m_moveVec.x = -1.f;
			break;
		case dae::Direction::right:
			m_moveVec.x = 1.f;
			break;
		}
	}
}