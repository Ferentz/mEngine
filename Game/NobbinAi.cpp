#include "NobbinAi.h"

//#include "Digger.h"
#include "GameTile.h"
#include "Nobbin.h"
#include "Digger.h"

#include <GameObject.h>
#include <tileGrid/GridStar.h>
#include <tileGrid/GridMove.h>
#include <eventSystem/EventTypes.h>

namespace digger
{
	NobbinAI::NobbinAI(dae::GameObject& obj, dae::GridMove& mov, Nobbin& nob)
		:dae::GameComponent{ obj }
		, movement{&mov}
		, nobbin{&nob}
		, up{ &mov, dae::Direction::up, nob }
		, down{ &mov, dae::Direction::down, nob }
		, left{ &mov, dae::Direction::left, nob }
		, right{ &mov, dae::Direction::right, nob }
	{
		nob.signal.Register(*this);
	}
	void NobbinAI::Update(float delta)
	{
		if (playerone == nullptr)
		{
			auto player = movement->GetGrid()->GetGameObject()->GetComponent_ChildrenInclusive<digger::Digger>();
			if (player == nullptr) return; // no player
			playerone = player->GetGameObject();
		}

		if (auto digger = playerone->GetComponent<digger::Digger>())
		{
			if (!digger->IsAllive()) return; // no allive player
		}


		timer += delta;

		if (timer >= interval)
		{
			/*auto player = movement->GetGrid()->GetGameObject()->GetComponent_ChildrenInclusive<digger::Digger>();

			if(player == nullptr)
			auto const& objects{ dae::SceneManager::GetInstance().GetActiveScene()->GetObjects() };*/

			/*for (auto& object : objects)
			{
				if (auto player = object->GetComponent<digger::Digger>())
				{
					SetTarget(player->GetGameObject());
					break;
				}
			}*/

			timer = 0;
			node = 0;
			auto playerMov{ playerone->GetComponent<dae::GridMove>() };
			path = dae::GridStar::FindPath(*movement->GetGrid(), movement->GetClosestPoint(), playerMov->GetClosestPoint(), !nobbin->CanDig() );

		}

		if (node >= path.size()) return;

		auto aimPoint = path[node];
		auto grid = movement->GetGrid();

		auto aimpos = grid->GetGridLocationOfPoint(aimPoint);
		auto pos = GetGameObject()->GetLocalTransform()->GetPosition();

		float xDif = (aimpos.x - pos.x)* (aimpos.x - pos.x);
		float yDif = (aimpos.y - pos.y)* (aimpos.y - pos.y);



		if (xDif > yDif)
		{
			//horixontal move needed
			if (pos.x < aimpos.x)
			{
				//right
				right.Execute();
			}
			else
			{
				//left
				left.Execute();
			}
		}
		else if(xDif < yDif)
		{
			//vertical move needed
			if (pos.y < aimpos.y)
			{
				//down
				down.Execute();
			}
			else
			{
				//up
				up.Execute();
			}
		}

		if (grid->IsAtPoint(aimPoint, GetGameObject()->GetLocalTransform()->GetPosition()))
		{
			node++;
		}
	}
	void NobbinAI::TuneIn(dae::EventId , dae::GameObject* )
	{
		timer = interval; // trigger recalibration
	}
}