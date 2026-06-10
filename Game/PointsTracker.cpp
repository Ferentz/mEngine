#include "PointsTracker.h"

#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>

#include <GameObject.h>

namespace digger
{
	PointsTracker::PointsTracker(dae::GameObject& parent, dae::TextComponent& textcomp)
		:GameComponent{parent},
		text{ &textcomp }
	{
		text->SetText("00000");
		dae::EventStack::GetEventStack().Register(*this);

		GetGameObject()->objectName = "ptTracker";
	}
	PointsTracker::~PointsTracker()
	{
		dae::EventStack::GetEventStack().Unregister(this);
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
		auto pointsText{ std::to_string(points) };
		while (pointsText.length() < 5)
		{
			pointsText.insert(0, "0");
		}
		text->SetText(pointsText);
	}
	int PointsTracker::GetPoints() const
	{
		return points;
	}
}