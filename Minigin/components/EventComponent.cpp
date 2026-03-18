#include "EventComponent.h"
#include "components/EventComponent.h"
#include "eventSystem/EventHash.h"

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
		m_signal.BroadCast(make_sdbm_hash("health changed"), GetParent());
		return m_value;
	}

	int IntTracker::AddValue(int val)
	{
		m_value += val;
		if (m_value >= m_maxValue)
		{
			m_signal.BroadCast(make_sdbm_hash("won"), GetParent());
		}
		else
		{
			m_signal.BroadCast(make_sdbm_hash("health changed"), GetParent());
		}
		return m_value;
	}

	HealthDisplay::HealthDisplay(GameObject& parent, const std::string& text, std::shared_ptr<Font> font, IntTracker& healthComponent)
		:TextComponent(parent, "Health : ", font),
		m_base{text},
		m_healthComponent{&healthComponent }
	{
		SetText(m_base + std::format(": {}", m_healthComponent->GetValue()));
		m_listener.AddEvent(make_sdbm_hash("health changed"),this,  &HealthDisplay::SetLife);
		m_listener.Subscribe(healthComponent.m_signal);
	}
	void dae::HealthDisplay::SetLife()
	{
//#if USE_STEAMWORKS
//		if (dae::Minigin::g_SteamAchievements)
//			dae::Minigin::g_SteamAchievements->SetAchievement("ACH_WIN_100_GAMES");
//#endif 
		
		SetText(m_base + std::format(": {}", m_healthComponent->GetValue()));
		//return m_healthComponent->GetValue();
	}
}