#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

#include <string>
namespace dae
{
	class InputMethod;
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
		int const maxLives{ 3 };
		int remainingLives{ maxLives };
		int curentLevel{};
		int const MaxLevel{ 3 };
		
		dae::GameObject* levelRoot;
		PointsTracker* pointsTracker;
		gameMode const mode;
		dae::InputMethod* player1{};
		dae::InputMethod* player2{};

	public:
		GameManager(dae::GameObject& parent, PointsTracker& pointsTracker, dae::GameObject& levelroot, gameMode mode);
		void Start() override;
		void Update(float) override;
		void TuneIn(dae::EventId message, dae::GameObject* subject) override;
		void GetAllNeeded();
		void NextLevel();
		void EndGame();
		void RecordHighScore();
		void SaveHighScore(std::string& name);
	};
}