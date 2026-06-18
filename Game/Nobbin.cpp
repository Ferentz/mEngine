#include "Nobbin.h"


#include <components/RenderComponent.h>
#include <tileGrid/GridMove.h>
#include <collision/Collider.h>
#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>



#include <GameTile.h>
#include <Digger.h>
#include <GoldBag.h>
#include "Spawner.h"

#include <iostream>

namespace digger
{
	Nobbin::Nobbin(dae::GameObject& parent, dae::GridMove& movement, dae::Collider& collider, glm::ivec2 base, Spawner* origin, dae::TextureComponent& tex)
		:Entity(parent, base)
		, gridMove{ &movement }
		, home{origin}
		, texture{ &tex }
	{
		//if (!home) removeOnDie = false;

		auto tile = movement.GetGrid()->GetTile(movement.GetClosestPoint())->GetComponent<GameTile>();
		tile->SetTraversed();
		//movement.m_signal.Register(*this);
		collider.m_signal.Register(*this);
		texture->SetTexture("nobbin.png");

		GetGameObject()->objectName = "nobbin";
	}


	void Nobbin::Update(float delta)
	{
		timer += delta;

		if (canDig)
		{
			if (timer >= digTime)
			{
				timer = 0;
				canDig = false;
				texture->SetTexture("nobbin.png");
				signal.BroadCast({}, GetGameObject()); // curently only one person listeneing
			}
		}
		else
		{
			if (timer >= noDigTime)
			{
				timer = 0;
				canDig = true;
				texture->SetTexture("hobbin.png");
				signal.BroadCast({}, GetGameObject());
			}
		}
	}

	void Nobbin::TuneIn(dae::EventId id, dae::GameObject* subject)
	{
		if (id == dae::make_sdbm_hash("collide"))
		{
			auto collider = subject->GetComponent<dae::Collider>();
			for (auto collision : collider->GetCollisions())
			{
				/*if (collision->GetGameObject()->GetComponent<GoldBag>())
				{
					std::cout << "hi";
				}*/

				if (auto digger = collision->GetGameObject()->GetComponent<Digger>())
				{
					digger->Die();
				}

				// else check if gold/collectable
			}
		}
	}

	void Nobbin::Die()
	{
		gridMove->canMove = false;
		dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("death enemy"),GetGameObject() });
		auto collider = GetGameObject()->GetComponent<dae::Collider>();
		collider->canCollide = false;
		isAllive = false;
		if (home)
		{
			GetGameObject()->MarkForDelete();
		}
	}
	void Nobbin::Respawn()
	{
		if (home)
		{
			home->ReturnNobin();
			GetGameObject()->MarkForDelete();
			return;
		}
		else
		{
			auto newPos{ gridMove->GetGrid()->GetGridLocationOfPoint(basePos) };
			GetGameObject()->m_transform.SetLocalPosition(newPos.x, newPos.y);
			gridMove->canMove = true;
			auto collider = GetGameObject()->GetComponent<dae::Collider>();
			collider->canCollide = true;
		}
	}
}