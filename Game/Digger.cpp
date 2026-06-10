#include "Digger.h"

#include <tileGrid/GridMove.h>
#include <collision/Collider.h>
#include <tileGrid/Tilegrid.h>
#include <GameObject.h>
#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>

#include "GameTile.h"
namespace digger
{
	Digger::Digger(dae::GameObject& obj, dae::GridMove& movement, dae::Collider& collider, glm::ivec2 base)
		:Entity(obj, base)
		, gridMove{ &movement }
	{
		auto tile = movement.GetGrid()->GetTile(movement.GetClosestPoint())->GetComponent<GameTile>();
		tile->SetTraversed();
		//movement.m_signal.Register(*this);
		collider.m_signal.Register(*this);
		GetGameObject()->objectName = "digger";
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
		isAllive = false;
		if (auto texture = GetGameObject()->GetComponent<dae::TextureComponent>())
		{
			texture->SetTexture("digger_dead.png");
		}
		if (auto collider = GetGameObject()->GetComponent<dae::Collider>())
		{
			collider->canCollide = false;
		}
		dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("died player"), GetGameObject()});
		gridMove->canMove = false;
	}
	void Digger::Respawn()
	{
		isAllive = true;
		if (auto texture = GetGameObject()->GetComponent<dae::TextureComponent>())
		{
			texture->SetTexture("digger.png");
		}
		if (auto collider = GetGameObject()->GetComponent<dae::Collider>())
		{
			collider->canCollide = true;
		}
		auto newPos{ gridMove->GetGrid()->GetGridLocationOfPoint(basePos) };
		GetGameObject()->m_transform.SetLocalPosition(newPos.x, newPos.y);
		gridMove->canMove = true;
	}
}