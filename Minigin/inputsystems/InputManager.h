#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include <utility>

//#include <xinput.h>


#include "Comand.h"

namespace dae
{
	enum class KeyState
	{
		presed,
		released,
		down,
		up
	};

	enum class InputType
	{
		keyboard,
		gamepad
	};

	class BaseAction final
	{
	public:
		BaseAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState)
			: m_button{ keybind },
			m_commands{std::move(command)},
			m_state{ triggerState }
		{}
		unsigned int m_button{};
		std::unique_ptr<Command>m_commands{};
		KeyState m_state{};

		BaseAction(const BaseAction& other) = delete;
		BaseAction(BaseAction&& other) = delete;
		BaseAction& operator=(const BaseAction& other) = delete;
		BaseAction& operator=(BaseAction&& other) = delete;

	};

	class InputMethod
	{
	public:

		static std::vector<InputType> GetAvailableInputs();

		InputMethod() = default;
		virtual ~InputMethod() = default;

		virtual bool ProcessInput() = 0;

		virtual bool IsButtonPressedThisFrame(unsigned int button)const = 0;
		virtual bool IsButtonReleasedThisFrame(unsigned int button)const = 0;
		virtual bool IsButtonDown(unsigned int button)const = 0;

		//template<class T, typename ...Args>
		virtual void AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState) = 0;
		virtual void ClearActions() { m_actions.clear(); };
		std::vector<std::unique_ptr<BaseAction>> const & GetActions() { return m_actions; }

	protected:

		//virtual 
		std::vector<std::unique_ptr<BaseAction>> m_actions;
	};

	class ControllerInput final : public InputMethod
	{
		class ControllerImpl;

		std::unique_ptr<ControllerImpl> m_impl;
	public:
		ControllerInput();
		virtual ~ControllerInput() override;

		static int GetConnectedGamePadCount();

		virtual bool ProcessInput() override;

		virtual bool IsButtonPressedThisFrame(unsigned int button)const override;
		virtual bool IsButtonReleasedThisFrame(unsigned int button)const override;
		virtual bool IsButtonDown(unsigned int button)const override;

		virtual void AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState) override;
	};

	class KeyBoardInput final : public InputMethod
	{
		const bool* m_currentStatePtr{ nullptr };
		std::vector<bool> m_previousState{};
		std::vector<bool> m_currentState{};
		int m_numKeys{};


	public:
		KeyBoardInput();
		virtual ~KeyBoardInput() override = default;

		static bool HasKeyboard();

		 bool ProcessInput() override;

		 virtual bool IsButtonPressedThisFrame(unsigned int button) const override;
		 virtual bool IsButtonReleasedThisFrame(unsigned int button)const override;
		 virtual bool IsButtonDown(unsigned int button)const override;

		
		 void AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState) override;
	};


	class InputManager final : public Singleton<InputManager>
	{
	public:

		enum class controllerButtons;

		bool ProcessInput();

		InputManager() = default;
		~InputManager() override = default;

		InputMethod* AddInputMethod(InputType input)
		{
			switch (input)
			{
			case dae::InputType::keyboard:
				m_inputs.emplace_back(std::make_unique<KeyBoardInput>());
				break;
			case dae::InputType::gamepad:
				m_inputs.emplace_back(std::make_unique<ControllerInput>());
				break;
			default:
				break;
			}
			return m_inputs.back().get();
		}

		void ReleaseInputs()
		{
			for (auto & input : m_inputs)
			{
				input.reset();
			}
		}
		
		std::vector<std::unique_ptr<InputMethod>> m_inputs{};

		InputMethod* GetInputMethod(InputType const idx);

	private:
		
	};
}