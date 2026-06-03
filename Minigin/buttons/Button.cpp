#include "Button.h"

#include <GameObject.h>
#include <components/RenderComponent.h>

namespace dae
{
	Button::Button(GameObject& obj, std::string const& name, std::shared_ptr<Font> & font , SDL_Color const & col, SDL_Color const & selectCol, std::function<void()> func)
		:GameComponent{obj}
		, colour{col}
		, selectColour{selectCol}
		, Execute{ func }
	{
		text = GetGameObject()->AddNGetComponent<TextComponent>(name, font, col);
	}
	void Button::Sellect()
	{
		text->SetColor(selectColour);
	}
	void Button::Unselect()
	{
		text->SetColor(colour);
	}
}