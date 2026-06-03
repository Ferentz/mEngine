#include "Selector.h"

#include <inputsystems/InputManager.h>
#include <buttons/Button.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>

#include <memory>

namespace dae
{
	ButtonSelector::ButtonSelector(GameObject& obj)
		:GameComponent{obj}
	{
	}

	void ButtonSelector::Update(float)
	{
		if (buttons.size() == 0) return;

		const int numInputs{ 3 };
		SDL_Scancode input1[numInputs]{
			SDL_SCANCODE_W ,
			SDL_SCANCODE_S ,
			SDL_SCANCODE_SPACE };

		SDL_GamepadButton input2[numInputs]{
			SDL_GAMEPAD_BUTTON_DPAD_UP ,
			SDL_GAMEPAD_BUTTON_DPAD_DOWN,
			SDL_GAMEPAD_BUTTON_SOUTH};

		unsigned int inputCodes[numInputs]{};

		auto const & inputs{ InputManager::GetInstance().m_inputs };
		for (auto & input : inputs)
		{
			
			if (dynamic_cast<dae::KeyBoardInput*>(input.get()))
			{
				for (int i{}; i < numInputs; i++)
				{
					inputCodes[i] = static_cast<unsigned int>(input1[i]);
				}
			}
			else
			{
				for (int i{}; i < numInputs; i++)
				{
					inputCodes[i] = static_cast<unsigned int>(input2[i]);
				}
			}

			if (input->IsButtonReleasedThisFrame(inputCodes[0]))
			{
				// up
				buttons[selectedButtonIDX]->Unselect();
				selectedButtonIDX--;
				if (selectedButtonIDX >= buttons.size())
					selectedButtonIDX = buttons.size() - 1;
				buttons[selectedButtonIDX]->Sellect();
			}
			else if (input->IsButtonReleasedThisFrame(inputCodes[1]))
			{
				// down
				buttons[selectedButtonIDX]->Unselect();
				selectedButtonIDX++;
				if (selectedButtonIDX >= buttons.size())
					selectedButtonIDX = 0;
				buttons[selectedButtonIDX]->Sellect();
			}
			else if (input->IsButtonReleasedThisFrame(inputCodes[2]))
			{
				// press
				buttons[selectedButtonIDX]->Execute();
			}
		}
	}
	void ButtonSelector::AddButton(Button& button)
	{
		buttons.push_back(&button);
	}
	void ButtonSelector::SetSelected(int idx)
	{
		if (idx < 0 || idx >= buttons.size()) return;
		buttons[selectedButtonIDX]->Unselect();
		selectedButtonIDX = idx;
		buttons[selectedButtonIDX]->Sellect();
	}
}