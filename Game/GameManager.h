#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

namespace digger
{

	class PointsTracker;

	class GameManager : public dae::GameComponent , public dae::Listener
	{
		int remainingGems{};
		int remainingNobbins{};
		int remainingLives{3};

		PointsTracker* pointsTracker;

	public:
		GameManager(dae::GameObject& parent, PointsTracker& pointsTracker);
		void Start() override;
		void TuneIn(dae::EventId message, dae::GameObject* subject) override;
	};
}