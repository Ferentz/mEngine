#pragma once
#include <components/GameComponent.h>
#include <eventSystem/Listener.h>
#include <serviceLocator.h>
#include <eventSystem/EventTypes.h>

namespace dae
{
	class ServiceComponent : public GameComponent, public Listener
	{
	public:
		ServiceComponent(GameObject& parent);
		~ServiceComponent();
		servicelocator soundService{};
		virtual void TuneIn(EventId event, GameObject* subject) override;
	};
}