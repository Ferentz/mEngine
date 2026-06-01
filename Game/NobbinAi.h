#pragma once

#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

#include "command/NobbinMoveCommand.h"
namespace dae
{
	class Command;
	class GridMove;
}

namespace digger
{
	//class Digger;

	class NobbinAI : public dae::GameComponent, public dae::Listener
	{

		// so, our little obbin noggin needs to get the player, and commands
		//and every so ofen play the A*

	public:
		NobbinAI(dae::GameObject& obj, dae::GridMove& mov, Nobbin& nob);

		void Update(float) override;

		void SetTarget(dae::GameObject* targ) { playerone = targ; }

		void TuneIn(dae::EventId id, dae::GameObject* subject) override;

	private:
		float timer{};
		float interval{5.f};
		
		int node{};

		std::vector<glm::ivec2> path;

		dae::GridMove* movement;
		Nobbin* nobbin;
		NobbinMoveCommnad up;
		NobbinMoveCommnad down;
		NobbinMoveCommnad left;
		NobbinMoveCommnad right;

		dae::GameObject* playerone{};
		//Digger* playerTwo{};
	};
}