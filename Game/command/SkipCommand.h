#pragma once
#include <Comand.h>

#include "GameManager.h"
namespace digger
{

	class SkipCommand : public dae::GameCommand<GameManager>
	{
	public:
		SkipCommand(GameManager* object)
			:GameCommand<GameManager>(object)
		{
		}
		virtual ~SkipCommand() override = default;

		virtual void Execute() override;
	private:
		dae::Direction m_moveDirection;
	};
}