
//#if __EMSCRIPTEN__
#include "InputManager.h"

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <map>

namespace dae
{
	class ControllerInput::ControllerImpl
	{
		
	public:
		static unsigned int numConnected;
		unsigned int gamePadIDX{};

		std::map<unsigned int, KeyState> m_buttonStates{
		{SDL_GAMEPAD_BUTTON_DPAD_LEFT, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_DPAD_RIGHT, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_DPAD_DOWN, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_DPAD_UP, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_SOUTH, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_EAST, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_WEST, {KeyState::up}},
		{SDL_GAMEPAD_BUTTON_NORTH, {KeyState::up}}
		};

		bool ProcessInput(std::vector<std::unique_ptr<BaseAction>>& actions);

		static int GetConnectedGamePadCount()
		{
			SDL_Init(SDL_INIT_GAMEPAD);
			int count{ 0 };
			SDL_JoystickID* ids = SDL_GetGamepads(&count);
			SDL_OpenGamepad(ids[0]);

			return count;
		}
	};

	unsigned int ControllerInput::ControllerImpl::numConnected{};


	bool  ControllerInput::ControllerImpl::ProcessInput(std::vector<std::unique_ptr<BaseAction>>& actions)
	{
		SDL_PumpEvents();
		int count = 0;
		SDL_JoystickID* ids = SDL_GetGamepads(&count);
		SDL_Gamepad* gamepad = SDL_OpenGamepad(ids[gamePadIDX]);

		for (auto& buttonState : m_buttonStates)
		{
			bool isPressed = SDL_GetGamepadButton(gamepad, static_cast<SDL_GamepadButton>(buttonState.first));
			if (!isPressed)
			{
				if (buttonState.second == KeyState::down || buttonState.second == KeyState::presed)
				{
					buttonState.second = KeyState::released;
				}
				else
				{
					buttonState.second = KeyState::up;
				}
			}
			else
			{
				if (buttonState.second == KeyState::presed || buttonState.second == KeyState::down)
				{
					buttonState.second = KeyState::down;
				}
				else
				{
					buttonState.second = KeyState::presed;
				}
			}
		}

		for (auto& binding : actions)
		{
			unsigned int buttonKey{ binding->m_button };

			if(m_buttonStates[buttonKey] == binding->m_state)
				binding->m_commands->Execute();
		}
		SDL_free(ids);
		return true;
	}

	int ControllerInput::GetConnectedGamePadCount()
	{
		return ControllerInput::ControllerImpl::GetConnectedGamePadCount();
	}

	bool ControllerInput::ProcessInput()
	{
		return m_impl->ProcessInput(m_actions);
	}



	bool ControllerInput::IsButtonPressedThisFrame(unsigned int button)const
	{
		return m_impl->m_buttonStates[button] == KeyState::presed;
	}
	bool ControllerInput::IsButtonReleasedThisFrame(unsigned int button)const
	{
		return m_impl->m_buttonStates[button] == KeyState::released;
	}
	bool ControllerInput::IsButtonDown(unsigned int button)const
	{
		return m_impl->m_buttonStates[button] == KeyState::down;
	}



	void  ControllerInput::AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState)
	{
		m_actions.emplace_back(std::make_unique<BaseAction>(std::move(command), keybind, triggerState));

	}

	ControllerInput::ControllerInput()
		:m_impl{ std::make_unique<ControllerInput::ControllerImpl>() }
	{
		

		m_impl->gamePadIDX = m_impl->numConnected;
		m_impl->numConnected++;
	}
	ControllerInput::~ControllerInput() = default;

}

//#endif