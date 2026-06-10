#include "ServiceComponent.h"
#include <components/ServiceComponent.h>
#include <sound/sound.h>
#include "eventSystem/EventHash.h"
#include <eventSystem/EventStack.h>
#include <GameObject.h>

namespace dae
{
	ServiceComponent::ServiceComponent(GameObject& parent)
		:GameComponent{ parent },
		Listener{}
	{
		EventStack::GetEventStack().Register(*this);
	}
	ServiceComponent::~ServiceComponent()
	{
		EventStack::GetEventStack().Unregister(this);
	}
	void ServiceComponent::TuneIn(EventId message, GameObject* )
	{
		if (message == make_sdbm_hash("health changed"))
			soundService.get_sound_system().Play(0, 0.f);
	}
}