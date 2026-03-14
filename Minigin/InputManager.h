#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>

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
		virtual bool ProcessInput() = 0;

		virtual bool IsPressedThisFrame(unsigned int button) const = 0;
		virtual bool IsReleasedThisFrame(unsigned int button) const = 0;
		virtual bool IsDown(unsigned int button) const = 0;

		//template<class T, typename ...Args>
		virtual void AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState) = 0;

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
		~ControllerInput();
		virtual bool ProcessInput() override;

		virtual bool IsPressedThisFrame(unsigned int button) const override;
		virtual bool IsReleasedThisFrame(unsigned int button) const override;
		virtual bool IsDown(unsigned int button) const override;

		virtual void AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState) override;
	};

	class KeyBoardInput final : public InputMethod
	{
		const bool* m_currentState{ nullptr };
		const bool* m_previousState{ nullptr };
	public:
		virtual bool ProcessInput() override;

		virtual bool IsPressedThisFrame(unsigned int button) const override;
		virtual bool IsReleasedThisFrame(unsigned int button) const override;
		virtual bool IsDown(unsigned int button) const override;
		virtual void AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState) override;
	};


	class InputManager final : public Singleton<InputManager>
	{
	public:

		enum class controllerButtons;

		bool ProcessInput();

		InputManager()
		{
			m_inputs.emplace_back(std::make_unique<KeyBoardInput>());
			#if __EMSCRIPTEN__
			m_inputs.emplace_back(std::make_unique<KeyBoardInput>());
			#else
			m_inputs.emplace_back(std::make_unique<ControllerInput>());
			#endif
			
		}

		std::vector<std::unique_ptr<InputMethod>> m_inputs{};//std::unique_ptr<InputMethod> m_controlerInput{std::make_unique<XInputMethod>()};
		//std::unique_ptr<InputMethod> m_keyboardInput;

		InputMethod* GetInputMethod(int const idx);

	private:
		
	};


	
}


/*
	


*/