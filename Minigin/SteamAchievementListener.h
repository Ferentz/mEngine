#pragma once

#include "eventSystem/Listener.h"

class SteamEventListener
{
public:
	void WinnerAchievement();
	dae::Listener m_listener{};
};
