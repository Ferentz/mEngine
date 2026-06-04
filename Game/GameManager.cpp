#include "GameManager.h"

namespace digger
{

	GameManager::GameManager(dae::GameObject& parent, PointsTracker& pt)
		:GameComponent{ parent }
		, pointsTracker{&pt }
	{
	}

	void GameManager::Start()
	{
		// we wanna get all gems and 
	}

	void GameManager::TuneIn(dae::EventId , dae::GameObject* )
	{

	}
}