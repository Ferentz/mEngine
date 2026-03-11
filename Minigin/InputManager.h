#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <Xinput.h>

#pragma comment(lib, "xinput.lib")
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

	class XInputMethod final : public InputMethod
	{
		XINPUT_STATE m_currentState{};
		XINPUT_STATE m_previousState{};
		DWORD m_controllerIndex{};
		int m_buttonsPressedThisFrame{};
		int m_buttonsReleasedThisFrame{};

	public:
		virtual bool ProcessInput() override;

		virtual bool IsPressedThisFrame(unsigned int button) const override;
		virtual bool IsReleasedThisFrame(unsigned int button) const override;
		virtual bool IsDown(unsigned int button) const override;

		virtual void AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState) override;
	};

	class SDLInputMethod final : public InputMethod
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
		bool ProcessInput();

		InputManager()
		{
			m_inputs.emplace_back(std::make_unique<SDLInputMethod>());
			m_inputs.emplace_back(std::make_unique<XInputMethod>());
		}

		std::vector<std::unique_ptr<InputMethod>> m_inputs{};//std::unique_ptr<InputMethod> m_controlerInput{std::make_unique<XInputMethod>()};
		//std::unique_ptr<InputMethod> m_keyboardInput;

		InputMethod* GetInputMethod(int const idx);

		bool IsPressedThisFrame(unsigned int button) const;
		bool IsReleasedThisFrame(unsigned int button) const;
		bool IsDown(unsigned int button) const;

	private:
		
	};


	
}


/*
	


*/