#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"

#include "Gem.h"
#include "Spawner.h"
#include "PointsTracker.h"
#include "LevelDataContainer.h"
#include "HighScoreHandler.h"

#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>
#include <inputsystems/InputManager.h>

#include <iostream>

namespace digger
{

	GameManager::GameManager(dae::GameObject& parent, PointsTracker& pt, dae::GameObject& levelroot, gameMode m)
		:GameComponent{ parent }
		, pointsTracker{&pt }
		, levelRoot{ &levelroot }
		, mode{m}
	{

		dae::EventStack::GetEventStack().Register(*this);

		auto& inputs = dae::InputManager::GetInstance().m_inputs;
		player1 = inputs[0].get();
		player2 = inputs.size() > 1 ? inputs[1].get() : inputs[0].get();

	}

	void GameManager::Start()
	{
		// we wanna get all gems and 
		GetAllNeeded();
	}

	void GameManager::Update(float )
	{
		// we wanna get all gems and 
		//GetAllNeeded();
		//std::cout << "wtf";
	}

	void GameManager::TuneIn(dae::EventId id , dae::GameObject* subject)
	{
		if (id == dae::make_sdbm_hash("died player"))
		{
			EndGame();
			remainingLives--;
			if (remainingLives == 0)
			{
				//end game;
				
			}
		}
		else if (id == dae::make_sdbm_hash("treshhold collected"))
		{
			if (remainingLives >= maxLives) return;
			remainingLives++;
		}
		else if (id == dae::make_sdbm_hash("collect gem"))
		{
			remainingGems--;
			if (remainingGems <= 0)
			{
				//next level
				NextLevel();

			}
		}
		else if (id == dae::make_sdbm_hash("death enemy"))
		{
			remainingNobbins--;
			if (remainingNobbins <= 0)
			{
				// next level
				NextLevel();
			}
		}
		else if (id == dae::make_sdbm_hash("name entered"))
		{
			SaveHighScore(subject->GetComponent<HighScoreHandler>()->GetUsername());
		}
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

	}

	void GameManager::NextLevel()
	{
		auto const& sceneObjects{ levelRoot->GetChildren() };

		for (auto& object : sceneObjects)
		{
			object->MarkForDelete();
		}

		player1->ClearActions();
		player2->ClearActions();

		curentLevel++;

		if (curentLevel >= MaxLevel)
		{
			EndGame();
			return;
		}

		LevelDataContainer::GetInstance().BuildLevel(curentLevel, *levelRoot, mode, player1, player2);

		GetAllNeeded();
	}

	void GameManager::EndGame()
	{
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

		LevelDataContainer::GetInstance().SaveHighScores("new_highScores.csv", highscores);

		dae::SceneManager::GetInstance().SetActiveScene(0);
	}
}