#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_gamepad.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
//#include "TextObject.h"
#include "Scene.h"

#include <filesystem>
#include "components/GameComponent.h"

#include "InputManager.h"
#include "Comand.h"
#include "SteamAchievementListener.h"
#include "eventSystem/EventHash.h"



namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::derivedComponent>(7.f);
	go->AddComponent<dae::TextureComponent>();
	go->GetComponents<dae::TextureComponent>()[0]->SetTexture("background.png");
	//go->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto component = go->AddNGetComponent<dae::TextureComponent>();
	component->SetTexture("logo.png");
	component->SetPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto go2 = std::make_unique<dae::GameObject>();
	//go2->SetParent(*go.get());
	auto textComponent = go2->AddNGetComponent<dae::TextComponent>("Programming 4 Assignment", font);
	textComponent->SetPosition(292, 20);
	textComponent->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(go2));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::FPSComponent>("FPS", font);
	scene.Add(std::move(go));


	// ============ players!
	auto player1 = std::make_unique<dae::GameObject>();
	player1->SetPosition(70, 0);
	auto imgComp = player1->AddNGetComponent<dae::TextureComponent>();
	imgComp->SetTexture("SPOSA1.png");
	auto healthComponent = player1->AddNGetComponent<dae::IntTracker>(3,3,0);
	auto pointsComponent = 
		player1->AddNGetComponent<dae::IntTracker>(0, 500, 0);


	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::left),
		static_cast<unsigned int>(SDL_SCANCODE_A), dae::KeyState::down);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::up),
		static_cast<unsigned int>(SDL_SCANCODE_W), dae::KeyState::down);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::right),
		static_cast<unsigned int>(SDL_SCANCODE_D), dae::KeyState::down);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::down),
		static_cast<unsigned int>(SDL_SCANCODE_S), dae::KeyState::down);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::HurtCommand>(player1.get(), healthComponent),
		static_cast<unsigned int>(SDL_SCANCODE_E), dae::KeyState::released);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::HurtCommand>(player1.get(), pointsComponent, false),
		static_cast<unsigned int>(SDL_SCANCODE_Q), dae::KeyState::down);

	//----------player 2

	auto player2 = std::make_unique<dae::GameObject>();
	player2->SetPosition(200, 100);
	auto imgComponent2 = player2->AddNGetComponent<dae::TextureComponent>();
	imgComponent2->SetTexture("SPOSA1.png");
	auto healthComponent2 = player2->AddNGetComponent<dae::IntTracker>(3, 3, 0);
	auto pointsComponent2 =
		player2->AddNGetComponent<dae::IntTracker>(0, 500, 0);

	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::left),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_LEFT), dae::KeyState::down);
	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::up),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_UP), dae::KeyState::down);
	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::right),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_RIGHT), dae::KeyState::down);
	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::down),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_DOWN), dae::KeyState::down);

	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::HurtCommand>(player2.get(), healthComponent2),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER), dae::KeyState::presed);

	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::HurtCommand>(player2.get(), pointsComponent2, false),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER), dae::KeyState::down);

	scene.Add(std::move(player1));
	scene.Add(std::move(player2));


	//================ ui/event stuff
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto player1_HealthDisplay = std::make_unique<dae::GameObject>();
	player1_HealthDisplay->SetPosition(50, 300);
	//auto explenation1 =
		player1_HealthDisplay->AddNGetComponent<dae::TextComponent>("wasd to move. e to take damage. q to collect points", smallFont);
	auto healthDisplayComponent = 
		player1_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("health", font, *healthComponent);
	healthDisplayComponent->SetPosition(100, 25);

	auto pointsDisplayComponent =
		player1_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("points", font, *pointsComponent);
	pointsDisplayComponent->SetPosition(100, 55);

	scene.Add(std::move(player1_HealthDisplay));



	auto player2_HealthDisplay = std::make_unique<dae::GameObject>();
	player2_HealthDisplay->SetPosition(50, 420);
	//auto explenation2 =
		player2_HealthDisplay->AddNGetComponent<dae::TextComponent>("d-pad to move. right shoulder to take damage. left shoulder to collect points", smallFont);
	auto healthDisplayComponent2 =
		player2_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("health", font, *healthComponent2);
	healthDisplayComponent2->SetPosition(100, 25);

	auto pointsDisplayComponent2 =
		player2_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("points", font, *pointsComponent2);
	pointsDisplayComponent2->SetPosition(100, 55);

	scene.Add(std::move(player2_HealthDisplay));

#if USE_STEAMWORKS
	
	dae::Minigin::SteamListener->m_listener.AddEvent(dae::make_sdbm_hash("won"), dae::Minigin::SteamListener.get(), &SteamEventListener::WinnerAchievement);
	dae::Minigin::SteamListener->m_listener.Subscribe(pointsComponent->m_signal);
	dae::Minigin::SteamListener->m_listener.Subscribe(pointsComponent2->m_signal);
#endif
	
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}