#include "InputManager.h"
#include "InputManager.h"
#include "InputManager.h"
#include "InputManager.h"
#include "InputManager.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>


#include "Comand.h"

namespace dae
{
	std::vector<InputType> InputMethod::GetAvailableInputs()
	{
		std::vector<InputType> inputs;

		if (KeyBoardInput::HasKeyboard())
		{
			inputs.push_back(InputType::keyboard);
		}

		const int gamepadCount = ControllerInput::GetConnectedGamePadCount();

		for (int i{}; i < gamepadCount; ++i)
		{
			inputs.push_back(InputType::gamepad);
		}

		return inputs;
	}

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

	InputMethod* InputManager::GetInputMethod(InputType const idx)
	{
		//if (size_t(idx) >= m_inputs.size()) return nullptr;
		return m_inputs[size_t(idx)].get();
	}

	KeyBoardInput::KeyBoardInput()
		:InputMethod()
	{
		// Get current state + size
		m_currentStatePtr = SDL_GetKeyboardState(&m_numKeys);

		m_currentState.resize(m_numKeys, 0);
		m_previousState.resize(m_numKeys, 0);
		
	}

	bool KeyBoardInput::HasKeyboard()
	{
		return SDL_HasKeyboard();
	}

	bool KeyBoardInput::ProcessInput()
	{
		//SDL_PumpEvents();  // <-- add this

		//m_previousState = m_currentState;
		std::copy(m_currentState.begin(), m_currentState.end(), m_previousState.begin());

		m_currentStatePtr = SDL_GetKeyboardState(nullptr);
		std::copy(m_currentStatePtr, m_currentStatePtr + m_numKeys, m_currentState.begin());
		

		for (auto& binding : m_actions)
		{
			if (binding->m_state == KeyState::presed &&
				IsButtonPressedThisFrame(binding->m_button))
			{
				binding->m_commands->Execute();
			}

			if (binding->m_state == KeyState::released &&
				IsButtonReleasedThisFrame(binding->m_button))
			{
				binding->m_commands->Execute();
			}

			if (binding->m_state == KeyState::down &&
				IsButtonDown(binding->m_button))
			{
				binding->m_commands->Execute();
			}
		}

		//std::copy(m_currentStatePtr, m_currentStatePtr + m_numKeys, m_previousState.begin());



		return true;
	}
	bool KeyBoardInput::IsButtonPressedThisFrame(unsigned int button) const
	{
		bool buttonChange = m_currentState[button] ^ m_previousState[button];

		return buttonChange && m_currentState[button];
	}

	bool KeyBoardInput::IsButtonReleasedThisFrame(unsigned int button)const
	{
		bool buttonChange = m_currentState[button] ^ m_previousState[button];

		return buttonChange && !m_currentState[button];
	}

	bool KeyBoardInput::IsButtonDown(unsigned int button)const
	{
		return m_currentState[button];
	}

	void  KeyBoardInput::AddAction(std::unique_ptr< Command> command, unsigned int keybind,  KeyState triggerState)
	{
		m_actions.emplace_back(std::make_unique< BaseAction>(std::move(command), keybind, triggerState));
	}

}