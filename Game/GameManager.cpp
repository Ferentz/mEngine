#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include <components/RenderComponent.h>

#include "Gem.h"
#include "Spawner.h"
#include "PointsTracker.h"
#include "LevelDataContainer.h"
#include "HighScoreHandler.h"
#include "Digger.h"
#include "NobbinAi.h"
#include "Nobbin.h"
#include "command/SkipCommand.h"
#include "command/MuteCommand.h"

#include "SoundManager.h"

#include <tileGrid/GridMove.h>

#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>
#include <inputsystems/InputManager.h>

#include <iostream>

namespace digger
{

	GameManager::GameManager(dae::GameObject& parent, PointsTracker& pt, dae::GameObject& levelroot, gameMode m, dae::InputMethod * p1, dae::InputMethod* p2)
		:GameComponent{ parent }
		, pointsTracker{&pt }
		, levelRoot{ &levelroot }
		, mode{m}
		, player1{p1}
		, player2{p2}
	{

		dae::EventStack::GetEventStack().Register(*this);

		/*auto& inputs = dae::InputManager::GetInstance().m_inputs;
		player1 = inputs[0].get();
		player2 = inputs.size() > 1 ? inputs[1].get() : inputs[0].get();*/

		
		lifeDisplays.emplace_back();
		if (m == gameMode::coop)
		{
			lifeDisplays.emplace_back();
			remainingLives2 = remainingLives;
		}

		int start{ 120 };
		int spacing{ 40 };

		for (auto& disp : lifeDisplays)
		{
			for (int i{}; i < maxLives-1; i++)
			{
				disp.push_back(GetGameObject()->AddNGetComponent<dae::TextureComponent>());
				disp.back()->SetTexture("digger.png");
				disp.back()->m_offset.SetPosition(float(start + spacing*i), 0);
			}
			start = 800;
			spacing = -spacing;
		}

		updateLife();

		auto keyboard{ dae::InputManager::GetInstance().GetInputMethod(dae::InputType::keyboard) };
		keyboard->AddAction(std::make_unique<SkipCommand>(this), SDL_SCANCODE_F1, dae::KeyState::presed);
	}

	GameManager::~GameManager()
	{
		dae::EventStack::GetEventStack().Unregister(this);
	}


	void GameManager::Start()
	{
		// we wanna get all gems and 
		GetAllNeeded();
		dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("start game"), nullptr });
		dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("start level"), nullptr });
	}

	void GameManager::Update(float )
	{
		// we wanna get all gems and 
		//GetAllNeeded();
		//std::cout << "wtf";
	}

	void GameManager::TuneIn(dae::EventId id , dae::GameObject* subject)
	{
		switch (id)
		{
			case dae::make_sdbm_hash("died player"):
			{

				if (GetObject(player1) == subject)
				{
					remainingLives--;
				}
				else
				{
					remainingLives2--;
				}

				
				if (remainingLives + remainingLives2 == 0)
				{
					//end game;
					EndGame();
					return;
				}
				//subject->GetComponent<Digger>()->Respawn();
				ReturnNobbins();
				updateLife();
				break;
			}
			case dae::make_sdbm_hash("treshhold collected"):
			{
				if (remainingLives < maxLives)
				remainingLives++;
				if (remainingLives2 < maxLives)
				remainingLives2++;
				break;
			}
			case dae::make_sdbm_hash("collect gem"):
			{
				remainingGems--;
				if (remainingGems <= 0)
				{
					//next level
					NextLevel();
					return;
				}
				break;
			}
			case dae::make_sdbm_hash("death enemy"):
			{
				remainingNobbins--;
				if (remainingNobbins <= 0)
				{
					// next level
					NextLevel();
					return;
				}
				break;
			}
			case dae::make_sdbm_hash("name entered"):
			{
				std::cout << "name enteredd\n";
				SaveHighScore(subject->GetComponent<HighScoreHandler>()->GetUsername());
				break;
			}
		}
	}

	void GameManager::ReturnNobbins()
	{
		//auto& objects{ dae::SceneManager::GetInstance().GetActiveScene()->GetObjects() };


		auto nobbins{ levelRoot->GetComponents_ChildrenInclusive<Nobbin>() };
		for (auto& nobbin : nobbins)
		{
			nobbin->Respawn();
		}

		if (remainingLives>0)
		{
			GetObject(player1)->GetComponent<Digger>()->Respawn();
		}
		/*else if (remainingLives == 0)
		{
			GetObject(player1)->MarkForDelete();
		}*/

		if(remainingLives2 > 0)
		{
			GetObject(player2)->GetComponent<Digger>()->Respawn();
		}
		/*else if (remainingLives2 == 0)
		{
			GetObject(player2)->MarkForDelete();
		}*/
		
	}

	void GameManager::GetAllNeeded()
	{
		auto & objects{ dae::SceneManager::GetInstance().GetActiveScene()->GetObjects() };
		remainingGems = 0;
		remainingNobbins = 0;
		for (auto & object : objects)
		{
			remainingGems += object->CountComponents_childrenInclusive<Gem>();
			if (auto spawner = object->GetComponent_ChildrenInclusive<Spawner>())
			{
				remainingNobbins += spawner->NumToSpawn();
			}
		}
		std::cout << remainingNobbins << " remaining nobbins\n";
		std::cout << remainingGems << " remaining gems\n";
	}

	void GameManager::NextLevel()
	{
		dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("start level"), nullptr });
		std::cout << "\nnext level";
		auto const& sceneObjects{ levelRoot->GetChildren() };

		for (auto& object : sceneObjects)
		{
			object->MarkForDelete();
		}

		player1->ClearActions();
		player2->ClearActions();
		EnsureSkipFunc();

		if (mode == gameMode::coop)
		{
			if (remainingLives == 0) remainingLives++;
			if (remainingLives2 == 0) remainingLives2++;
		}

		curentLevel++;

		if (curentLevel >= MaxLevel)
		{
			EndGame();
			return;
		}

		LevelDataContainer::GetInstance().BuildLevel(curentLevel, *levelRoot, mode, player1, player2);

		GetAllNeeded();
	}

	void GameManager::EnsureSkipFunc()
	{
		auto keyboard{ dae::InputManager::GetInstance().GetInputMethod(dae::InputType::keyboard) };
		if (player1 == keyboard || player2 == keyboard)
		{
			std::cout << "\nadding skip func";
			keyboard->AddAction(std::make_unique<SkipCommand>(this), SDL_SCANCODE_F1, dae::KeyState::presed);
			keyboard->AddAction(std::make_unique<digger::MuteCommand>(),
				static_cast<unsigned int>(SDL_SCANCODE_F2), dae::KeyState::presed);
		}
	}

	void GameManager::EndGame()
	{
		dae::EventStack::GetEventStack().PushEvent(dae::Event{ dae::make_sdbm_hash("end game"), nullptr });
		std::cout << "\nendgame";
		//levelRoot->MarkForDelete();

		auto& highscores{ LevelDataContainer::GetInstance().GetHighScores() };

		if (highscores.size() < 5)
		{
			// record highscore
			RecordHighScore();
			return;
		}

		for (auto& score : highscores)
		{
			if (score.score < pointsTracker->GetPoints())
			{
				// record highscore
				RecordHighScore();
				return;
			}
		}

		dae::SceneManager::GetInstance().SetActiveScene(0);

		// go back to start screen
	}

	void GameManager::RecordHighScore()
	{
		auto const& sceneObjects{ levelRoot->GetChildren() }; 
		
		for (auto& object : sceneObjects)
		{
			object->MarkForDelete();
		}

		
		player1->ClearActions();
		player2->ClearActions();
		

		LevelDataContainer::GetInstance().BuildEndScreen(*levelRoot);

	}

	void GameManager::SaveHighScore(std::string & name)
	{
		auto& highscores{ LevelDataContainer::GetInstance().GetHighScores() };

		HighScore newscore{ .name = name, .score = pointsTracker->GetPoints() };
		bool entered{ false };
		for (size_t i{}; i < highscores.size(); i++)
		{
			if (highscores[i].score < pointsTracker->GetPoints())
			{
				highscores.insert(std::next(highscores.begin(), i), newscore);
				entered = true;
				break;
			}
		}
		if (!entered)
		{
			highscores.push_back(newscore);
		}

		if (highscores.size() > 5)
		{
			highscores.resize(5);
		}

		LevelDataContainer::GetInstance().SaveHighScores( highscores);

		dae::SceneManager::GetInstance().SetActiveScene(0);

		LevelDataContainer::GetInstance().CreateLeaderBoard();
	}

	void GameManager::updateLife()
	{
		int remLife = remainingLives;
		for (auto& disp : lifeDisplays)
		{
			int displayed{};
			for (auto life : disp)
			{
				if (remLife - 1 > displayed)
				{
					life->isVisible = true;
				}
				else
				{
					life->isVisible = false;;
				}
				displayed++;
			}
			remLife = remainingLives2;
		}
		
	}

	dae::GameObject* GameManager::GetObject(dae::InputMethod * playerInput)
	{
		auto com = dynamic_cast<dae::GameCommand<dae::GridMove> const*>(playerInput->GetActions()[2]->m_commands.get());
		return com->GetSubject()->GetGameObject();
	}
}