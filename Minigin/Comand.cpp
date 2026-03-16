#include "GameObject.h"
#include "Comand.h"
#include "Minigin.h"
void dae::MoveCommand::Execute()
{
	glm::vec3 m_moveVec{};
	//GetComponent())->Move(m_moveDirection);
	switch (m_moveDirection)
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

	auto transform{ GetObject()->GetTransform() };
	glm::vec3 const& r_pos{ transform->GetLocalTransform()->GetPosition() };
	float m_speed{5.f};
	transform->SetLocalPosition(
		r_pos.x + m_moveVec.x * m_speed * dae::Minigin::GetDeltaTime(),
		r_pos.y + m_moveVec.y * m_speed * dae::Minigin::GetDeltaTime());
	//GetComponent()->MakeDirty(); // why doesnt set position make everything dirty yet T.T
	//m_moveVec = {};

}