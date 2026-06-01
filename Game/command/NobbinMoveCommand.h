#pragma once
#include <Comand.h>


namespace digger
{
	class Nobbin;

	class NobbinMoveCommnad : public dae::GameCommand<dae::GridMove>
	{
	public:
		NobbinMoveCommnad(dae::GridMove* object, dae::Direction direction, Nobbin& nob)
			:GameCommand<dae::GridMove>(object),
			m_moveDirection{ direction },
			nobbin{&nob}
		{
		}
		virtual ~NobbinMoveCommnad() override = default;

		virtual void Execute() override;
	private:
		dae::Direction m_moveDirection;
		Nobbin* nobbin;
	};
}