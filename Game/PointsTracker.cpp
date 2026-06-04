#include "PointsTracker.h"

#include <eventSystem/EventHash.h>

namespace digger
{
	PointsTracker::PointsTracker(dae::GameObject& parent)
		:GameComponent{parent}
	{
	}
	void PointsTracker::TuneIn(dae::EventId message, dae::GameObject* )
	{
		if (message == dae::make_sdbm_hash("collect gold"))
		{
			points += 500;
		}
		else if (message == dae::make_sdbm_hash("collect gem"))
		{
			points += 25;
			numGems++;
			if (numGems >= 8)
			{
				points += 250;
				numGems = 0;
			}
		}
		else if(message == dae::make_sdbm_hash("death enemy"))
		{
			points += 250;
		}

		if ( points / lifeTreshHold > numTreshHolds)
		{
			numTreshHolds++;
			signal.BroadCast(dae::make_sdbm_hash("treshhold collected"), GetGameObject());
		}
	}
	int PointsTracker::GetPoints() const
	{
		return points;
	}
}