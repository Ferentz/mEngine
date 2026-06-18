#include "HighScoreHandler.h"

#include <inputsystems/InputManager.h>
#include <components/RenderComponent.h>
#include <GameObject.h>
#include <array>

#include <eventSystem/EventStack.h>
#include <eventSystem/EventHash.h>
#include <ResourceManager.h>

#include <iostream>

namespace digger
{

     char sdlToAsci(unsigned int in)
	{
        if (in < 4 || in > 39) return '-';

        std::array<char, 40> codes{
            '\0', '\0', '\0', '\0', // 0 - 3
            // Letters
            'a', // 4  SDL_SCANCODE_A
            'b', // 5
            'c', // 6
            'd', // 7
            'e', // 8
            'f', // 9
            'g', // 10
            'h', // 11
            'i', // 12
            'j', // 13
            'k', // 14
            'l', // 15
            'm', // 16
            'n', // 17
            'o', // 18
            'p', // 19
            'q', // 20
            'r', // 21
            's', // 22
            't', // 23
            'u', // 24
            'v', // 25
            'w', // 26
            'x', // 27
            'y', // 28
            'z', // 29

            // Numbers
            '1', // 30 SDL_SCANCODE_1
            '2', // 31
            '3', // 32
            '4', // 33
            '5', // 34
            '6', // 35
            '7', // 36
            '8', // 37
            '9', // 38
            '0'  // 39 SDL_SCANCODE_0
        };
	
        return codes[in];
    }

	HighScoreHandler::HighScoreHandler(dae::GameObject& parent)
		:dae::GameComponent(parent)
	{

	}
    HighScoreHandler::~HighScoreHandler()
    {
        //std::cout << "goodbye";
    }
	void HighScoreHandler::Update(float)
	{

		dae::KeyBoardInput* inp{};

		auto const& inputs{ dae::InputManager::GetInstance().m_inputs };
		for (auto& input : inputs)
		{

			if (auto inptemp = dynamic_cast<dae::KeyBoardInput*>(input.get()))
			{
                inp = inptemp;
				break;
			}
		}
		for (unsigned int scancode{ SDL_SCANCODE_A }; scancode <= SDL_SCANCODE_0; scancode++)
		{
			if (inp->IsButtonReleasedThisFrame(scancode))
			{
                char newKey{ sdlToAsci(scancode) };
                NewChar(newKey);
                break;
			}
		}

        
	}

    void HighScoreHandler::NewChar(char in)
    {
        auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

        auto go = std::make_unique<dae::GameObject>();

        // make render component
        auto text = go->AddNGetComponent<dae::TextComponent>(std::string(1, in), font);
        text->m_offset.Translate(float(x_offset), 370);
        GetGameObject()->Add(std::move(go));
        x_offset += spacing;
        username.push_back(in);
        if (username.length() >= 3)
        {
            dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("name entered"), GetGameObject() });
            // send to save highscore.
        }
    }
}