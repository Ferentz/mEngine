#include "GameObject.h"
#include "Comand.h"
#include "Minigin.h"

#include "components/EventComponent.h"
#include "tileGrid/Gridmove.h"

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

	auto transform{ GetSubject()->GetTransform() };
	glm::vec2 const& r_pos{ transform->GetLocalTransform()->GetPosition() };
	float m_speed{20.f};
	transform->SetLocalPosition(
		r_pos.x + m_moveVec.x * m_speed * dae::Minigin::GetDeltaTime(),
		r_pos.y + m_moveVec.y * m_speed * dae::Minigin::GetDeltaTime());
	//GetComponent()->MakeDirty(); // why doesnt set position make everything dirty yet T.T
	//m_moveVec = {};

}

void dae::HurtCommand::Execute()
{
	if (m_doesSubtract)
	{
		GetSubject()->SubtractValue(1);
	}
	else
	{
		GetSubject()->AddValue(1);
	}
}

void dae::GridMoveCommand::Execute()
{

	bool horizontal{ false };
	int direction{1};

	switch (m_moveDirection)
	{
	case dae::Direction::up:
		horizontal = false;
		direction = -1;
		break;
	case dae::Direction::down:
		horizontal = false;
		direction = 1;
		break;
	case dae::Direction::left:
		horizontal = true;
		direction = -1;
		break;
	case dae::Direction::right:
		horizontal = true;
		direction = 1;
		break;
	}

	GetSubject()->Move(horizontal, direction);

}