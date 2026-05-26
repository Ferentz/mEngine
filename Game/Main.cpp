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


#include <Comand.h>
#include <SteamAchievementListener.h>
#include <eventSystem/EventHash.h>

#include <inputsystems/InputManager.h>

#include <serviceLocator.h>
#include <sound/sound.h>

#include <GameObject.h>
#include <Components.h>

#include <vector>
#include <string>
#include <memory>
#include <utility>

//using sdlInput = SDL_Scancode;
//using sdlInput = SDL_GamepadButton;

namespace fs = std::filesystem;

#include <type_traits>

template<typename T>
requires std::is_enum<T>::value
static std::unique_ptr<dae::GameObject> MakePlayer(std::string image, dae::InputType inputType, T* wasd_a1_a2)
{
	//static_assert(std::is_enum<T>::value, "T must be an enum");
	auto player = std::make_unique<dae::GameObject>();

	auto imgComp2 = player->AddNGetComponent<dae::TextureComponent>();
	imgComp2->SetTexture(image);


	auto healthComponent = player->AddNGetComponent<dae::IntTracker>(3, 3, 0);
	auto pointsComponent =
		player->AddNGetComponent<dae::IntTracker>(0, 500, 0);


	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.GetInputMethod(inputType)->AddAction(
		std::make_unique<dae::MoveCommand>(player.get(), dae::Direction::left),
		static_cast<unsigned int>(wasd_a1_a2[1]), dae::KeyState::down);

	inputManager.GetInputMethod(inputType)->AddAction(
		std::make_unique<dae::MoveCommand>(player.get(), dae::Direction::up),
		static_cast<unsigned int>(wasd_a1_a2[0]), dae::KeyState::down);

	inputManager.GetInputMethod(inputType)->AddAction(
		std::make_unique<dae::MoveCommand>(player.get(), dae::Direction::right),
		static_cast<unsigned int>(wasd_a1_a2[3]), dae::KeyState::down);

	inputManager.GetInputMethod(inputType)->AddAction(
		std::make_unique<dae::MoveCommand>(player.get(), dae::Direction::down),
		static_cast<unsigned int>(wasd_a1_a2[2]), dae::KeyState::down);

	inputManager.GetInputMethod(inputType)->AddAction(
		std::make_unique<dae::HurtCommand>(player.get(), healthComponent),
		static_cast<unsigned int>(wasd_a1_a2[4]), dae::KeyState::released);

	inputManager.GetInputMethod(inputType)->AddAction(
		std::make_unique<dae::HurtCommand>(player.get(), pointsComponent, false),
		static_cast<unsigned int>(wasd_a1_a2[5]), dae::KeyState::down);

	return player;
}

static void load()
{
	std::vector<std::string> paths{"sci_fi_door-6451.mp3"};
	dae::servicelocator soundService{};

	soundService.register_sound_system(std::make_unique<dae::SoundSystem_Logging>(paths));

	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::derivedComponent>(7.f);
	go->AddComponent<dae::TextureComponent>();
	go->GetComponents<dae::TextureComponent>()[0]->SetTexture("background.png");
	//go->SetTexture("background.png");
	scene.Add(std::move(go));

	//go = std::make_unique<dae::GameObject>();
	//auto component = go->AddNGetComponent<dae::TextureComponent>();
	//component->SetTexture("logo.png");
	//component->m_offset.SetPosition(358, 180);
	//scene.Add(std::move(go));

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//auto go2 = std::make_unique<dae::GameObject>();
	////go2->SetParent(*go.get());
	//auto textComponent = go2->AddNGetComponent<dae::TextComponent>("Programming 4 Assignment", font);
	//textComponent->m_offset.SetPosition(292, 20);
	//textComponent->SetColor({ 255, 255, 0, 255 });
	//scene.Add(std::move(go2));

	//go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::FPSComponent>("FPS", font);
	//scene.Add(std::move(go));


	//// ============ players!
	//SDL_Scancode input1 [6] { SDL_SCANCODE_W , SDL_SCANCODE_A ,SDL_SCANCODE_S, SDL_SCANCODE_D ,SDL_SCANCODE_E, SDL_SCANCODE_Q };
	//auto player1 = MakePlayer<SDL_Scancode>("CDBIG3X.BMP", dae::InputType::keyboard, input1);
	//player1->SetPosition(70, 0);

	////auto imgComp1 = player1->AddNGetComponent<dae::TextureComponent>();
	////imgComp1->SetTexture("SPOSA1.png");
	////auto imgComp2 = player1->AddNGetComponent<dae::TextureComponent>();
	////imgComp2->SetTexture("CDBIG3X.BMP");

	//////imgComp->SetTexture("digger/CDBIG3X.png");

	////auto healthComponent = player1->AddNGetComponent<dae::IntTracker>(3,3,0);
	////auto pointsComponent = 
	////	player1->AddNGetComponent<dae::IntTracker>(0, 500, 0);


	////auto& inputManager = dae::InputManager::GetInstance();

	////inputManager.GetInputMethod(0)->AddAction(
	////	std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::left),
	////	static_cast<unsigned int>(SDL_SCANCODE_A), dae::KeyState::down);

	////inputManager.GetInputMethod(0)->AddAction(
	////	std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::up),
	////	static_cast<unsigned int>(SDL_SCANCODE_W), dae::KeyState::down);

	////inputManager.GetInputMethod(0)->AddAction(
	////	std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::right),
	////	static_cast<unsigned int>(SDL_SCANCODE_D), dae::KeyState::down);

	////inputManager.GetInputMethod(0)->AddAction(
	////	std::make_unique<dae::MoveCommand>(player1.get(), dae::Direction::down),
	////	static_cast<unsigned int>(SDL_SCANCODE_S), dae::KeyState::down);

	////inputManager.GetInputMethod(0)->AddAction(
	////	std::make_unique<dae::HurtCommand>(player1.get(), healthComponent),
	////	static_cast<unsigned int>(SDL_SCANCODE_E), dae::KeyState::released);

	////inputManager.GetInputMethod(0)->AddAction(
	////	std::make_unique<dae::HurtCommand>(player1.get(), pointsComponent, false),
	////	static_cast<unsigned int>(SDL_SCANCODE_Q), dae::KeyState::down);

	////----------player 2
	//SDL_GamepadButton input2[6]{ SDL_GAMEPAD_BUTTON_DPAD_UP , SDL_GAMEPAD_BUTTON_DPAD_LEFT ,SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_DPAD_RIGHT ,SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER };
	//auto player2 = MakePlayer("SPOSA1.png", dae::InputType::gamepad, input2);
	//player2->SetPosition(200, 100);


	///*auto imgComponent2 = player2->AddNGetComponent<dae::TextureComponent>();
	//imgComponent2->SetTexture("SPOSA1.png");
	//auto healthComponent2 = player2->AddNGetComponent<dae::IntTracker>(3, 3, 0);
	//auto pointsComponent2 =
	//	player2->AddNGetComponent<dae::IntTracker>(0, 500, 0);

	//inputManager.GetInputMethod(1)->AddAction(
	//	std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::left),
	//	static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_LEFT), dae::KeyState::down);
	//inputManager.GetInputMethod(1)->AddAction(
	//	std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::up),
	//	static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_UP), dae::KeyState::down);
	//inputManager.GetInputMethod(1)->AddAction(
	//	std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::right),
	//	static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_RIGHT), dae::KeyState::down);
	//inputManager.GetInputMethod(1)->AddAction(
	//	std::make_unique<dae::MoveCommand>(player2.get(), dae::Direction::down),
	//	static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_DOWN), dae::KeyState::down);

	//inputManager.GetInputMethod(1)->AddAction(
	//	std::make_unique<dae::HurtCommand>(player2.get(), healthComponent2),
	//	static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER), dae::KeyState::presed);

	//inputManager.GetInputMethod(1)->AddAction(
	//	std::make_unique<dae::HurtCommand>(player2.get(), pointsComponent2, false),
	//	static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER), dae::KeyState::down);*/

	//scene.Add(std::move(player1));
	//scene.Add(std::move(player2));


	////================ ui/event stuff
	//auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	//auto player1_HealthDisplay = std::make_unique<dae::GameObject>();
	//player1_HealthDisplay->SetPosition(50, 300);
	////auto explenation1 =
	//	player1_HealthDisplay->AddNGetComponent<dae::TextComponent>("wasd to move. e to take damage. q to collect points", smallFont);
	///*auto healthDisplayComponent = 
	//	player1_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("health", font, *healthComponent);
	//healthDisplayComponent->SetPosition(100, 25);

	//auto pointsDisplayComponent =
	//	player1_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("points", font, *pointsComponent);
	//pointsDisplayComponent->SetPosition(100, 55);
	//player1_HealthDisplay->AddComponent<dae::ServiceComponent>();*/

	//scene.Add(std::move(player1_HealthDisplay));



	//auto player2_HealthDisplay = std::make_unique<dae::GameObject>();
	//player2_HealthDisplay->SetPosition(50, 420);
	////auto explenation2 =
	//	player2_HealthDisplay->AddNGetComponent<dae::TextComponent>("d-pad to move. right shoulder to take damage. left shoulder to collect points", smallFont);
	///*auto healthDisplayComponent2 =
	//	player2_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("health", font, *healthComponent2);
	//healthDisplayComponent2->SetPosition(100, 25);

	//auto pointsDisplayComponent2 =
	//	player2_HealthDisplay->AddNGetComponent<dae::HealthDisplay>("points", font, *pointsComponent2);
	//pointsDisplayComponent2->SetPosition(100, 55);
	//player2_HealthDisplay->AddComponent<dae::ServiceComponent>();*/

	//scene.Add(std::move(player2_HealthDisplay));

#if USE_STEAMWORKS
	
	//dae::Minigin::SteamListener->m_listener.AddEvent(dae::make_sdbm_hash("won"), dae::Minigin::SteamListener.get(), &SteamEventListener::WinnerAchievement);
	//dae::Minigin::SteamListener->m_listener.Subscribe(pointsComponent->m_signal);
	pointsComponent->m_signal.Register(*dae::Minigin::SteamListener);
	//dae::Minigin::SteamListener->m_listener.Subscribe(pointsComponent2->m_signal);
	pointsComponent2->m_signal.Register(*dae::Minigin::SteamListener);
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