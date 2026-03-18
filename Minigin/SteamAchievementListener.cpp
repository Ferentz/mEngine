#include "SteamAchievementListener.h"
#include "Minigin.h"

#if USE_STEAMWORKS
#include "mSteam.h"
#endif

void SteamEventListener::WinnerAchievement()
{
#if USE_STEAMWORKS
	if (dae::Minigin::SteamAchievements)
		dae::Minigin::SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
#endif
}