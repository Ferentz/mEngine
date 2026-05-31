#include "Player.h"

#include <eventSystem/EventHash.h>

#include <tileGrid/GridMove.h>
#include <collision/Collider.h>
#include <tileGrid/Tilegrid.h>
#include <GameObject.h>

#include "GameTile.h"
namespace digger
{
	Player::Player(dae::GameObject& obj, dae::GridMove& movement, dae::Collider& collider)
		:dae::GameComponent(obj)
		, gridMove{ &movement }
	{
		auto tile = movement.GetGrid()->GetTile(movement.GetClosestPoint())->GetComponent<GameTile>();
		tile->TakeOccupancy(GetGameObject());
		//movement.m_signal.Register(*this);
		collider.m_signal.Register(*this);
	}

	void Player::TuneIn(dae::EventId id, dae::GameObject*)
	{
		switch (id)
		{
		case dae::make_sdbm_hash("arrived"):

			if (auto tileObj = gridMove->GetGrid()->GetTile(gridMove->GetClosestPoint()))
			{
				if (auto tile = tileObj->GetComponent<GameTile>())
				{
					tile->SetTraversed();
				}
			}
			break;
		case dae::make_sdbm_hash("collide"):
			break;
		default:
			break;
		}
	}
}