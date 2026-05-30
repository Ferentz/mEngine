#include "EventComponent.h"
#include "EventComponent.h"
#include "components/EventComponent.h"
#include "eventSystem/EventHash.h"
#include <eventSystem/EventStack.h>
//#include "mSteam.h"
//#include <format>
#include "Minigin.h"

namespace dae
{

	int IntTracker::SubtractValue(int val)
	{ 
		m_value -= val;
		if (m_value < m_minValue)
		{
			m_value = m_minValue;
			return m_value;
		}
		//explicit Event(EventId _id, GameComponent const * _subject)
		EventStack::GetEventStack().PushEvent(Event{ make_sdbm_hash("health changed"), this->GetGameObject()});
		m_signal.BroadCast(make_sdbm_hash("health changed"), GetGameObject());
		return m_value;
	}

	int IntTracker::AddValue(int val)
	{
		m_value += val;
		if (m_value >= m_maxValue)
		{
			m_signal.BroadCast(make_sdbm_hash("won"), GetGameObject());
		}
		else
		{
			m_signal.BroadCast(make_sdbm_hash("health changed"), GetGameObject());
		}
		return m_value;
	}

	HealthDisplay::HealthDisplay(GameObject& parent, const std::string& text, std::shared_ptr<Font> font, IntTracker& healthComponent)
		:TextComponent(parent, "Health : ", font),
		m_base{text},
		m_healthComponent{&healthComponent }
	{
		SetText(m_base + std::format(": {}", m_healthComponent->GetValue()));
		//m_listener.AddEvent(make_sdbm_hash("health changed"),this,  &HealthDisplay::SetLife);
		healthComponent.m_signal.Register(*this);
		//m_listener.Subscribe(healthComponent.m_signal);
	}
	void dae::HealthDisplay::SetLife()
	{
		
		SetText(m_base + std::format(": {}", m_healthComponent->GetValue()));
		//return m_healthComponent->GetValue();
	}
	void dae::HealthDisplay::TuneIn(EventId , GameObject* )
	{
		SetText(m_base + std::format(": {}", m_healthComponent->GetValue()));
	}
}