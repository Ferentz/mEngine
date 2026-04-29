#include "SteamAchievementListener.h"
#include "SteamAchievementListener.h"
#include "Minigin.h"
#include "GameObject.h"

void SteamEventListener::WinnerAchievement()
{
#if USE_STEAMWORKS
	if (dae::Minigin::SteamAchievements)
		dae::Minigin::SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
#endif
}

void SteamEventListener::TuneIn(dae::EventId , dae::GameObject* )
{
#if USE_STEAMWORKS
	if (dae::Minigin::SteamAchievements)
		dae::Minigin::SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
#endif
}
