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

	/*go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::UIComponent>("exercise 1");
	go->AddComponent<dae::UIComponent_2>("exercise 2");
	scene.Add(std::move(go));*/


	auto rotBase = std::make_unique<dae::GameObject>();
	rotBase->SetPosition(200, 200);
	//rotBase->AddComponent<dae::RotatorComponent>();
	

	//rotatorComp1->SetRotationSpeed(-2.5);

	auto aco1 = std::make_unique<dae::GameObject>();
	//auto rotatorComp2 = aco1->AddNGetComponent<dae::RotatorComponent>();
	aco1->SetPosition(70, 0);
	//rotatorComp2->SetRotationSpeed(-2.5);
	auto acocomponent = aco1->AddNGetComponent<dae::TextureComponent>();
	acocomponent->SetTexture("SPOSA1.png");
	aco1->SetParent(*rotBase);

	//auto moveComponent = aco1->AddNGetComponent<dae::MoveComponent>(10.f);
	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(aco1.get(), dae::Direction::left),
		static_cast<unsigned int>(SDL_SCANCODE_A), dae::KeyState::down);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(aco1.get(), dae::Direction::up),
		static_cast<unsigned int>(SDL_SCANCODE_W), dae::KeyState::down);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(aco1.get(), dae::Direction::right),
		static_cast<unsigned int>(SDL_SCANCODE_D), dae::KeyState::down);

	inputManager.GetInputMethod(0)->AddAction(
		std::make_unique<dae::MoveCommand>(aco1.get(), dae::Direction::down),
		static_cast<unsigned int>(SDL_SCANCODE_S), dae::KeyState::down);


	auto aco2 = std::make_unique<dae::GameObject>();
	aco2->SetPosition(200, 100);
	auto acocomponent2 = aco2->AddNGetComponent<dae::TextureComponent>();
	acocomponent2->SetTexture("SPOSA1.png");
	//aco2->SetParent(*aco1);

	/*
	DPAD_RIGHT = 0x800,
    DPAD_LEFT = 0x400,
    DPAD_DOWN = 0x200,
    DPAD_UP = 0x100,
	*/

	//moveComponent = aco2->AddNGetComponent<dae::MoveComponent>(10.f);
	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(aco2.get(), dae::Direction::left),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_LEFT), dae::KeyState::down);
	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(aco2.get(), dae::Direction::up),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_UP), dae::KeyState::down);
	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(aco2.get(), dae::Direction::right),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_RIGHT), dae::KeyState::down);
	inputManager.GetInputMethod(1)->AddAction(
		std::make_unique<dae::MoveCommand>(aco2.get(), dae::Direction::down),
		static_cast<unsigned int>(SDL_GAMEPAD_BUTTON_DPAD_DOWN), dae::KeyState::down);



	scene.Add(std::move(rotBase));
	scene.Add(std::move(aco1));
	scene.Add(std::move(aco2));



	std::string fps{ "FPS" };
	
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