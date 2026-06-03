#include "Nobbin.h"


#include <components/RenderComponent.h>
#include <tileGrid/Gridmove.h>
#include <collision/Collider.h>



#include <GameTile.h>
#include <Digger.h>

namespace digger
{
	Nobbin::Nobbin(dae::GameObject& parent, dae::GridMove& movement, dae::Collider& collider, dae::TextureComponent& tex)
		:Entity(parent)
		, gridMove{ &movement }
		, texture{ &tex }
	{
		auto tile = movement.GetGrid()->GetTile(movement.GetClosestPoint())->GetComponent<GameTile>();
		tile->SetTraversed();
		//movement.m_signal.Register(*this);
		collider.m_signal.Register(*this);
		texture->SetTexture("nobbin.png");
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
		if (removeOnDie) GetGameObject()->MarkForDelete();

		auto collider = GetGameObject()->GetComponent<dae::Collider>();
		collider->canCollide = false;
		isAllive = false;
	}
}