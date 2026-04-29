#pragma once

#include "eventSystem/Listener.h"

class SteamEventListener : public dae::Listener
{
public:
	SteamEventListener() = default;
	virtual ~SteamEventListener() override = default;
	void WinnerAchievement();
	//dae::Listener m_listener{};
	virtual void TuneIn(dae::EventId message, dae::GameObject* subject);
};
