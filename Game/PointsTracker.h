#include <components/GameComponent.h>
#include <eventSystem/Listener.h>
#include <eventSystem/BroadCaster.h>

namespace digger
{

	class PointsTracker : public dae::GameComponent, public dae::Listener
	{
		int const  lifeTreshHold{20000};
		int numTreshHolds{};
		int numGems{};
		int points{};

		dae::BroadCaster signal;

	public:
		PointsTracker(dae::GameObject& parent);
		void TuneIn(dae::EventId message, dae::GameObject* subject) override;

		int GetPoints() const;
	};
}