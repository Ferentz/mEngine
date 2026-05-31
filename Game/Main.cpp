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


//#include <Comand.h>
#include "command/GameCommand.h"
#include <SteamAchievementListener.h>
#include <eventSystem/EventHash.h>

#include <inputsystems/InputManager.h>

#include <serviceLocator.h>
#include <sound/sound.h>

#include <GameObject.h>
#include <Components.h>
#include <TileGrid/Tilegrid.h>
#include <TileGrid/GridMove.h>
#include <collision/Collider.h>
#include "GameTile.h"

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "Player.h"
#include "GoldBag.h"

//using sdlInput = SDL_Scancode;
//using sdlInput = SDL_GamepadButton;

namespace fs = std::filesystem;

#include <type_traits>

template<typename T>
requires std::is_enum<T>::value
static std::unique_ptr<dae::GameObject> MakePlayer(std::string image,
	dae::InputType inputType
	, T* wasd_a1_a2
	, dae::Tilegrid& in_grid)
{
	//static_assert(std::is_enum<T>::value, "T must be an enum");
	auto player = std::make_unique<dae::GameObject>();

	auto imgComp2 = player->AddNGetComponent<dae::TextureComponent>();
	imgComp2->SetTexture(image);
	
	auto col = 
		player->AddNGetComponent<dae::Collider>();

	auto gridmove =
		player->AddNGetComponent<dae::GridMove>(in_grid, *col, glm::ivec2(2, 2), 1.f);
	gridmove->SetInfluence(false, false);
	//auto playerComp =
		player->AddNGetComponent<digger::Player>(*gridmove, *col);


	auto healthComponent = 
		player->AddNGetComponent<dae::IntTracker>(3, 3, 0);
	auto pointsComponent =
		player->AddNGetComponent<dae::IntTracker>(0, 500, 0);


	auto& inputManager = 
		dae::InputManager::GetInstance();
	auto input = 
		inputManager.AddInputMethod(inputType);

	input->AddAction(
		std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::left),
		static_cast<unsigned int>(wasd_a1_a2[1]), dae::KeyState::down);

	input->AddAction(
		std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::up),
		static_cast<unsigned int>(wasd_a1_a2[0]), dae::KeyState::down);

	input->AddAction(
		std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::right),
		static_cast<unsigned int>(wasd_a1_a2[3]), dae::KeyState::down);

	input->AddAction(
		std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::down),
		static_cast<unsigned int>(wasd_a1_a2[2]), dae::KeyState::down);

	input->AddAction(
		std::make_unique<dae::HurtCommand>(healthComponent),
		static_cast<unsigned int>(wasd_a1_a2[4]), dae::KeyState::released);

	input->AddAction(
		std::make_unique<dae::HurtCommand>(pointsComponent, false),
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

	go = std::make_unique<dae::GameObject>();
	auto component = go->AddNGetComponent<dae::TextureComponent>();
	component->SetTexture("logo.png");
	component->m_offset.SetPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto go2 = std::make_unique<dae::GameObject>();
	//go2->SetParent(*go.get());
	auto textComponent = go2->AddNGetComponent<dae::TextComponent>("Programming 4 Assignment", font);
	textComponent->m_offset.SetPosition(292, 20);
	textComponent->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(go2));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::FPSComponent>("FPS", font);
	scene.Add(std::move(go));

	auto grid =
		std::make_unique<dae::GameObject>();
	auto gridComp =
		grid->AddNGetComponent<dae::Tilegrid>(10, 10, 10.f);
	gridComp->MakeGrid<GameTile>(true, "ground.png", "ground_dug.png");
	grid->SetPosition(100, 100);
	
	// ============ players!
	SDL_Scancode input1 [6] { SDL_SCANCODE_W , SDL_SCANCODE_A ,SDL_SCANCODE_S, SDL_SCANCODE_D ,SDL_SCANCODE_E, SDL_SCANCODE_Q };
	SDL_GamepadButton input2[6]{ SDL_GAMEPAD_BUTTON_DPAD_UP , SDL_GAMEPAD_BUTTON_DPAD_LEFT ,SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_DPAD_RIGHT ,SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER };

	auto player1 = MakePlayer("CDBIG3X.BMP", dae::InputType::keyboard, input1, *gridComp);

	//----------player 2
	auto player2 = MakePlayer("SPOSA1.png", dae::InputType::gamepad, input2, *gridComp);


	scene.Add(std::move(grid));
	scene.Add(std::move(player1));
	scene.Add(std::move(player2));

	auto goldBag =
		std::make_unique<dae::GameObject>();
	auto imgComp = goldBag->AddNGetComponent<dae::TextureComponent>();
	imgComp->SetTexture("gold_bag.png");
	
	auto col =
		goldBag->AddNGetComponent<dae::Collider>();
	auto gridmove =
		goldBag->AddNGetComponent<dae::GridMove>(*gridComp,*col, glm::ivec2(5, 5), 1.f);
	gridmove->SetInfluence(true, false);
	//dae::TextureComponent& texture, dae::GridMove& gridmove, dae::Collider& collisionBox
	//auto goldComp =
		goldBag->AddNGetComponent<digger::GoldBag>(*imgComp, *gridmove, *col);

	scene.Add(std::move(goldBag));
	//================ ui/event stuff
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto player1_HealthDisplay = std::make_unique<dae::GameObject>();
	player1_HealthDisplay->SetPosition(50, 300);
	//auto explenation1 =
		player1_HealthDisplay->AddNGetComponent<dae::TextComponent>("wasd to move. e to take damage. q to collect points", smallFont);
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