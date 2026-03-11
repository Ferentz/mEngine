#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"


#include "Comand.h"

namespace dae
{


	bool InputManager::ProcessInput()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				return false;
			}
			if (e.type == SDL_EVENT_KEY_DOWN) {

			}
			if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

			}
			// etc...
			//process event for IMGUI
			ImGui_ImplSDL3_ProcessEvent(&e);
		}

		for (auto& input : m_inputs)
		{
			input->ProcessInput();
		}

		return true;
	}

	InputMethod* InputManager::GetInputMethod(int const idx)
	{
		if (idx >= m_inputs.size()) return nullptr;
		return m_inputs[idx].get();
	}


	bool SDLInputMethod::ProcessInput()
	{
		//SDL_PumpEvents();  // <-- add this

		m_previousState = m_currentState;
		m_currentState = SDL_GetKeyboardState(nullptr);

		for (auto& binding : m_actions)
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

	bool  SDLInputMethod::IsPressedThisFrame(unsigned int button) const
	{
		if (m_previousState == nullptr)
			return m_currentState[button];
		bool buttonChange = m_currentState[button] ^ m_previousState[button];

		return buttonChange && m_currentState[button];
	}

	bool  SDLInputMethod::IsReleasedThisFrame(unsigned int button) const
	{
		if (m_previousState == nullptr)
			return !m_currentState[button];
		bool buttonChange = m_currentState[button] ^ m_previousState[button];

		return buttonChange && !m_currentState[button];

	}

	bool  SDLInputMethod::IsDown(unsigned int button) const
	{
		return m_currentState[button];
	}

	void  SDLInputMethod::AddAction(std::unique_ptr< Command> command, unsigned int keybind,  KeyState triggerState)
	{
		m_actions.emplace_back(std::make_unique< BaseAction>(std::move(command), keybind, triggerState));
	}





	bool  XInputMethod::ProcessInput()
	{
		CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
		XInputGetState(m_controllerIndex, &m_currentState);

		auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
		m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
		m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
		for (auto& binding : m_actions)
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



	bool  XInputMethod::IsPressedThisFrame(unsigned int button) const
	{
		return m_buttonsPressedThisFrame & button;
	}

	bool  XInputMethod::IsReleasedThisFrame(unsigned int button) const
	{
		return m_buttonsReleasedThisFrame & button;
	}

	bool  XInputMethod::IsDown(unsigned int button) const
	{
		return m_currentState.Gamepad.wButtons & button;
	}

	void  XInputMethod::AddAction(std::unique_ptr<Command> command, unsigned int keybind, KeyState triggerState)
	{
		m_actions.emplace_back(std::make_unique<BaseAction>(std::move(command), keybind, triggerState));

	}

}