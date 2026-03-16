#pragma once
//#include "components/CommandComponent.h"


namespace dae
{
	class GameObject;


	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	

	class GameObjectCommand : public Command
	{
		GameObject* m_component;
	protected:
		GameObject* GetObject() const { return m_component; }
	public:
		GameObjectCommand(GameObject* object) :m_component{ object } {};
		virtual ~GameObjectCommand() override = default;
	};


	enum class Direction
	{
		up,
		down,
		left,
		right
	};

	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* object, Direction direction)
			:GameObjectCommand(object),
			m_moveDirection{ direction }
		{
		}
		virtual ~MoveCommand() override = default;

		virtual void Execute() override;
	private:
		Direction m_moveDirection;
	};

	/*

	we want to attch input to objects, and make it do things.

	we have the input handeler. which tracks the buttons.

	input comonent could keep track of the

	*/

}