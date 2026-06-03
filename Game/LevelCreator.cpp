#include "LevelDataContainer.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_pixels.h>
#include <glm/fwd.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <string> 
#include <memory>
#include <utility>

#include <GameObject.h>
#include <Components.h>
#include <TileGrid/Tilegrid.h>
#include <TileGrid/GridMove.h>
#include <collision/Collider.h>
#include <Transform.h>
#include <inputsystems/InputManager.h>
#include <SceneManager.h>
#include <Scene.h>
#include <ResourceManager.h>
#include <buttons/Selector.h>
#include <buttons/Button.h>

#include <inputsystems/InputManager.h>

#include "command/NobbinMoveCommand.h"
#include "command/GameCommand.h"
#include "GameTile.h"
#include "Digger.h"
#include "Nobbin.h"
#include "NobbinAi.h"
#include "GoldBag.h"
#include "Gem.h"
#include "Spawner.h"

#include <iostream>


namespace digger
{
	static void function()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene();
		auto & player1 = dae::InputManager::GetInstance().m_inputs[0];
		auto& player2 = dae::InputManager::GetInstance().m_inputs[0];
		dae::SceneManager::GetInstance().SetActiveScene(1);
		LevelDataContainer::GetInstance().BuildScene(0, scene, gameMode::normal, player1.get(), player2.get());
		
	}

	void LevelDataContainer::LoadData(std::string file)
	{
		std::ifstream reader{};
		reader.open(dae::ResourceManager::GetInstance().GetFullDataPath(file));

		if (!reader.is_open())
			return;

		std::string line;

		LevelData* level{};

		while (std::getline(reader, line))
		{
			if (line.empty()) continue;

			//comments
			if (line.find("//") != std::string::npos) continue;

			if (line.find("Level") != std::string::npos)
			{
				levels.push_back({});
				level = &levels.back();
				continue;
			}
			int width{};
			for (auto& character : line)
			{
				level->tiles.push_back(static_cast<levelType>(character - '0'));
				width++;
			}
			level->width = width; //yes, it will be set to the width of the last line in the level
			level->height++;

		}
	}

	void LevelDataContainer::BuildStartScreen(dae::Scene& scene)
	{
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextureComponent>();
		go->GetComponents<dae::TextureComponent>()[0]->SetTexture("digger_score_bg.png");
		scene.Add(std::move(go));


		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

		auto higScores{ loadHighScores("highScores.csv")};

		float xBaseOffset{ 120.f };
		float xOffset{ 200.f };
		float yOffset{ 170.f };
		for (auto& score : higScores)
		{
			go = std::make_unique<dae::GameObject>();
			//go2->SetParent(*go.get());
			auto textComponent = go->AddNGetComponent<dae::TextComponent>(score.name, font);
			textComponent->SetColor({ 255, 220, 0, 255 });
			textComponent->m_offset.SetPosition(glm::vec2(xBaseOffset, yOffset));

			auto textComponent2 = go->AddNGetComponent<dae::TextComponent>(std::to_string(score.score), font);
			textComponent2->SetColor({ 255, 220, 0, 255 });
			textComponent2->m_offset.SetPosition( glm::vec2(xBaseOffset + xOffset, yOffset));

			scene.Add(std::move(go));
			yOffset += 50;
		}

		auto inputTypes{ dae::InputMethod::GetAvailableInputs() };

		
		for (auto & inputType : inputTypes)
		{
			dae::InputManager::GetInstance().AddInputMethod(inputType);
		}



		//const int gamepadCount = dae::ControllerInput::GetConnectedGamePadCount();
		//bool const hasKeyBoard = dae::KeyBoardInput::HasKeyboard();

		//if (gamepadCount < 1)
		//{
		//	// only 1 player option
		//}

		//if (gamepadCount >= 2)
		//{
		//	//use gamepad for player 2
		//}

		go = std::make_unique<dae::GameObject>();

		//go2->SetParent(*go.get());
		auto selector = go->AddNGetComponent<dae::ButtonSelector>();

		auto but = std::make_unique<dae::GameObject>();
		auto button = but->AddNGetComponent<dae::Button>("play", font, SDL_Color{ 255, 210, 0, 255 }, SDL_Color{ 255, 0,0,255 }, & function);

		selector->AddButton(*button);
		selector->SetSelected(0);

		but->m_transform.SetLocalPosition(500, 200);
		go->Add(std::move(but));

		but = std::make_unique<dae::GameObject>();
		button = but->AddNGetComponent<dae::Button>("vs", font, SDL_Color{ 255, 210, 0, 255 }, SDL_Color{ 255, 0,0,255 }, &function);

		selector->AddButton(*button);
		selector->SetSelected(0);

		but->m_transform.SetLocalPosition(500, 300);
		go->Add(std::move(but));

		/*auto button = go->AddNGetComponent<dae::TextComponent>(std::to_string(score.score), font);
		textComponent2->SetColor({ 255, 220, 0, 255 });
		textComponent2->m_offset.SetPosition(glm::vec2(xBaseOffset + xOffset, yOffset));*/

		scene.Add(std::move(go));
		
	}

	std::vector<HighScore> LevelDataContainer::loadHighScores(std::string filename)
	{
		std::vector<HighScore> scores;

		std::ifstream file(dae::ResourceManager::GetInstance().GetFullDataPath(filename));

		if (!file.is_open())
		{
			return scores;
		}

		std::string line;

		while (std::getline(file, line))
		{
			if (line.empty())
			{
				continue;
			}

			std::stringstream stream(line);

			std::string name;
			std::string scoreString;

			if (!std::getline(stream, name, ','))
			{
				continue;
			}

			if (!std::getline(stream, scoreString))
			{
				continue;
			}

			try
			{
				HighScore score;
				score.name = std::move(name);
				score.score = std::stoi(scoreString);

				scores.push_back(std::move(score));
			}
			catch (...)
			{
				// Skip malformed entries
				continue;
			}
		}

		return scores;
	}

	bool LevelDataContainer::SaveHighScores( std::string & filename, std::vector<HighScore> const &scores)
	{
		std::ofstream file(dae::ResourceManager::GetInstance().GetFullDataPath(filename));

		if (!file.is_open())
		{
			return false;
		}

		for (const auto& score : scores)
		{
			file << score.name
				<< ','
				<< score.score
				<< '\n';
		}

		return true;
	}


	void LevelDataContainer::BuildScene(int level, dae::Scene& scene, gameMode mode, dae::InputMethod* player1, dae::InputMethod* player2)
	{

		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextureComponent>();
		go->GetComponents<dae::TextureComponent>()[0]->SetTexture("digger_play_bg.png");
		scene.Add(std::move(go));


		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

		go = std::make_unique<dae::GameObject>();
		//go2->SetParent(*go.get());
		auto textComponent = go->AddNGetComponent<dae::TextComponent>("Programming 4 Assignment", font);
		textComponent->m_offset.SetPosition(292, 20);
		textComponent->SetColor({ 255, 255, 0, 255 });
		scene.Add(std::move(go));


		auto grid = std::make_unique<dae::GameObject>();

		auto gridComp = grid->AddNGetComponent<dae::Tilegrid>(levels[level].width, levels[level].height, 10.f);
		gridComp->MakeGrid<digger::GameTile>(true, "ground.png", "ground_dug.png");
		grid->SetPosition(150, 80);

		for (int x{}; x < levels[level].width; x++)
		{
			for (int y{}; y < levels[level].height; y++)
			{
				int tileIDX = x + levels[level].width * y;
				auto tile = gridComp->GetTile(glm::ivec2(x, y))->GetComponent<digger::GameTile>();

				std::unique_ptr<dae::GameObject> obj{ nullptr };

				{
					switch (levels[level].tiles[tileIDX])
					{
					case digger::levelType::empty:
						break;
					case digger::levelType::hole:
						tile->SetTraversed();
						break;
					case digger::levelType::digger:
						obj = MakeDigger(player1, *gridComp, x, y);
						tile->SetTraversed();
						break;
					case digger::levelType::spawner:
						obj = MakeSpawner(*gridComp, x, y, level + 3);
						tile->SetTraversed();
						break;
					case digger::levelType::goldbag:
						obj = MakeBag(*gridComp, x, y);
						break;
					case digger::levelType::gem:
						obj = MakeGem(*gridComp, x, y);
						break;


						// mode dependant
					case digger::levelType::digger2:
						if (mode == digger::gameMode::coop)
						{
							obj = MakeDigger(player2, *gridComp, x, y);
						}
						tile->SetTraversed();
						break;
					case digger::levelType::nobbin:
						if (mode == digger::gameMode::vs)
						{
							obj = MakeNobbin_player(player2, *gridComp, x, y);
						}
						tile->SetTraversed();
						break;
					default:
						break;
					}
				}


				if (obj.get() == nullptr) continue; // no object to add on tile;
				grid->Add(std::move(obj));
			}
		}

		scene.Add(std::move(grid));
	}

	std::unique_ptr<dae::GameObject> LevelDataContainer::MakeDigger(dae::InputMethod* input, dae::Tilegrid& in_grid, int x, int y)
	{
		auto player = std::make_unique<dae::GameObject>();

		auto imgComp = player->AddNGetComponent<dae::TextureComponent>();
		imgComp->SetTexture("digger.png");
		imgComp->renderScale = in_grid.GetTile(glm::ivec2(0, 0))->GetComponent<dae::TextureComponent>()->renderScale;

		auto col = player->AddNGetComponent<dae::Collider>();

		auto gridmove = player->AddNGetComponent<dae::GridMove>(in_grid, *col, glm::ivec2(x, y), 60.f);
		gridmove->SetInfluence(false, false);
		player->AddNGetComponent<digger::Digger>(*gridmove, *col);


		auto healthComponent = player->AddNGetComponent<dae::IntTracker>(3, 3, 0);
		auto pointsComponent = player->AddNGetComponent<dae::IntTracker>(0, 500, 0);


		SDL_Scancode input1[6]{
			SDL_SCANCODE_W ,
			SDL_SCANCODE_A ,
			SDL_SCANCODE_S ,
			SDL_SCANCODE_D ,
			SDL_SCANCODE_E ,
			SDL_SCANCODE_Q };
		SDL_GamepadButton input2[6]{
			SDL_GAMEPAD_BUTTON_DPAD_UP ,
			SDL_GAMEPAD_BUTTON_DPAD_LEFT ,
			SDL_GAMEPAD_BUTTON_DPAD_DOWN ,
			SDL_GAMEPAD_BUTTON_DPAD_RIGHT ,
			SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER ,
			SDL_GAMEPAD_BUTTON_LEFT_SHOULDER };


		unsigned int inputCodes[6]{};

		if (auto in = dynamic_cast<dae::KeyBoardInput*>(input))
		{
			for (int i{}; i < 6; i++)
			{
				inputCodes[i] = static_cast<unsigned int>(input1[i]);

			}
		}
		else
		{
			for (int i{}; i < 6; i++)
			{
				inputCodes[i] = static_cast<unsigned int>(input2[i]);

			}
		}

		input->AddAction(
			std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::left),
			static_cast<unsigned int>(inputCodes[1]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::up),
			static_cast<unsigned int>(inputCodes[0]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::right),
			static_cast<unsigned int>(inputCodes[3]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<digger::DiggerMoveCommnad>(gridmove, dae::Direction::down),
			static_cast<unsigned int>(inputCodes[2]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<dae::HurtCommand>(healthComponent),
			static_cast<unsigned int>(inputCodes[4]), dae::KeyState::released);

		input->AddAction(
			std::make_unique<dae::HurtCommand>(pointsComponent, false),
			static_cast<unsigned int>(inputCodes[5]), dae::KeyState::down);

		return player;
	}

	std::unique_ptr<dae::GameObject> LevelDataContainer::MakeNobbin(dae::Tilegrid& in_grid, int x, int y)
	{
		auto nob = std::make_unique<dae::GameObject>();

		auto imgComp = nob->AddNGetComponent<dae::TextureComponent>();
		imgComp->renderScale = in_grid.GetTile(glm::ivec2(0, 0))->GetComponent<dae::TextureComponent>()->renderScale;

		auto col = nob->AddNGetComponent<dae::Collider>();


		auto gridmove = nob->AddNGetComponent<dae::GridMove>(in_grid, *col, glm::ivec2(x, y), 40.f);
		gridmove->SetInfluence(false, false);
		nob->AddComponent<digger::Nobbin>(*gridmove, *col, *imgComp);

		return nob;
	}

	std::unique_ptr<dae::GameObject> LevelDataContainer::MakeNobbin_ai(dae::Tilegrid& in_grid, int x, int y)
	{
		auto nob = MakeNobbin(in_grid, x, y);
		auto nobbin = nob->GetComponent<digger::Nobbin>();
		auto gridmove = nob->GetComponent<dae::GridMove>();

		 nob->AddNGetComponent<digger::NobbinAI>(*gridmove, *nobbin);

		

		return nob;
	}

	std::unique_ptr<dae::GameObject> LevelDataContainer::MakeNobbin_player(dae::InputMethod* input, dae::Tilegrid& in_grid, int x, int y)
	{
		auto nob = MakeNobbin(in_grid, x, y);

		auto healthComponent = nob->AddNGetComponent<dae::IntTracker>(3, 3, 0);
		auto pointsComponent = nob->AddNGetComponent<dae::IntTracker>(0, 500, 0);


		SDL_Scancode input1[6]{
			SDL_SCANCODE_W ,
			SDL_SCANCODE_A ,
			SDL_SCANCODE_S ,
			SDL_SCANCODE_D ,
			SDL_SCANCODE_E ,
			SDL_SCANCODE_Q };
		SDL_GamepadButton input2[6]{
			SDL_GAMEPAD_BUTTON_DPAD_UP ,
			SDL_GAMEPAD_BUTTON_DPAD_LEFT ,
			SDL_GAMEPAD_BUTTON_DPAD_DOWN ,
			SDL_GAMEPAD_BUTTON_DPAD_RIGHT ,
			SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER ,
			SDL_GAMEPAD_BUTTON_LEFT_SHOULDER };


		unsigned int inputCodes[6]{};

		if (auto in = dynamic_cast<dae::KeyBoardInput*>(input))
		{
			for (int i{}; i < 6; i++)
			{
				inputCodes[i] = static_cast<unsigned int>(input1[i]);

			}
		}
		else
		{
			for (int i{}; i < 6; i++)
			{
				inputCodes[i] = static_cast<unsigned int>(input2[i]);

			}
		}

		auto gridmove = nob->GetComponent<dae::GridMove>();
		auto nobbin = nob->GetComponent<digger::Nobbin>();

		input->AddAction(
			std::make_unique<digger::NobbinMoveCommnad>(gridmove, dae::Direction::left, *nobbin),
			static_cast<unsigned int>(inputCodes[1]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<digger::NobbinMoveCommnad>(gridmove, dae::Direction::up, *nobbin),
			static_cast<unsigned int>(inputCodes[0]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<digger::NobbinMoveCommnad>(gridmove, dae::Direction::right, *nobbin),
			static_cast<unsigned int>(inputCodes[3]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<digger::NobbinMoveCommnad>(gridmove, dae::Direction::down, *nobbin),
			static_cast<unsigned int>(inputCodes[2]), dae::KeyState::down);

		input->AddAction(
			std::make_unique<dae::HurtCommand>(healthComponent),
			static_cast<unsigned int>(inputCodes[4]), dae::KeyState::released);

		input->AddAction(
			std::make_unique<dae::HurtCommand>(pointsComponent, false),
			static_cast<unsigned int>(inputCodes[5]), dae::KeyState::down);

		return nob;
	}

	std::unique_ptr<dae::GameObject> LevelDataContainer::MakeGem(dae::Tilegrid& in_grid, int x, int y)
	{
		auto gem = std::make_unique<dae::GameObject>();
		auto gemCol = gem->AddNGetComponent<dae::Collider>();
		gemCol->isTrigger = true;
		gemCol->size.x = in_grid.GetTileScale() * 0.75f;
		gemCol->size.y = in_grid.GetTileScale() * 0.75f;

		gem->AddComponent<digger::Gem>(*gemCol);

		auto gemTex = gem->AddNGetComponent<dae::TextureComponent>();
		gemTex->SetTexture("gem.png");

		

		auto gemPos = in_grid.GetGridLocationOfPoint(glm::ivec2(x, y));
		gem->m_transform.SetLocalPosition(gemPos.x, gemPos.y);

		return std::move(gem);
	}

	std::unique_ptr<dae::GameObject> LevelDataContainer::MakeBag(dae::Tilegrid& in_grid, int x, int y)
	{
		auto goldBag = std::make_unique<dae::GameObject>();

		auto imgComp = goldBag->AddNGetComponent<dae::TextureComponent>();
		imgComp->SetTexture("gold_bag.png");
		imgComp->renderScale = in_grid.GetTile(glm::ivec2(0, 0))->GetComponent<dae::TextureComponent>()->renderScale;

		auto col = goldBag->AddNGetComponent<dae::Collider>();

		auto gridmove = goldBag->AddNGetComponent<dae::GridMove>(in_grid, *col, glm::ivec2(x, y), 40.f);
		gridmove->SetInfluence(true, false);

		goldBag->AddComponent<digger::GoldBag>(*imgComp, *gridmove, *col, "gold.png");

		return goldBag;
	}
	
	std::unique_ptr<dae::GameObject> LevelDataContainer::MakeSpawner(dae::Tilegrid& in_grid, int x, int y, int amount)
	{
		auto spawn = std::make_unique<dae::GameObject>();

		

		spawn->AddNGetComponent<digger::Spawner>(in_grid, x, y, amount);

		return std::move(spawn);
	}
}