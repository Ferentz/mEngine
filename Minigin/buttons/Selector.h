#pragma once

#include <components/GameComponent.h>

#include <functional>
#include <vector>

namespace dae
{
	class Button;

	class ButtonSelector : public GameComponent
	{
		size_t selectedButtonIDX{};
		std::vector<Button*> buttons{};

	public:

		ButtonSelector(GameObject& obj);

		void Update(float) override;

		void AddButton(Button& button);

		void SetSelected(size_t idx);
	};
}