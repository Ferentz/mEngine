#pragma once
#include "components/CommandComponent.h"

namespace dae
{
	class GameComponent;


	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	

	class GameObjectCommand : public Command
	{
		GameComponent* m_component;
	protected:
		GameComponent* GetComponent() const { return m_component; }
	public:
		GameObjectCommand(GameComponent* object) :m_component{ object } {};
		virtual ~GameObjectCommand() override = default;
	};


	

	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameComponent* object, Direction direction)
			:GameObjectCommand(object),
			m_moveDirection{ direction }
		{
		}
		virtual ~MoveCommand() override = default;

		virtual void Execute() override
		{
			static_cast<MoveComponent*>(GetComponent())->Move(m_moveDirection);
		}
	private:
		Direction m_moveDirection;
	};

	/*

	we want to attch input to objects, and make it do things.

	we have the input handeler. which tracks the buttons.

	input comonent could keep track of the

	*/

}