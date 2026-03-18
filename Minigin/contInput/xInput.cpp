
#if __EMSCRIPTEN__
#else
#include "InputManager.h"

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

#include <SDL3/SDL_gamepad.h>
#include <map>

namespace dae
{
	

	class ControllerInput::ControllerImpl
	{
	public:
		static std::map<int, short> inputMap;
		

		XINPUT_STATE m_currentState{};
		XINPUT_STATE m_previousState{};
		DWORD m_controllerIndex{};
		int m_buttonsPressedThisFrame{};
		int m_buttonsReleasedThisFrame{};
		
	
		bool ProcessInput(std::vector<std::unique_ptr<BaseAction>>& actions);
		bool  IsPressedThisFrame(unsigned int button) const
		{
			return m_buttonsPressedThisFrame & button;
		}

		bool  IsReleasedThisFrame(unsigned int button) const
		{
			return m_buttonsReleasedThisFrame & button;
		}

		bool  IsDown(unsigned int button) const
		{
			return m_currentState.Gamepad.wButtons & button;
		}

	};

	std::map<int, short> ControllerInput::ControllerImpl::inputMap{
	{SDL_GAMEPAD_BUTTON_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_LEFT},
	{SDL_GAMEPAD_BUTTON_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT},
	{SDL_GAMEPAD_BUTTON_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_DOWN},
	{SDL_GAMEPAD_BUTTON_DPAD_UP, XINPUT_GAMEPAD_DPAD_UP},
	{SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER},
	{SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, XINPUT_GAMEPAD_LEFT_SHOULDER}
	};

	bool  ControllerInput::ControllerImpl::ProcessInput(std::vector<std::unique_ptr<BaseAction>>& actions)
	{
		CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
		XInputGetState(m_controllerIndex, &m_currentState);

		auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
		m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
		m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
		for (auto& binding : actions)
		{
			if (binding->m_state == KeyState::presed &&
				IsPressedThisFrame(binding->m_button))
			{
				binding->m_commands->Execute();
			}

			if (binding->m_state == KeyState::released &&
				IsReleasedThisFrame(binding->m_button))
			{
				binding->m_commands->Execute();
			}

			if (binding->m_state == KeyState::down &&
				IsDown(binding->m_button))
			{
				binding->m_commands->Execute();
			}
		}

		return true;
	}

	bool ControllerInput::ProcessInput()
	{
		return m_impl->ProcessInput(m_actions);
	}

	/*bool  ControllerInput::IsPressedThisFrame(unsigned int button) const
	{
		return m_impl->m_buttonsPressedThisFrame & button;
	}

	bool  ControllerInput::IsReleasedThisFrame(unsigned int button) const
	{
		return m_impl->m_buttonsReleasedThisFrame & button;
	}

	bool  ControllerInput::IsDown(unsigned int button) const
	{
		return m_impl->m_currentState.Gamepad.wButtons & button;
	}*/

	void  ControllerInput::AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState)
	{
		
		m_actions.emplace_back(std::make_unique<BaseAction>(std::move(command), unsigned int(ControllerImpl::inputMap[keybind]), triggerState));

	}

	ControllerInput::ControllerInput()
		:m_impl{ std::make_unique<ControllerInput::ControllerImpl>() }
	{

	}
	ControllerInput::~ControllerInput() = default;

}

#endif