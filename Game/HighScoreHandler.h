#include <components/GameComponent.h>

#include <string>

namespace digger
{
	
	class HighScoreHandler : public dae::GameComponent
	{
		std::string username{};
		int x_offset{};
		int spacing{ 50 };

	public:
		HighScoreHandler(dae::GameObject& parent);
		~HighScoreHandler();
		std::string& GetUsername() { return username; }
		void Update(float delta) override;
		void NewChar(char in);
	};
}