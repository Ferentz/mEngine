#include "GameCommand.h"
#include "GameTile.h"

#include <glm/glm.hpp>
#include <tileGrid/GridMove.h>

void digger::DiggerMoveCommnad::Execute()
{
	bool horizontal{ false };
	int direction{ 1 };

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

	auto gridMove = GetSubject();

	if (gridMove->Move(horizontal, direction))
	{
		auto tile = gridMove->GetGrid()->GetTile(gridMove->GetClosestPoint())->GetComponent<GameTile>();
		if (tile)
		{
			tile->SetTraversed();
		}
	}


}