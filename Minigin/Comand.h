#pragma once
//#include "components/CommandComponent.h"



namespace dae
{
	class GameObject;
	class GridMove;

	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	template<class T>
	class GameCommand : public Command
	{
		T* m_subject;
	protected:
		
	public:
		
		GameCommand(T* object)
			:m_subject{ object }
		{
		};
		virtual ~GameCommand() override = default;

		T* GetSubject() const { return m_subject; }
	};

	enum class Direction
	{
		up,
		down,
		left,
		right
	};

	class MoveCommand : public GameCommand<GameObject>
	{
	public:
		MoveCommand(GameObject* object, Direction direction)
			:GameCommand<GameObject>(object),
			m_moveDirection{ direction }
		{
		}
		virtual ~MoveCommand() override = default;

		virtual void Execute() override;
	private:
		Direction m_moveDirection;
	};

	class GridMoveCommand : public GameCommand<GridMove>
	{
	public:
		GridMoveCommand(GridMove* object, Direction direction)
			:GameCommand<GridMove>(object),
			m_moveDirection{ direction }
		{
		}
		virtual ~GridMoveCommand() override = default;

		virtual void Execute() override;
	private:
		Direction m_moveDirection;
	};


	class IntTracker;

	class HurtCommand : public GameCommand<IntTracker>
	{
	public:
		HurtCommand(IntTracker* health, bool subtract = true)
			:GameCommand<IntTracker>(health),
			m_doesSubtract{subtract}
		{
		}
		virtual ~HurtCommand() override = default;

		virtual void Execute() override;
	private:
		bool m_doesSubtract;
	};

	/*

	we want to attch input to objects, and make it do things.

	we have the input handeler. which tracks the buttons.

	input comonent could keep track of the

	*/

}