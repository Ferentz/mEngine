#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_main_impl.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
//#include "TextObject.h"
#include <Scene.h>

#include <filesystem>


//#include <Comand.h>
#include "command/GameCommand.h"
#include <SteamAchievementListener.h>
#include <eventSystem/EventHash.h>

#include "LevelDataContainer.h"

#include <inputsystems/InputManager.h>

#include <serviceLocator.h>
#include <sound/sound.h>

#include <GameObject.h>
#include <Components.h>
#include "GameTile.h"

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <iostream>


//using sdlInput = SDL_Scancode;
//using sdlInput = SDL_GamepadButton;

namespace fs = std::filesystem;

#include <windows.h>


static void load()
{
	std::cout << "hifrommain";
	std::vector<std::string> paths{ "sci_fi_door-6451.mp3" };
	dae::servicelocator soundService{};

	soundService.register_sound_system(std::make_unique<dae::SoundSystem_Logging>(paths));

	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	digger::LevelDataContainer::GetInstance().LoadData("levels.txt");


	/*auto& inputManager =
		dae::InputManager::GetInstance();
	auto input1 =
		inputManager.AddInputMethod(dae::InputType::keyboard);
	auto input2 =
		inputManager.AddInputMethod(dae::InputType::gamepad);*/


	//levels.BuildScene(0, scene, digger::gameMode::normal, input1, input2);

	digger::LevelDataContainer::GetInstance().BuildStartScreen(scene);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

		//================ ui/event stuff
		auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto player1_HealthDisplay = std::make_unique<dae::GameObject>();
	player1_HealthDisplay->AddComponent<dae::FPSComponent>("fps" , font);
	player1_HealthDisplay->SetPosition(50, 300);
	//auto explenation1 =
	//player1_HealthDisplay->AddNGetComponent<dae::TextComponent>("wasd to move. e to take damage. q to collect points", smallFont);
	/*auto healthDisplayComponent =
		player1_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("health", font, *healthComponent);
	healthDisplayComponent->SetPosition(100, 25);

	auto pointsDisplayComponent =
		player1_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("points", font, *pointsComponent);
	pointsDisplayComponent->SetPosition(100, 55);
	player1_HealthDisplay->AddComponent<dae::ServiceComponent>();*/

	scene.Add(std::move(player1_HealthDisplay));



	auto player2_HealthDisplay = std::make_unique<dae::GameObject>();
	player2_HealthDisplay->SetPosition(50, 420);
	//auto explenation2 =
	player2_HealthDisplay->AddNGetComponent<dae::TextComponent>("d-pad to move. right shoulder to take damage. left shoulder to collect points", smallFont);
	/*auto healthDisplayComponent2 =
		player2_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("health", font, *healthComponent2);
	healthDisplayComponent2->SetPosition(100, 25);

	auto pointsDisplayComponent2 =
		player2_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("points", font, *pointsComponent2);
	pointsDisplayComponent2->SetPosition(100, 55);
	player2_HealthDisplay->AddComponent<dae::ServiceComponent>();*/

	scene.Add(std::move(player2_HealthDisplay));

#if USE_STEAMWORKS

	//dae::Minigin::SteamListener->m_listener.AddEvent(dae::make_sdbm_hash("won"), dae::Minigin::SteamListener.get(), &SteamEventListener::WinnerAchievement);
	//dae::Minigin::SteamListener->m_listener.Subscribe(pointsComponent->m_signal);
	pointsComponent->m_signal.Register(*dae::Minigin::SteamListener);
	//dae::Minigin::SteamListener->m_listener.Subscribe(pointsComponent2->m_signal);
	pointsComponent2->m_signal.Register(*dae::Minigin::SteamListener);
#endif

}

FILE* stream;

int main(int, char*[]) {

	AllocConsole();
	freopen_s(&stream, "CONOUT$", "w", stdout);
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);

	FreeConsole();

    return 0;
}