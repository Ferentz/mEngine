#pragma once

#include <components/GameComponent.h>

#include <functional>
#include <string>
#include <memory>

#include <SDL3/SDL_pixels.h>

namespace dae
{
	class TextComponent;
	class Button : public GameComponent
	{
		TextComponent* text{};
		SDL_Color const colour;
		SDL_Color const selectColour;
	public:
		
		Button(GameObject& obj, std::string const& name, std::shared_ptr<Font>& font, SDL_Color const& col, SDL_Color const& selectCol, std::function<void()> func);
		void Sellect();
		void Unselect();

		std::function<void()> Execute;
	};
}