#include "Spawner.h"

#include "LevelDataContainer.h"
#include <SceneManager.h>

namespace digger
{
	Spawner::Spawner(dae::GameObject& object, dae::Tilegrid& tilegrid, int x, int y, int load)
		:GameComponent{object}
		, spawnPos{x,y}
		, grid{&tilegrid }
		, numToSpawn{load}
	{

	}
	void Spawner::Update(float delta)
	{
		if (numToSpawn == 0) return;

		timer += delta;
		if (timer >= interval )
		{
			timer = 0;
			if (numToSpawn > 0) numToSpawn--;
			auto nob = LevelDataContainer::MakeNobbin_ai(*grid, spawnPos.x, spawnPos.y);
			dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(nob));
			// spawn nobbin
		}
	}
}