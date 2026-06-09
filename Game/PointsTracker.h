#include <components/GameComponent.h>
#include <components/RenderComponent.h>
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

		dae::TextComponent* text;

	public:
		PointsTracker(dae::GameObject& parent, dae::TextComponent& textcomp);
		~PointsTracker() override;
		void TuneIn(dae::EventId message, dae::GameObject* subject) override;

		int GetPoints() const;
	};
}