#include "Digger.h"

#include <eventSystem/EventHash.h>

#include <tileGrid/GridMove.h>
#include <collision/Collider.h>
#include <tileGrid/Tilegrid.h>
#include <GameObject.h>

#include "GameTile.h"
namespace digger
{
	Digger::Digger(dae::GameObject& obj, dae::GridMove& movement, dae::Collider& collider)
		:Entity(obj)
		, gridMove{ &movement }
	{
		auto tile = movement.GetGrid()->GetTile(movement.GetClosestPoint())->GetComponent<GameTile>();
		tile->SetTraversed();
		//movement.m_signal.Register(*this);
		collider.m_signal.Register(*this);
	}

	void Digger::TuneIn(dae::EventId , dae::GameObject*)
	{
		/*switch (id)
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
		}*/
	}

	void Digger::Die()
	{
		if (auto texture = GetGameObject()->GetComponent<dae::TextureComponent>())
		{
			texture->SetTexture("digger_dead.png");
		}
		if (auto collider = GetGameObject()->GetComponent<dae::Collider>())
		{
			collider->canCollide = false;
		}
		gridMove->canMove = false;
	}
}