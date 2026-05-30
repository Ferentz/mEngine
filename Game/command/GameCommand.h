#pragma once
#include <Comand.h>


namespace digger
{
	class DiggerMoveCommnad : public dae::GameCommand<dae::GridMove>
	{
	public:
		DiggerMoveCommnad(dae::GridMove * object, dae::Direction direction)
			:GameCommand<dae::GridMove>(object),
			m_moveDirection{ direction }
		{
		}
		virtual ~DiggerMoveCommnad() override = default;

		virtual void Execute() override;
	private:
		dae::Direction m_moveDirection;
	};
}