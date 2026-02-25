#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"

#include <filesystem>
#include "GameComponent.h"
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

	auto rotBase = std::make_unique<dae::RotatingObject>();
	rotBase->SetPosition(200, 200);

	auto aco1 = std::make_unique<dae::RotatingObject>();
	aco1->SetPosition(70, 0);
	aco1->SetRotationSpeed(2.5);
	auto acocomponent = aco1->AddNGetComponent<dae::TextureComponent>();
	acocomponent->SetTexture("SPOSA1.png");
	aco1->SetParent(*rotBase);

	auto aco2 = std::make_unique<dae::GameObject>();
	aco2->SetPosition(20, 0);
	auto acocomponent2 = aco2->AddNGetComponent<dae::TextureComponent>();
	acocomponent2->SetTexture("SPOSA1.png");
	aco2->SetParent(*aco1);

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