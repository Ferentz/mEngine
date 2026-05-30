#include "GameCommand.h"
#include "GameTile.h"

#include <glm/glm.hpp>
#include <tileGrid/GridMove.h>

void digger::DiggerMoveCommnad::Execute()
{
	auto gridMove{ GetSubject() };
	glm::ivec2 m_moveVec{};

	switch (m_moveDirection)
	{
	case dae::Direction::up:
		m_moveVec.y = -1;
		break;
	case dae::Direction::down:
		m_moveVec.y = 1;
		break;
	case dae::Direction::left:
		m_moveVec.x = -1;
		break;
	case dae::Direction::right:
		m_moveVec.x = 1;
		break;
	}

	//gridMove->MoveTo(m_moveVec);

	if (!gridMove->IsAtPoint()) return;

	auto newPoint = gridMove->GetPoint() + m_moveVec;
	if (!gridMove->GetGrid()->IsPointValid(newPoint)) return;
	auto tile = gridMove->GetGrid()->GetTile(newPoint)->GetComponent<GameTile>();
	if ( (m_moveDirection == dae::Direction::left || m_moveDirection == dae::Direction::right) || tile->CanMoveToTile())
	{
		//release poit
		gridMove->GetGrid()->GetTile(gridMove->GetPoint())->GetComponent<GameTile>()->ReleaseOccupancy(gridMove->GetGameObject());
		gridMove->SetTarget(newPoint);
		//aquire point
		tile->TakeOccupancy(gridMove->GetGameObject());
	}

}