#pragma once

#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

#include <string>
#include <vector>
namespace dae
{
	class InputMethod;
	class TextureComponent;
}

namespace digger
{

	class PointsTracker;

	enum class gameMode
	{
		normal,
		coop,
		vs
	};

	class GameManager : public dae::GameComponent , public dae::Listener
	{
		int remainingGems{};
		int remainingNobbins{};
		int const maxLives{ 4 };
		int remainingLives{ maxLives };
		int remainingLives2{ -1 };
		int curentLevel{};
		int const MaxLevel{ 3 };
		
		dae::GameObject* levelRoot;
		PointsTracker* pointsTracker;
		gameMode const mode;
		dae::InputMethod* player1{};
		dae::InputMethod* player2{};

		std::vector<std::vector<dae::TextureComponent*>> lifeDisplays;

	public:
		GameManager(dae::GameObject& parent, PointsTracker& pointsTracker, dae::GameObject& levelroot, gameMode mode, dae::InputMethod* p1, dae::InputMethod* p2);
		~GameManager();
		void Start() override;
		void Update(float) override;
		void TuneIn(dae::EventId message, dae::GameObject* subject) override;
		void ReturnNobbins();
		void GetAllNeeded();
		void NextLevel();
		void EnsureSkipFunc();
		void EndGame();
		void RecordHighScore();
		void SaveHighScore(std::string& name);
		void updateLife();
		dae::GameObject * GetObject(dae::InputMethod* playerInput);
	};
}