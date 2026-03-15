
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

		/*int m_controllerIndex{};
		int m_buttonsPressedThisFrame{};
		int m_buttonsReleasedThisFrame{};*/

		std::vector<KeyState> m_buttonStates;


		bool ProcessInput(std::vector<std::unique_ptr<BaseAction>>& actions);
		/*bool  IsPressedThisFrame(unsigned int button) const
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
		}*/

	};

	bool  ControllerInput::ControllerImpl::ProcessInput(std::vector<std::unique_ptr<BaseAction>>& actions)
	{

		//int count = 0;
		//SDL_JoystickID* ids = SDL_GetGamepads(&count);
		//SDL_Gamepad* gamepad = NULL;

		//// Iterate over the list of gamepads
		//for (int i = 0; i < count; i++) {
		//	SDL_Gamepad* gamepd = SDL_OpenGamepad(ids[i]);
		//	if (gamepad == NULL) {
		//		gamepad = gamepd;
		//	}
		//	SDL_GetGamepadButton(gamepd, )

		//	std::cout << "Gamepad connected: " << SDL_GetGamepadName(gamepd) << "\n";

		//	// Close the other gamepads
		//	if (i > 0) {
		//		SDL_CloseGamepad(gamepd);
		//	}
		//}
		//if (!gamepad) {
		//	std::cerr << "Failed to open gamepad: " << SDL_GetError() << "\n";
		//	SDL_Quit();
		//	return 1;
		//}

		//bool running = true;
		//SDL_Event event;

		//while (running) {
		//	while (SDL_PollEvent(&event)) {
		//		switch (event.type) {
		//		case SDL_EVENT_QUIT:
		//			running = false;
		//			break;

		//		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		//			std::cout << "Button pressed: "
		//				<< SDL_GetGamepadStringForButton((SDL_GamepadButton)event.gbutton.button)
		//				<< "\n";
		//			break;

		//		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		//			std::cout << "Button released: "
		//				<< SDL_GetGamepadStringForButton((SDL_GamepadButton)event.gbutton.button)
		//				<< "\n";
		//			break;

		//		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		//			std::cout << "Axis moved: "
		//				<< SDL_GetGamepadStringForAxis((SDL_GamepadAxis)event.gaxis.axis)
		//				<< " Value: " << event.gaxis.value << "\n";
		//			break;
		//		}
		//	}

		//	SDL_Delay(16); // Sleep 16ms (~60FPS)
		//}

		SDL_PumpEvents();
		int count = 0;
		SDL_JoystickID* ids = SDL_GetGamepads(&count);
		//SDL_Gamepad* gamepad = NULL;
		SDL_Gamepad* gamepad = SDL_OpenGamepad(ids[0]);

		//for (int b = 0; b < SDL_GAMEPAD_BUTTON_COUNT; b++)
		//{
		//	if (SDL_GetGamepadButton(gamepad, (SDL_GamepadButton)b))
		//	{
		//		printf("Pressed: %d\n", b);
		//	}
		//}

		//int joystickCount = 0;
		////SDL_JoystickID* joysticks =
		//	SDL_GetJoysticks(&joystickCount);

		//if (joystickCount > 0)
		//{
		//	printf("Joysticks: %d\n", joystickCount);
		//}

		//if (SDL_IsGamepad(ids[0]))
		//{
		//	//SDL_Gamepad* gamepd =
		//		SDL_OpenGamepad(ids[0]);
		//}



		int i{};
		for (auto& binding : actions)
		{

			bool isPressed{
				SDL_GetGamepadButton(gamepad, static_cast<SDL_GamepadButton>(binding->m_button)) };

			if (!isPressed)
			{
				if (m_buttonStates[i] != KeyState::released)
				{
					if (binding->m_state == KeyState::released)
					{
						binding->m_commands->Execute();
					}
					m_buttonStates[i] = KeyState::released;
				}
			}
			else
			{
				if (m_buttonStates[i] == KeyState::presed || m_buttonStates[i] == KeyState::down)
				{
					if (binding->m_state == KeyState::down)
					{
						binding->m_commands->Execute();
					}
					m_buttonStates[i] = KeyState::down;
				}
				else if (m_buttonStates[i] == KeyState::released)
				{
					if (binding->m_state == KeyState::presed)
					{
						binding->m_commands->Execute();
					}
					m_buttonStates[i] = KeyState::presed;
				}
			}
			i++;
		}
		SDL_free(ids);
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
		m_impl->m_buttonStates.push_back(KeyState::up);
		m_actions.emplace_back(std::make_unique<BaseAction>(std::move(command), unsigned int(keybind), triggerState));

	}

	ControllerInput::ControllerInput()
		:m_impl{ std::make_unique<ControllerInput::ControllerImpl>() }
	{
		SDL_Init(SDL_INIT_GAMEPAD);
	}
	ControllerInput::~ControllerInput() = default;

}

//#endif