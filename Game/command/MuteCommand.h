#pragma once
#include <Comand.h>

#include "GameManager.h"
namespace digger
{

	class MuteCommand : public dae::Command
	{
	public:
		MuteCommand()
			:dae::Command{}
		{
		}
		virtual ~MuteCommand() override = default;

		virtual void Execute() override;
	private:
	};
}